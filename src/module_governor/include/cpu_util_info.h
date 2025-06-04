#ifndef CPU_UTIL_INFO_H
#define CPU_UTIL_INFO_H

#include <linux/types.h>

struct cpu_stat_snapshot {
    u64 user; 
    u64 nice; 
    u64 system; 
    u64 idle; 
};

// Get the snapshot current CPU time
void get_cpu_snapshot(struct cpu_stat_snapshot *s, int cpu);

// Compute % of util between 2 snapshots
unsigned int compute_cpu_util_percent(const struct cpu_stat_snapshot *old,
                                      const struct cpu_stat_snapshot *new);

#endif
