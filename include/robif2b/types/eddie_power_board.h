// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_TYPES_EDDIE_POWER_BOARD_H
#define ROBIF2B_TYPES_EDDIE_POWER_BOARD_H

#include <stdbool.h>
#include <robif2b/types/eddie_power_board_api.h>

#ifdef __cplusplus
extern "C" {
#endif

struct robif2b_eddie_power_board
{
    /**
     * Process data objects that represent the measured power-board state.
     */
    const struct robif2b_eddie_power_board_api_msr_pdo *msr_pdo;

    /**
     * Process data objects that represent the power-board commands.
     */
    struct robif2b_eddie_power_board_api_cmd_pdo *cmd_pdo;

    /**
     * Time stamp of the last received EtherCAT frame in [ms].
     */
    uint64_t *time_stamp;

    /**
     * Status in bits.
     */
    uint16_t *status;

    /**
     * Battery's voltage in [Volt].
     */
    double *voltage_msr;

    /**
     * Current pulled from battery in [Ampere].
     */
    double *current_msr;

    /**
     * Overall power consumption in [Watt].
     */
    double *power_msr;
};

#ifdef __cplusplus
}
#endif

#endif
