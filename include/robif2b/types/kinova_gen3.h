// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_TYPES_KINOVA_GEN3_H
#define ROBIF2B_TYPES_KINOVA_GEN3_H

#include <stdbool.h>
#include <stdint.h>

#include <robif2b/types/control_mode.h>


#ifdef __cplusplus
extern "C" {
#endif

#define ROBIF2B_KINOVA_GEN3_NR_JOINTS 7

enum robif2b_kinova_servoing_mode
{
    ROBIF2B_KINOVA_SERVOING_LOW_LEVEL = 0,
    ROBIF2B_KINOVA_SERVOING_SINGLE_LEVEL = 1
};

enum robif2b_kinova_cartesian_reference_frame
{
    ROBIF2B_KINOVA_CARTESIAN_REFERENCE_FRAME_BASE = 0,
    ROBIF2B_KINOVA_CARTESIAN_REFERENCE_FRAME_TOOL = 1,
    ROBIF2B_KINOVA_CARTESIAN_REFERENCE_FRAME_MIXED = 2
};


struct robif2b_kinova_gen3_config
{
    const char *ip_address;
    int port;
    int port_real_time;
    const char *user;
    const char *password;
    int session_timeout;        // [ms]
    int connection_timeout;     // [ms]
    bool use_gripper;           // robotiq gripper
};

struct robif2b_kionva_gen3_cart_cmd
{
    float *twist;       // [vx, vy, vz, wx, wy, wz]
    float *wrench;      // [N, N, N, Nm, Nm, Nm]
    uint32_t duration;  // [s]
    enum robif2b_kinova_cartesian_reference_frame reference_frame;
};

struct robif2b_kinova_gen3_comm;


struct robif2b_kinova_gen3_nbx
{
    // Configuration
    struct robif2b_kinova_gen3_config conf;
    // Ports
    const double *cycle_time;               // [s]
    enum robif2b_kinova_servoing_mode *servoing_mode;
    enum robif2b_ctrl_mode *ctrl_mode;
    double *jnt_pos_msr;                    // [rad]
    double *jnt_vel_msr;                    // [rad/s]
    double *jnt_trq_msr;                    // [Nm]
    double *act_cur_msr;                    // [A]
    const double *jnt_pos_cmd;              // [rad]
    const double *jnt_vel_cmd;              // [rad/s]
    const double *jnt_trq_cmd;              // [Nm]
    const double *act_cur_cmd;              // [A]
    double *imu_ang_vel_msr;                // XYZ [rad/s]
    double *imu_lin_acc_msr;                // XYZ [m/s^2]
    float *gripper_pos_msr;                  // [%]
    float *gripper_vel_msr;                  // [%]
    float *gripper_cur_msr;                  // [%]
    float *gripper_pos_cmd;                  // [%]
    float *gripper_vel_cmd;                  // [%]
    float *gripper_frc_cmd;                  // [%]
    struct robif2b_kionva_gen3_cart_cmd cartesian_cmd;
    bool *success;
    // Internal state
    struct robif2b_kinova_gen3_comm *comm;
    enum robif2b_ctrl_mode ctrl_mode_prev;
};

#ifdef __cplusplus
}
#endif

#endif
