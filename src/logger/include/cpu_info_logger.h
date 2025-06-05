#ifndef LOGGER
#define LOGGER

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

#define MAX_CPUS 4

// Get CPU utilization by parsing /proc/stat
int get_cpu_utilization();

// Get current CPU frequency for core 0 (can extend to average of all cores)
int get_cpu_freq_mhz();

// Count number of online CPU cores
int count_online_cores();

// Read load average from /proc/loadavg
int get_load_average();

// Read CPU temperature (if exists)
int get_cpu_temperature_millicelsius();

// Get current CPU runqueue length
int get_run_queue_length();
#endif
