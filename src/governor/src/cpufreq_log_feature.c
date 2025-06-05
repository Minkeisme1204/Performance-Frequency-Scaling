#include "cpufreq_log_feature.h"

// Get CPU Utilization Reverse value to ensure not to be vanished
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

// Get Load average feature
#define FSHIFT     11
#define FIXED_1    (1 << FSHIFT)

extern unsigned long avenrun[];
int get_loadavg_percent(void) {
    return (avenrun[0] * 100) >> FSHIFT;
}

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

EXPORT_SYMBOL(get_cpu_util);
EXPORT_SYMBOL(get_loadavg_percent);
EXPORT_SYMBOL(get_cpu_temperature_millicelsius);
EXPORT_SYMBOL(get_run_queue_length);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Minh Nguyen");
MODULE_DESCRIPTION("CPUFreq log feature module for logging CPU data");