// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_TYPES_ROBOTIQ_GRIPPER_H
#define ROBIF2B_TYPES_ROBOTIQ_GRIPPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

enum robif2b_robotiq_gripper_obj_status {
    ROBIF2B_ROBOTIQ_OBJ_MOVING = 0,
    ROBIF2B_ROBOTIQ_OBJ_DETECTED_OPENING,
    ROBIF2B_ROBOTIQ_OBJ_DETECTED_CLOSING,
    ROBIF2B_ROBOTIQ_OBJ_AT_REQUESTED_POSITION,
    ROBIF2B_ROBOTIQ_OBJ_UNKNOWN
};

enum robif2b_robotiq_gripper_status {
    ROBIF2B_ROBOTIQ_RESET = 0,
    ROBIF2B_ROBOTIQ_IN_PROGRESS,
    ROBIF2B_ROBOTIQ_COMPLETED,
    ROBIF2B_ROBOTIQ_UNKNOWN
};

enum robif2b_robotiq_gripper_error {
    ROBIF2B_ROBOTIQ_NO_ERROR = 0,
    ROBIF2B_ROBOTIQ_ERR_MEMORY,
    ROBIF2B_ROBOTIQ_ERR_CONNECTION,
    ROBIF2B_ROBOTIQ_ERR_DEACTIVATION,
    ROBIF2B_ROBOTIQ_ERR_ACTIVATION,
    ROBIF2B_ROBOTIQ_ERR_READ,
    ROBIF2B_ROBOTIQ_ERR_WRITE,
};

struct robif2b_robotiq_gripper_config {
    const char * port;
    uint32_t baudrate;
    double timeout_ms;      // timeout in milliseconds
    uint8_t slave_address;
};

struct robif2b_robotiq_gripper_comm;

struct robif2b_robotiq_gripper_nbx {
    // Configuration
    struct robif2b_robotiq_gripper_config conf;
    // Ports
    uint8_t *position_msr;
    bool *is_gripper_moving;
    // Object detection returned from gripper via serial,
    // doesn't work well for smaller objects
    enum robif2b_robotiq_gripper_obj_status *obj_detection_status;
    enum robif2b_robotiq_gripper_status *gripper_status;
    uint8_t *position_cmd;
    uint8_t *speed_cmd;
    uint8_t *force_cmd;
    enum robif2b_robotiq_gripper_error *error;
    bool *success;
    // Internal state
    struct robif2b_robotiq_gripper_comm *comm;
};

#ifdef __cplusplus
}
#endif

#endif
