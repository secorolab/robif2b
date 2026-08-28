// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_TYPES_ROBOTIQ_FT_SENSOR_H
#define ROBIF2B_TYPES_ROBOTIQ_FT_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "robif2b/types/serial.h"

struct robif2b_robotiq_ft_sensor_comm;

struct robif2b_robotiq_ft_sensor_nbx {
    // Configuration
    struct robif2b_serial_config serial;    // Full path to the port, e.g. "/dev/ttyUSB0". The
                                            // timeout has to outlast a frame period: the sensor
                                            // streams at 100 Hz, so anything under 10 ms times
                                            // out on every read.
    // Ports
    float *force_msr;                       // [N] XYZ forces
    float *force_offset;                    // [N] XYZ forces
    float *moment_msr;                      // [Nm] XYZ moments
    float *moment_offset;                   // [Nm] XYZ moments
    bool *success;
    // Internal state
    struct robif2b_robotiq_ft_sensor_comm *comm;
};

#ifdef __cplusplus
}
#endif

#endif
