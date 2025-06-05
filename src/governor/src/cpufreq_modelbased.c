#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cpufreq.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/jiffies.h>
#include "cpufreq_log_feature.h"

#define DEF_SAMPLING_RATE (100) // ms
#define DEF_POWERSAVE_BIAS 0

// Forward declaration
static void modelbased_sample(struct work_struct *work);

// Governor data
struct modelbased_data {
	struct cpufreq_policy *policy;
	struct delayed_work work;
	struct gov_attr_set attr_set;
	unsigned int sampling_rate;
	unsigned int powersave_bias;
};

// Sample function
static void modelbased_sample(struct work_struct *work)
{
	struct modelbased_data *data = container_of(work, struct modelbased_data, work.work);
	struct cpufreq_policy *policy = data->policy;
	unsigned int new_freq = 0, cur_freq;

	// Collect data
 	int cpu_util = get_cpu_util();

	int avg_load = get_loadavg_percent();
	int temperature = get_cpu_temperature_millicelsius();

	int runq = get_run_queue_length(0) + 
				get_run_queue_length(1) + 
				get_run_queue_length(2) + 
				get_run_queue_length(3); 

	cur_freq = policy->cur;

	pr_info("Collected: util=%u, load=%u, temp=%u\n", cpu_util, avg_load, temperature);

	// TODO: Replace with decision tree logic
	new_freq = cur_freq;

	if (new_freq != cur_freq) {
		cpufreq_driver_target(policy, new_freq, CPUFREQ_RELATION_H);
	}

	// Schedule next sample
	queue_delayed_work_on(policy->cpu, system_highpri_wq, &data->work, msecs_to_jiffies(data->sampling_rate));
}

static int modelbased_init_policy(struct cpufreq_policy *policy)
{
	struct modelbased_data *data;

	data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->policy = policy;
	data->sampling_rate = DEF_SAMPLING_RATE;
	data->powersave_bias = DEF_POWERSAVE_BIAS;

	policy->governor_data = data;

	// Correct param: use policy->policy_list instead of policy->cpu
	gov_attr_set_init(&data->attr_set, &policy->policy_list);
	return 0;
}

static void modelbased_exit_policy(struct cpufreq_policy *policy)
{
	struct modelbased_data *data = policy->governor_data;
	// gov_attr_set_destroy(&data->attr_set);
	kfree(data);
}

static int modelbased_start(struct cpufreq_policy *policy)
{
	struct modelbased_data *data = policy->governor_data;

	INIT_DELAYED_WORK(&data->work, modelbased_sample);
	queue_delayed_work_on(policy->cpu, system_highpri_wq, &data->work, msecs_to_jiffies(data->sampling_rate));

	return 0;
}

static void modelbased_stop(struct cpufreq_policy *policy)
{
	struct modelbased_data *data = policy->governor_data;
	cancel_delayed_work_sync(&data->work);
}

static void modelbased_limits(struct cpufreq_policy *policy)
{
	struct modelbased_data *data = policy->governor_data;
	unsigned int freq = policy->cur;

	if (freq > policy->max)
		freq = policy->max;
	else if (freq < policy->min)
		freq = policy->min;

	__cpufreq_driver_target(policy, freq, CPUFREQ_RELATION_L);
}

// Define governor
static struct cpufreq_governor cpufreq_gov_modelbased = {
	.name = "modelbased",
	.owner = THIS_MODULE,
	.flags = 0, // CPUFREQ_GOV_DYNAMIC may not exist

	.init    = modelbased_init_policy,
	.exit    = modelbased_exit_policy,
	.start   = modelbased_start,
	.stop    = modelbased_stop,
	.limits  = modelbased_limits,
};

// Module info
MODULE_AUTHOR("Nguyen Cao Minh <minhnguyencao1204@gmail.com>");
MODULE_DESCRIPTION("CPUFreq governor 'modelbased' based on Softmax Regression ML Algorithm");
MODULE_LICENSE("GPL");

// Register governor
static int __init cpu_gov_modelbased_init(void)
{
	return cpufreq_register_governor(&cpufreq_gov_modelbased);
}

static void __exit cpu_gov_modelbased_exit(void)
{
	cpufreq_unregister_governor(&cpufreq_gov_modelbased);
}

module_init(cpu_gov_modelbased_init);
module_exit(cpu_gov_modelbased_exit);