#ifndef CPUFREQ_SAMPLE
#define CPUFREQ_SAMPLE
#define MAX_CPU 4

#include <linux/cpufreq.h>
#include <linux/kernel_stat.h>
#include <linux/jiffies.h>
#include <linux/sched.h>
#include <linux/tick.h>
#include <linux/cpu.h>
#include <linux/smp.h>
#include <linux/thermal.h>
#include <linux/sched/loadavg.h>
#include <linux/sched//topology.h>
#include <linux/sched/signal.h>

int get_cpu_util(void);
int get_online_cpus_list(int *cpu_list, int max);
int get_loadavg_percent(void);
int get_run_queue_length(int cpu);
int get_cpu_temperature_millicelsius(void);
#endif
