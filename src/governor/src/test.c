#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include "cpufreq_log_feature.h"

static int __init logger_init(void)
{
    int util;
    int load, temp;
    int runq; 

    pr_info("cpufreq_logger: Initializing module...\n");

    util = get_cpu_util();

	load = get_loadavg_percent();
	temp= get_cpu_temperature_millicelsius();

	runq = get_run_queue_length(0) + 
            get_run_queue_length(1) + 
            get_run_queue_length(2) + 
            get_run_queue_length(3); 

    pr_info("cpufreq_logger: CPU Utilization: %d \n", util);
    pr_info("cpufreq_logger: Load Average: %d%%\n", load);
    pr_info("cpufreq_logger: CPU Temperature: %d mC\n", temp);
    pr_info("cpufreq_logger: Run queue length: %d \n", runq);
    return 0;
}

static void __exit logger_exit(void)
{
    pr_info("cpufreq_logger: Exiting module...\n");
}

module_init(logger_init);
module_exit(logger_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Minh Nguyen");
MODULE_DESCRIPTION("A kernel module to log CPU stats from cpufreq_log.h");
