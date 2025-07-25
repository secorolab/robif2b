// SPDX-License-Identifier: LGPL-3.0
#include "robif2b/types/kinova_gen3.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include <robif2b/functions/kinova_gen3.h>


int main(int argc, char **argv)
{
    bool success = false;
    double cycle_time = 0.001;
    enum robif2b_ctrl_mode ctrl_mode = ROBIF2B_CTRL_MODE_FORCE;
    double pos_msr[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double vel_msr[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double eff_msr[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double cur_msr[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double imu_ang_vel_msr[] = { 0.0, 0.0, 0.0 };
    double imu_lin_acc_msr[] = { 0.0, 0.0, 0.0 };
    float wrench_cmd[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    
    struct robif2b_kionva_gen3_cart_cmd cart_cmd = {
        .reference_frame = ROBIF2B_KINOVA_CART_REF_FRAME_TOOL,
        .wrench          = wrench_cmd,
    };

    struct robif2b_kinova_gen3_hl_nbx rob = {
        // Configuration
        .conf.ip_address         = "192.168.1.10",
        .conf.port               = 10000,
        .conf.user               = "admin",
        .conf.password           = "admin",
        .conf.session_timeout    = 60000,
        .conf.connection_timeout = 2000,

        // Connections
        .ctrl_mode = &ctrl_mode,
        .jnt_pos_msr = pos_msr,
        .jnt_vel_msr = vel_msr,
        .jnt_trq_msr = eff_msr,
        .act_cur_msr = cur_msr,
        .imu_ang_vel_msr = imu_ang_vel_msr,
        .imu_lin_acc_msr = imu_lin_acc_msr,
        .cart_cmd = &cart_cmd,
        .success  = &success
    };


    robif2b_kinova_gen3_hl_configure(&rob);
    if (!success) {
        printf("Error during gen3_configure\n");
        goto shutdown;
    }

    robif2b_kinova_gen3_hl_recover(&rob);
    if (!success) {
        printf("Error during gen3_recover\n");
        goto shutdown;
    }

    printf("Starting\n");
    robif2b_kinova_gen3_hl_start(&rob);
    if (!success) {
        printf("Error during gen3_start\n");
        goto stop;
    }


    for (int i = 0; i < 3000; i++) {
        robif2b_kinova_gen3_hl_update(&rob);
        if (!success) {
            printf("Error during gen3_update\n");
            goto stop;
        }

        usleep(1000);
    }

stop:
    robif2b_kinova_gen3_hl_stop(&rob);
    printf("Stopped\n");

shutdown:
    robif2b_kinova_gen3_hl_shutdown(&rob);
    if (!success) {
        printf("Error during gen3_shutdown\n");
        return 1;
    }

    return 0;
}
