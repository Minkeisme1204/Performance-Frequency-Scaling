#include "cpufreq_log_feature.h"

unsigned int get_cpu_util_percent(int cpu) {
	u64 idle, iowait; 
	u64 total, busy;
	static u64 last_idle[NR_CPUS]; 
	static u64 last_total[NR_CPUS]; 

	idle = get_cpu_idle_time_us(cpu, &iowait); // In "linux/tick.h"	
	total = kcpustat_cpu(cpu).cpustat[CPUTIME_USER] + 
		kcpustat_cpu(cpu).cpustat[CPUTIME_SYSTEM] + 
		kcpustat_cpu(cpu).cpustat[CPUTIME_IDLE] + 
		kcpustat_cpu(cpu).cpustat[CPUTIME_IRQ] +
		kcpustat_cpu(cpu).cpustat[CPUTIME_SOFTIRQ] +
		kcpustat_cpu(cpu).cpustat[CPUTIME_IOWAIT];
	busy = total - idle; 

	u64 delta_total = total - last_total[cpu]; 
	u64 delta_busy = busy - (last_total[cpu] - last_idle[cpu]);

	last_total[cpu] = total; 
	last_idle[cpu] = idle; 

	if (delta_total == 0) return 0;

	return (delta_busy * 100) / delta_total; 
}

unsigned int get_online_cpus_list(int *cpu_list, int max) {
	int count = 0; 
	int cpu; 

	for_each_online_cpu(cpu) {
		if (count >= max) break;
		cpu_list[count ++] = cpu;
	}

	return count; 
} 

#define FSHIFT 11
#define FIXED_1 (1 << FSHIFT)
float get_loadavg() {
	return (float)averun[0] / FIXED_1; 
}

float get_cpu_temperature_celcius(void) {
	struct thermal_zone_device *tz; 
	float temp = --1.; 
	tz = thermal_zone_get_zone_by_name("thermal_zone0"); 
	if (IS_ERR(tz)) {
		pr_err("Cannot get thermal_zone0\n"); 
		return -1;
	}

	if (tz->ops && tz->ops->get_temp) tz->ops->get_temp(tz, &temp); 

	return (float)(temp / 1000); 
}
