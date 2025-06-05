#include "cpu_info_logger.h"

int main() {
    while (1) {
        int util = get_cpu_utilization();
        int freq = get_cpu_freq_mhz();
        int load = get_load_average();
        int temp = get_cpu_temperature_millicelsius();
        int runq = get_run_queue_length();

        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        printf("[%02d:%02d:%02d] CPU Util: %d | Freq: %d MHz | Load Avg: %d%% | Temp: %d milli°C | Run Queue Length: %d\n",
            t->tm_hour, t->tm_min, t->tm_sec,
            util, freq, load, temp, runq
        );

        usleep(1000000);
    }
    return 0;
}
