#ifndef CPU_DATA_COLLECT_H
#define CPU_DATA_COLLECT_H

#include <linux/cpufreq.h>
#include <linux/kernel_stat.h>
#include <linux/sched/loadavg.h>
#include <linux/thermal.h>
#include <linux/cpu.h>
#include <linux/timekeeping.h>

struct cpu_sample {
    unsigned int cpu;
    unsigned long freq_mhz; // Output data Y for the model 
    unsigned long user_time; // time for userspace
    unsigned long system_time; // 
    unsigned long idle_time;
    unsigned long load_avg_1min;
    unsigned int online_cores;
    int temperature_millic;
};

/* Public API */
void collect_cpu_sample(struct cpu_sample *sample, int cpu);

#endif