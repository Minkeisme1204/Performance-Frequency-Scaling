#include "cpu_info_logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define CSV_FILE "../logged_data/data.csv"
#define SAMPLE 3600

void write_header(FILE *fp) {
    fprintf(fp, "timestamp,cpu_util,online_cores,load_avg,temp,frequency\n");
}

int main() {
    FILE *fp = fopen(CSV_FILE, "w");
    if (!fp) {
        perror("Unable to open file");
        return 1;
    }

    write_header(fp);

    int sample = 0; 

    while (sample < 3600) {
        time_t now = time(NULL);
        char *timestamp = ctime(&now);
        timestamp[strcspn(timestamp, "\n")] = '\0';  // remove newline

        float util = get_cpu_utilization();
        int freq = get_cpu_freq_mhz();
        int cores = count_online_cores();
        float load = get_load_average();
        float temp = get_cpu_temperature();

        fprintf(fp, "%s,%.2f,%d,%.2f,%.2f,%d\n", timestamp, util, cores, load, temp, freq);
        fflush(fp);

        sample++; 
        sleep(1);  // log every second
    }

    fclose(fp);
    return 0;
}
