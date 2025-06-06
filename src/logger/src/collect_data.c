#include "cpu_info_logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define CSV_FILE "../logged_data/data_modelbased.csv"
#define SAMPLE 3600

void write_header(FILE *fp) {
    fprintf(fp, "cpu_util,load_avg,temp,run_queue_length,frequency\n");
}

int main() {
    FILE *fp = fopen(CSV_FILE, "a");
    if (!fp) {
        perror("Unable to open file");
        return 1;
    }

    write_header(fp);
    
    int sample = 0; 

    while (sample < 100) {
        time_t now = time(NULL);
        char *timestamp = ctime(&now);
        timestamp[strcspn(timestamp, "\n")] = '\0';  // remove newline

        static int prev_freq;

        int util;
        int freq;
        int load;
        int temp;
        int runq;

        /* Block for collecting training
        while (1) {
            util = get_cpu_utilization();
            freq = get_cpu_freq_mhz();
            load = get_load_average();
            temp = get_cpu_temperature_millicelsius();
            runq = get_run_queue_length();

            if (freq == prev_freq) break; 
            
            prev_freq = freq;
        }
        */

        util = get_cpu_utilization();
        freq = get_cpu_freq_mhz();
        load = get_load_average();
        temp = get_cpu_temperature_millicelsius();
        runq = get_run_queue_length();

        printf("CPU Util: %d | Freq: %d MHz | Load Avg: %d%% | Temp: %d milli°C | Run Queue Length: %d\n",
            util, freq, load, temp, runq
        );
        fprintf(fp, "%d,%d,%d,%d,%d\n", util, load, temp, runq, freq);
        fflush(fp);

        sample++; 
        usleep(1000000);  // log every second
    }

    fclose(fp);
    return 0;
}
