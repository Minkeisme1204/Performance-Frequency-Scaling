#ifndef CPUFREQ_LOG
#define CPUFREQ_LOG

#include <linux/cpufreq.h>
#include <linux/kernel_stat.h>
#include <linux/jiffies.h>
#include <linux/sched.h>
#include <linux/tick.h>
#include <linux/cpu.h>
#include <linux/smp.h>

unsigned int get_cpu_util_percent(int cpu);
unsigned get_online_cpus_list(int *cpu_list, int max);

#endif
