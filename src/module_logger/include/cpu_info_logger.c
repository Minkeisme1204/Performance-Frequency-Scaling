#include "cpu_info_logger.h"

float get_cpu_utilization() {
    static long prev_user=0, prev_nice=0, prev_system=0, prev_idle=0;
    long user, nice, system, idle;
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) return -1;

    fscanf(fp, "cpu  %ld %ld %ld %ld", &user, &nice, &system, &idle);
    fclose(fp);

    long total_prev = prev_user + prev_nice + prev_system + prev_idle;
    long total_now = user + nice + system + idle;

    long totald = total_now - total_prev;
    long idled = idle - prev_idle;

    prev_user = user;
    prev_nice = nice;
    prev_system = system;
    prev_idle = idle;

    if (totald == 0) return 0;
    return 100.0 * (totald - idled) / totald;
}

// Get current CPU runqueue length
int get_run_queue_length() {
    FILE *fp = fopen("/proc/loadavg", "r");
    if (!fp) return -1;
    int runq;
    fscanf(fp, "%*f %*f %*f %d", &runq);
    fclose(fp);
    return runq;
}

// Get current CPU frequency for core 0 (can extend to average of all cores)
int get_cpu_freq_mhz() {
    FILE *fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq", "r");
    if (!fp) return -1;
    int freq_khz;
    fscanf(fp, "%d", &freq_khz);
    fclose(fp);
    return freq_khz / 1000;
}

// Count number of online CPU cores
int count_online_cores() {
    FILE *fp = fopen("/sys/devices/system/cpu/online", "r");
    if (!fp) return -1;
    int start, end, count = 0;
    char buf[64];
    fscanf(fp, "%63s", buf);
    fclose(fp);

    if (strchr(buf, '-')) {
        sscanf(buf, "%d-%d", &start, &end);
        count = end - start + 1;
    } else {
        count = 1;
    }
    return count;
}

// Read load average from /proc/loadavg
float get_load_average() {
    FILE *fp = fopen("/proc/loadavg", "r");
    if (!fp) return -1;
    float load;
    fscanf(fp, "%f", &load);
    fclose(fp);
    return load;
}

// Read CPU temperature (if exists)
float get_cpu_temperature() {
    FILE *fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (!fp) return -1;
    int temp_millic;
    fscanf(fp, "%d", &temp_millic);
    fclose(fp);
    return temp_millic / 1000.0;
}
