// SPDX-License-Identifier: LGPL-3.0
#include <stdio.h>
#include <unistd.h>
#include "robif2b/functions/robotiq_ft_sensor.h"
#include "robif2b/types/robotiq_ft_sensor.h"
#include "time_util.h"

#define NUM_READS 50
#define NUM_RETRY 5

int main(int argc, char **argv)
{
    bool success = false;
    float force_msr[3] = {0};
    float force_offset[3] = {0};
    float moment_msr[3] = {0};
    float moment_offset[3] = {0};
    struct timespec start_time;
    struct timespec end_time;

    struct robif2b_robotiq_ft_sensor_nbx ft_sensor = {
        .serial.port = "ttyUSB0",
        .serial.baudrate = 19200,       // unused
        .serial.timeout_ms = 1,         // unused
        .serial.slave_address = 0x09,   // unused
        .force_msr = force_msr,
        .force_offset = force_offset,
        .moment_msr = moment_msr,
        .moment_offset = moment_offset,
        .success = &success,
    };

    int retry = 0;
    while (retry < NUM_RETRY) {
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        robif2b_robotiq_ft_configure(&ft_sensor);
        clock_gettime(CLOCK_MONOTONIC, &end_time);

        if (success) break;
        printf("Configuration failed after %d try(s)\n", retry + 1);
        retry++;
    }
    if (!success) {
        printf("Configuration failed\n");
        goto shutdown;
    }
    printf("Configuration time: %.5f msec\n",
           (double) (timespec_to_usec(&end_time) - timespec_to_usec(&start_time)) / USEC_IN_MSEC);

    int cycle_count = 0;
    double cycle_sum_us = 0.0;
    for (int i = 0; i < NUM_READS; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        robif2b_robotiq_ft_update(&ft_sensor);
        clock_gettime(CLOCK_MONOTONIC, &end_time);

        if (!success) {
            printf("Reading data failed\n");
            goto shutdown;
        }

        cycle_count += 1;
        cycle_sum_us += timespec_to_usec(&end_time) - timespec_to_usec(&start_time);

        printf("F = (%9.5f, %9.5f, %9.5f), M = (%9.5f, %9.5f, %9.5f)\n",
               force_msr[0], force_msr[1], force_msr[2],
               moment_msr[0], moment_msr[1], moment_msr[2]);
        usleep(10 * USEC_IN_MSEC);
    }

    printf("Setting offsets to last measurements.\n");
    for (int i = 0; i < 3; i++) {
        force_offset[i] = force_msr[i];
        moment_offset[i] = moment_msr[i];
    }
    for (int i = 0; i < NUM_READS; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        robif2b_robotiq_ft_update(&ft_sensor);
        clock_gettime(CLOCK_MONOTONIC, &end_time);

        if (!success) {
            printf("Reading data failed\n");
            goto shutdown;
        }

        cycle_count += 1;
        cycle_sum_us += timespec_to_usec(&end_time) - timespec_to_usec(&start_time);

        printf("F = (%9.5f, %9.5f, %9.5f), M = (%9.5f, %9.5f, %9.5f)\n",
               force_msr[0], force_msr[1], force_msr[2],
               moment_msr[0], moment_msr[1], moment_msr[2]);
        usleep(10 * USEC_IN_MSEC);
    }

    printf("Average update time (%d cycles): %.5f msec\n",
           cycle_count, cycle_sum_us / cycle_count / USEC_IN_MSEC);

shutdown:
    printf("Shutting down...\n");
    robif2b_robotiq_ft_shutdown(&ft_sensor);
    if (!success) {
        printf("Shutdown failed, error\n");
        return 1;
    }

    return 0;
}
