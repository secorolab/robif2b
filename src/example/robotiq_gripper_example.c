// SPDX-License-Identifier: LGPL-3.0
#include <stdio.h>
#include <unistd.h>
#include "robif2b/functions/robotiq_gripper.h"
#include "robif2b/types/robotiq_gripper.h"

#define MICRO_IN_MILI 1000

const char* err_code_to_str(enum robif2b_robotiq_gripper_error error) {
    switch (error) {
        case ROBIF2B_ROBOTIQ_NO_ERROR:
            return "no error";
        case ROBIF2B_ROBOTIQ_ERR_MEMORY:
            return "memory";
        case ROBIF2B_ROBOTIQ_ERR_CONNECTION:
            return "connection";
        case ROBIF2B_ROBOTIQ_ERR_ACTIVATION:
            return "activation";
        case ROBIF2B_ROBOTIQ_ERR_DEACTIVATION:
            return "deactivation";
        case ROBIF2B_ROBOTIQ_ERR_READ:
            return "read";
        case ROBIF2B_ROBOTIQ_ERR_WRITE:
            return "write";
        default:
            return "unknown error";
    }
}

int main(int argc, char **argv)
{
    bool success = false;
    bool is_gripper_moving = false;
    uint8_t position_msr = 0;
    uint8_t position_cmd = 0;
    uint8_t speed_cmd = 0xFF;
    uint8_t force_cmd = 0xFF;
    enum robif2b_robotiq_gripper_obj_status obj_status = ROBIF2B_ROBOTIQ_OBJ_UNKNOWN;
    enum robif2b_robotiq_gripper_status gripper_status = ROBIF2B_ROBOTIQ_UNKNOWN;
    enum robif2b_robotiq_gripper_error error = ROBIF2B_ROBOTIQ_NO_ERROR;

    struct robif2b_robotiq_gripper_nbx gripper = {
        .conf.port = "/dev/ttyUSB0",
        .conf.baudrate = 115200,
        .conf.timeout_ms = 1,
        .conf.slave_address = 0x09,
        .position_msr = &position_msr,
        .is_gripper_moving = &is_gripper_moving,
        .obj_detection_status = &obj_status,
        .gripper_status = &gripper_status,
        .position_cmd = &position_cmd,
        .speed_cmd = &speed_cmd,
        .force_cmd = &force_cmd,
        .error = &error,
        .success = &success,
    };

    robif2b_robotiq_gripper_configure(&gripper);
    if (!success) {
        printf("Configuration failed, error: %s\n", err_code_to_str(error));
        goto shutdown;
    }

    printf("Closing the gripper...\n");
    position_cmd = 0XFF;
    robif2b_robotiq_gripper_update(&gripper);
    if (!success) {
        printf("Closing failed, error: %s\n", err_code_to_str(error));
        goto shutdown;
    }
    while (is_gripper_moving) {
        robif2b_robotiq_gripper_update(&gripper);
        printf("Closing, gripper position: %d\n", position_msr);
        if (!success) {
            printf("Reading data failed, error: %s\n", err_code_to_str(error));
            goto shutdown;
        }
        usleep(100 * MICRO_IN_MILI);
    }

    printf("Openning the gripper at slower speed...\n");
    speed_cmd = 0x0F;
    position_cmd = 0X00;
    robif2b_robotiq_gripper_update(&gripper);
    if (!success) {
        printf("Opening failed, error: %s\n", err_code_to_str(error));
        goto shutdown;
    }
    while (is_gripper_moving) {
        robif2b_robotiq_gripper_update(&gripper);
        printf("Opening, gripper position: %d\n", position_msr);
        if (!success) {
            printf("Reading data failed, error: %s\n", err_code_to_str(error));
            goto shutdown;
        }
        usleep(100 * MICRO_IN_MILI);
    }

shutdown:
    printf("Shutting down...\n");
    robif2b_robotiq_gripper_shutdown(&gripper);
    if (!success) {
        printf("Shutdown failed, error: %s\n", err_code_to_str(error));
        return 1;
    }

    return 0;
}
