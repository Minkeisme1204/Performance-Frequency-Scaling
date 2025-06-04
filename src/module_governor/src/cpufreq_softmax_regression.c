#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cpu_freq.h>
#include <linux/init.h>
#include "../include/cpu_info_logger.h"

#define DEF_SAMPLING_RATE (100*1000)
#define DEF_POWERSAVE_BIAS 0
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

static struct cpufreq_governor ml_gov; 

struct mlSoftmax_data {
	struct cpufreq_policy *policy;
	struct delayed_work work; 
	struct gov_attr_set attr_set; 
	unsigned int sampling_rate; 
	unsigned int powersave_bias; 
}

static void mlSoftmax_sample (struct work_struct *work) {
	struct mlSoftmax_data *data = container_of(work, struct mlSoftmax_data, work.work); 
	struct cpufreq_policy *policy = data->policy;
	unsigned int new_freq, cur_freq; 

	/*Collect data block*/
	float cpu_util = get_cpu_utilization(); 
	unsigned int onl_cores = count_onlines_cores(); 
	float avg_load = get_load_average();
	float temperature = get_cpu_temperature(); 
	unsigned int run_queue = get_run_queue_length(); 
	cur_freq = policy->cur;

	/*Inference collected data*/
	/*????????????????????????*/
	
	/*Check condition and change the freq if necessary*/
	if (new_freq != cur_freq) {
		cpufreq_driver_target(policy, new_freq, CPUFREQ_RELATION_H); 
	}

	/*Schedule the next sampling phase*/
	queue_delayed_work_on(policy->cpu, system_highpri_wq, &data->work, msecs_to_jiffies(data->sampling_rate)); 
}

static int mlSoftmax_init_policy (struct cpufreq_policy *policy) {
	struct mlSoftmax_data *data; 
	
	data = kzalloc(sizeof(*data), GFP_KERNEL); 
	if (data == NULL) return -ENOMEM; 
	
	data->policy = policy; 
	data->sampling_rate = DEF_SAMPLING_RATE;
	data->powersave_bias = 0; 

	policy->governor_data = data; // Assign custom data to "governor_data" for further access in .start, .stop, .limits

	gov_attr_set_init(&data->attr_set, policy->cpu); // Create Folder Files manage 2 above attr
	return 0; 
}

static void mlSoftmax_exit_policy (struct cpufreq_policy *policy) {
	struct mlSoftmax_data *data = policy->governor_data; 

	gov_attr_set_destroy(&data->attr_set);
	kfree(data);
}

static int mlSoftmax_start(struct cpufreq_policy *policy) {
	struct mlSoftmax_data *data = policy->governor_data; 
	
	INIT_DELAYED_WORK(&data->work, /*Sampling function name*/);
	queue_delayed_work_on(policy->cpu, system_highpri_wq, &data->work, msecs_to_jiffies(data->sampling_rate)); 
}

static void mlSoftmax_stop(struct cpufreq_policy *policy) {
	struct mlSoftmax_data *data = policy->governor_data;
	cancel_delayed_work_sync(&data->work); 
}

static void mlSoftmax_limits(struct cpufreq_policy *policy) {
	struct mlSoftmax_data *data = policy->governor_data; 
	unsigned int freq = policy->cur; 

	if (freq > policy->max) freq = policy->max; 
	else if (freq < policy->min) freq = policy->min; 

	__cpufreq_driver_target(policy, freq, CPUFREQ_RELATION_L); 
}

static struct cpufreq_governor cpufreq_gov_mlSoftmax = {
	.name = "mlSoftmax", 
	.owner = THIS_MODULE, 
	.flags = CPUFREQ_GOV_DYNAMIC, 

	.init = mlSoftmax_init_policy, 
	.exit = mlSoftmax_exit_policy, 
       	.start = mlSoftmax_start, 
	.stop = mlSoftmax_stop, 
	.limits = mlSoftmax_limits,
	.adjust_perf = NULL, 
	.update = NULL,	
};

MODULE_AUTHOR("Nguyen Cao Minh <minhnguyencao1204@gmail.com>"); 
MODULE_DESCRIPTION("CPUfreq policy governor 'mlSoftmax' based on Softmax Regression ML Algorithm"); 
MODULE_LICENSE("GPL"); 

cpufreq_governor_init(cpu_gov_mlSoftmax); 
cpufreq_governor_exit(cpu_gov_mlSoftmax); 


