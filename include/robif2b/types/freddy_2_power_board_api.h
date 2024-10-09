// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_TYPES_FREDDY_2_POWER_BOARD_API_H
#define ROBIF2B_TYPES_FREDDY_2_POWER_BOARD_API_H

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

struct robif2b_freddy_2_power_board_api_msr_pdo {
    uint16_t status;      // Status bits
    uint64_t time_stamp;  // EtherCAT timestamp ms
    float current;        // Total current consumption
    float voltage;        // System Voltage
    float power;          // Total power consumption of the system
    float param1;         // Generic data, might be used for different purposes
    uint32_t param2;      // Generic data, might be used for different purposes
} __attribute__ ((__packed__));

#define COMMAND_DISABLE 0x00

struct robif2b_freddy_2_power_board_api_cmd_pdo {
    uint16_t shutdown;  // bit 15-8 reserved, bit 7 shutdown, bit 6-0 delay in seconds
    uint32_t command;
} __attribute__ ((__packed__));

#ifdef __cplusplus
}
#endif

#endif
