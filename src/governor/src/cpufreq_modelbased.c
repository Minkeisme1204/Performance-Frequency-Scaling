#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cpufreq.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/jiffies.h>
#include "cpufreq_governor.h"

#include "cpufreq_log_feature.h"
#include "model_inference.h"

#define DEF_SAMPLING_RATE 1000 // ms

int get_cpu_util(void) {
    u64 idle, total, busy;
    static u64 last_idle = 0;
    static u64 last_total = 0;

	// Get cpu stat 
    for (int i = 0; i < MAX_CPU; i++) {
        struct kernel_cpustat *kstat = &kcpustat_cpu(i);

        u64 user        = kstat->cpustat[CPUTIME_USER];
        u64 nice        = kstat->cpustat[CPUTIME_NICE];
        u64 system      = kstat->cpustat[CPUTIME_SYSTEM];
        u64 irq         = kstat->cpustat[CPUTIME_IRQ];
        u64 softirq     = kstat->cpustat[CPUTIME_SOFTIRQ];
        u64 steal       = kstat->cpustat[CPUTIME_STEAL];
        u64 guest       = kstat->cpustat[CPUTIME_GUEST];
        u64 guest_nice  = kstat->cpustat[CPUTIME_GUEST_NICE];
        u64 iowait      = kstat->cpustat[CPUTIME_IOWAIT];
        u64 idle_raw    = kstat->cpustat[CPUTIME_IDLE];\

        idle = idle_raw + iowait;
        busy = user + nice + system + irq + softirq + steal + guest + guest_nice;
        total = idle + busy;
    }

    u64 delta_total = total - last_total;
    u64 delta_idle  = idle  - last_idle;

    last_total = total;
    last_idle  = idle;

    if (delta_total == delta_idle) return 0;

    return delta_total / (delta_total - delta_idle);
}
//EXPORT_SYMBOL_GPL(get_cpu_util);

// Get number of online cpus 
int get_online_cpus_list(int *cpu_list, int max) {
	int count = 0; 
	int cpu; 

	for_each_online_cpu(cpu) {
		if (count >= max) break;
		cpu_list[count ++] = cpu;
	}

	return count; 
} 
//EXPORT_SYMBOL_GPL(get_online_cpus_list);

// Get Load average feature
#define FSHIFT     11
#define FIXED_1    (1 << FSHIFT)

extern unsigned long avenrun[];
int get_loadavg_percent(void) {
    return (avenrun[0] * 100) >> FSHIFT;
}
//EXPORT_SYMBOL_GPL(get_loadavg_percent);

// Get run queue length 
int get_run_queue_length(int cpu) {
    struct task_struct *p;
    unsigned int count = 0;

    rcu_read_lock();
    for_each_process(p) {
        if (task_cpu(p) == cpu && task_is_running(p))
            count++;
    }
    rcu_read_unlock();

    return count;
}
//EXPORT_SYMBOL_GPL(get_run_queue_length);
// Get temperature in milli celsius 
int get_cpu_temperature_millicelsius(void) {
    struct thermal_zone_device *tz;
    int temp = -1;

    tz = thermal_zone_get_zone_by_name("cpu-thermal");
    if (IS_ERR(tz)) {
        pr_err("thermal not found\n");
        return -1;
    }

    if (!thermal_zone_get_temp(tz, &temp))
        return temp;

    return -1;
}

int model_inference(int cpu_util, int avg_load, int temperature, int runq) {
    int res = 0; 
    if (avg_load <= 12) {
        res = 1500;
    }
    else {
        if (temperature < 60875) {
            if (avg_load <= 25) {
                if (runq <= 4) {
                    res = 1600; 
                }
                else res = 1500; // 1600 
            }
            else {
                if (runq <= 4) {
                    if (avg_load <= 49) {
                        res = 1700; // 1700
                    }
                    else res = 2400; // 2400
                }
                else {
                    if (avg_load <= 97) {
                        res = 1600; // 1600
                    }
                    else res = 1700; // 1700
                }
            }
        }
        else {
            if (runq <= 4) {
                if (cpu_util <= 2) {
                    if (avg_load <= 118) {
                        res = 2200; // 2200
                    } else {
                        res = 2400; // 2400
                    }
                }
                else res = 2400; // 2400
            }
            else {
                if (temperature <= 66925) {
                    if (temperature <= 64175) {
                        res = 1900; // 1900
                    }
                    else res = 2200; // 2200
                }
                else {
                    if(cpu_util <= 0.5) {
                        res = 2300; // 2300
                    }
                    else {
                        res = 2400; // 2400
                    }
                }
            }
        }
    }
    return res * 1000; 
}

struct modelbased_data {
	struct cpufreq_policy *policy;
	struct delayed_work work;
	struct gov_attr_set attr_set;
	unsigned int sampling_rate;
};

// Sampling function
static void modelbased_sample(struct work_struct *work)
{
	struct modelbased_data *data = container_of(work, struct modelbased_data, work.work);
	struct cpufreq_policy *policy = data->policy;

	unsigned int new_freq = 0, cur_freq;
	int cpu_util = get_cpu_util();
	int avg_load = get_loadavg_percent();
	int temperature = get_cpu_temperature_millicelsius();

	int runq = 0;
	for (int i = 0; i < num_online_cpus(); i++)
		runq += get_run_queue_length(i);

	cur_freq = policy->cur;
	new_freq = model_inference(cpu_util, avg_load, temperature, runq);

	pr_info("modelbased: util=%d, load=%d, temp=%d, runq=%d, cur=%d, new=%d\n",
	        cpu_util, avg_load, temperature, runq, cur_freq, new_freq);

	if (new_freq != cur_freq)
		cpufreq_driver_target(policy, new_freq, CPUFREQ_RELATION_H);

	queue_delayed_work_on(policy->cpu, system_highpri_wq, &data->work,
	                      msecs_to_jiffies(data->sampling_rate));
}

static int modelbased_init_policy(struct cpufreq_policy *policy)
{
	struct modelbased_data *data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->policy = policy;
	data->sampling_rate = DEF_SAMPLING_RATE;
	policy->governor_data = data;

	//gov_attr_set_init(&data->attr_set, &policy->policy_list);
	return 0;
}

static void modelbased_exit_policy(struct cpufreq_policy *policy)
{
	struct modelbased_data *data = policy->governor_data;
	//gov_attr_set_destroy(&data->attr_set);
	kfree(data);
}

static int modelbased_start(struct cpufreq_policy *policy)
{
	struct modelbased_data *data = policy->governor_data;

	INIT_DELAYED_WORK(&data->work, modelbased_sample);
	queue_delayed_work_on(policy->cpu, system_highpri_wq, &data->work,
	                      msecs_to_jiffies(data->sampling_rate));
	return 0;
}

static void modelbased_stop(struct cpufreq_policy *policy)
{
	struct modelbased_data *data = policy->governor_data;
	cancel_delayed_work_sync(&data->work);
}

static void modelbased_limits(struct cpufreq_policy *policy)
{
	//struct modelbased_data *data = policy->governor_data;
	unsigned int freq = policy->cur;

	if (freq > policy->max)
		freq = policy->max;
	else if (freq < policy->min)
		freq = policy->min;

	__cpufreq_driver_target(policy, freq, CPUFREQ_RELATION_L);
}

// Governor ops
static struct cpufreq_governor cpufreq_gov_modelbased = {
	.name = "modelbased",
	.owner = THIS_MODULE,
	.init    = modelbased_init_policy,
	.exit    = modelbased_exit_policy,
	.start   = modelbased_start,
	.stop    = modelbased_stop,
	.limits  = modelbased_limits,
};

// Register / unregister
static int __init cpu_gov_modelbased_init(void)
{
	pr_info(">>> modelbased governor init <<<\n");
	int ret = cpufreq_register_governor(&cpufreq_gov_modelbased);
	pr_info("register result = %d\n", ret);
	return ret;
}

static void __exit cpu_gov_modelbased_exit(void)
{
	cpufreq_unregister_governor(&cpufreq_gov_modelbased);
	pr_info(">>> modelbased governor exited <<<\n");
}

module_init(cpu_gov_modelbased_init);
module_exit(cpu_gov_modelbased_exit);

MODULE_AUTHOR("Nguyen Cao Minh <minhnguyencao1204@gmail.com>");
MODULE_DESCRIPTION("CPUFreq governor 'modelbased' based on Decision Tree ML Algorithm");
MODULE_LICENSE("GPL");
