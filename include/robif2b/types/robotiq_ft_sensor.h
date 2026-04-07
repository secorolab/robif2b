// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_TYPES_ROBOTIQ_FT_SENSOR_H
#define ROBIF2B_TYPES_ROBOTIQ_FT_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "robif2b/types/serial.h"

struct robif2b_robotiq_ft_sensor_nbx {
    // Configuration
    struct robif2b_serial_config serial;    // FT sensor driver will assume `/dev/` prefix
                                            // on port field
    // Ports
    float *force_msr;                       // [N] XYZ forces
    float *force_offset;                    // [N] XYZ forces
    float *moment_msr;                      // [Nm] XYZ moments
    float *moment_offset;                   // [Nm] XYZ moments
    bool *success;
};

#ifdef __cplusplus
}
#endif

#endif
