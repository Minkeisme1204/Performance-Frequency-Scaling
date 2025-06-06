#ifndef MODEL 
#define MODEL

#include <linux/module.h>
#include <linux/kernel.h>

const int FREQ_TABLE[10] = {
    1500, 
    1600,
    1700, 
    1800, 
    1900, 
    2000, 
    2100, 
    2200, 
    2300, 
    2400
};

int model_inference(int cpu_util, int avg_load, int temperature, int runq);

#endif