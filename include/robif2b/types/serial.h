// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_TYPES_SERIAL_H
#define ROBIF2B_TYPES_SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

struct robif2b_serial_config {
    const char * port;
    uint32_t baudrate;
    double timeout_ms;      // timeout in milliseconds
    uint8_t slave_address;
};

#ifdef __cplusplus
}
#endif

#endif
