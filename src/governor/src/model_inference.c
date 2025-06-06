#include "model_inference.h"

int model_inference(int cpu_util, int avg_load, int temperature, int runq) {
    int res = 0; 
    if (avg_load <= 12) {
        res = FREQ_TABLE[0];
    }
    else {
        if (temperature < 60875) {
            if (avg_load <= 25) {
                if (runq <= 4) {
                    res = FREQ_TABLE[1]; 
                }
                else res = FREQ_TABLE[0]; // 1600 
            }
            else {
                if (runq <= 4) {
                    if (avg_load <= 49) {
                        res = FREQ_TABLE[2]; // 1700
                    }
                    else res = FREQ_TABLE[9]; // 2400
                }
                else {
                    if (avg_load <= 97) {
                        res = FREQ_TABLE[1]; // 1600
                    }
                    else res = FREQ_TABLE[2]; // 1700
                }
            }
        }
        else {
            if (runq <= 4) {
                if (cpu_util <= 2) {
                    if (avg_load <= 118) {
                        res = FREQ_TABLE[7]; // 2200
                    } else {
                        res = FREQ_TABLE[9]; // 2400
                    }
                }
                else res = FREQ_TABLE[9]; // 2400
            }
            else {
                if (temperature <= 66925) {
                    if (temperature <= 64175) {
                        res = FREQ_TABLE[4]; // 1900
                    }
                    else res = FREQ_TABLE[7]; // 2200
                }
                else {
                    if(cpu_util <= 0.5) {
                        res = FREQ_TABLE[8]; // 2300
                    }
                    else {
                        res = FREQ_TABLE[9]; // 2400
                    }
                }
            }
        }
    }
    return res; 
}

EXPORT_SYMBOL(model_inference);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nguyen Cao Minh <minhnguyencao1204@gmail.com>");
MODULE_DESCRIPTION("CPUFreq log feature module for logging CPU data");