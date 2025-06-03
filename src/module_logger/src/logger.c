#include "cpu_info_logger.h"

int main() {
    while (1) {
        float util = get_cpu_utilization();
        int freq = get_cpu_freq_mhz();
        int cores = count_online_cores();
        float load = get_load_average();
        float temp = get_cpu_temperature();

        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        printf("[%02d:%02d:%02d] CPU Util: %.2f%% | Freq: %d MHz | Cores: %d | Load Avg: %.2f | Temp: %.2f°C\n",
            t->tm_hour, t->tm_min, t->tm_sec,
            util, freq, cores, load, temp
        );

        sleep(1);
    }
    return 0;
}
