// SPDX-License-Identifier: LGPL-3.0
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

#include <robif2b/functions/kinova_gen3.h>


volatile sig_atomic_t stop_signal_received = 0;

void handle_stop_signal(int signal)
{
    stop_signal_received = 1;
}


int main(int argc, char **argv)
{
    signal(SIGINT, handle_stop_signal);

    bool success = false;
    enum robif2b_hl_ctrl_mode ctrl_mode = ROBIF2B_HL_CTRL_MODE_WRENCH;
    enum robif2b_kinova_cart_ref_frame ref_frame = ROBIF2B_KINOVA_CART_REF_FRAME_TOOL;
    enum robif2b_kinova_cart_wrench_mode wrench_mode = ROBIF2B_KINOVA_CART_WRENCH_MODE_NORMAL;
    double pos_msr[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double vel_msr[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double eff_msr[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double cur_msr[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double imu_ang_vel_msr[] = { 0.0, 0.0, 0.0 };
    double imu_lin_acc_msr[] = { 0.0, 0.0, 0.0 };
    double wrench_cmd[] = { 0., 0., 0., 0., 0., 5. };
   
    struct robif2b_kinova_gen3_hl_nbx rob = {
        // Configuration
        .conf.ip_address         = "192.168.1.10",
        .conf.port               = 10000,
        .conf.user               = "admin",
        .conf.password           = "admin",
        .conf.session_timeout    = 60000,
        .conf.connection_timeout = 2000,

        // Connections
        .ctrl_mode       = &ctrl_mode,
        .reference_frame = &ref_frame,
        .wrench_mode     = &wrench_mode,
        .jnt_pos_msr     = pos_msr,
        .jnt_vel_msr     = vel_msr,
        .jnt_trq_msr     = eff_msr,
        .act_cur_msr     = cur_msr,
        .imu_ang_vel_msr = imu_ang_vel_msr,
        .imu_lin_acc_msr = imu_lin_acc_msr,
        .twist_cmd       = NULL,
        .wrench_cmd      = wrench_cmd,
        .success         = &success
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

    for (;;) {
        robif2b_kinova_gen3_hl_update(&rob);
        if (!success || stop_signal_received) {
            printf("Error during gen3_update or stop signal received\n");
            goto stop;
        }

        usleep(100);
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
    printf("Shutdown completed\n");

    return 0;
}
