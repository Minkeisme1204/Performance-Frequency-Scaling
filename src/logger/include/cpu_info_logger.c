#include "cpu_info_logger.h"

// Get CPU Utilization
int get_cpu_utilization() {
    static int prev_user = 0, prev_nice = 0, prev_system = 0;
    static int prev_idle = 0, prev_iowait = 0;
    static int prev_irq = 0, prev_softirq = 0, prev_steal = 0;

    int user, nice, system, idle;
    int iowait, irq, softirq, steal;

    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) return -1;

  
    fscanf(fp, "cpu  %d %d %d %d %d %d %d %d",
           &user, &nice, &system, &idle,
           &iowait, &irq, &softirq, &steal);
    fclose(fp);

    int prev_idle_all = prev_idle + prev_iowait;
    int idle_all = idle + iowait;

    int prev_non_idle = prev_user + prev_nice + prev_system + prev_irq + prev_softirq + prev_steal;
    int non_idle = user + nice + system + irq + softirq + steal;

    int prev_total = prev_idle_all + prev_non_idle;
    int total = idle_all + non_idle;

    int delta_total = total - prev_total; 
    int delta_idle = idle_all - prev_idle_all; 

    prev_user = user;
    prev_nice = nice;
    prev_system = system;
    prev_idle = idle;
    prev_iowait = iowait;
    prev_irq = irq;
    prev_softirq = softirq;
    prev_steal = steal;

    if (delta_total == 0) return 0.0f;
    return delta_total / (delta_total - delta_idle) ;
}

// Get current CPU runqueue length
int get_run_queue_length() {
    FILE *fp = fopen("/proc/loadavg", "r"); 
    if (!fp) {
        perror("Get run queue length failed"); 
        return -1; 
    }
    char buffer[128];
    if (!fgets(buffer, sizeof(buffer), fp)) {
        perror("Failed to read /proc/loadavg");
        fclose(fp);
        return -1;
    }
    fclose(fp);

    // Line format: "0.13 0.12 0.14 1/254 12345"
    // Needed infor in  "1/254" => 1

    int runq = 0;
    if (sscanf(buffer, "%*f %*f %*f %d", &runq) != 1) {
        fprintf(stderr, "Failed to parse run queue length\n");
        return -1;
    }

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
int get_load_average() {
    FILE *fp = fopen("/proc/loadavg", "r");
    if (!fp) return -1;
    float load;
    fscanf(fp, "%f", &load);
    fclose(fp);
    return load * 100;
}

// Read CPU temperature (if exists)
int get_cpu_temperature_millicelsius() {
    FILE *fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (!fp) return -1;
    int temp_millic;
    fscanf(fp, "%d", &temp_millic);
    fclose(fp);
    return temp_millic;
}

