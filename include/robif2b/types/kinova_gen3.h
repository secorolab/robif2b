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


struct robif2b_kinova_gen3_config
{
    const char *ip_address;
    int port;
    int port_real_time;
    const char *user;
    const char *password;
    int session_timeout;        // [ms]
    int connection_timeout;     // [ms]
};


struct robif2b_kinova_gen3_comm;


// low-level servoing
struct robif2b_kinova_gen3_nbx
{
    // Configuration
    struct robif2b_kinova_gen3_config conf;
    // Ports
    const double *cycle_time;               // [s]
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
    bool *success;
    // Internal state
    struct robif2b_kinova_gen3_comm *comm;
    enum robif2b_ctrl_mode ctrl_mode_prev;
};


// high-level servoing
enum robif2b_kinova_cart_ref_frame
{
    ROBIF2B_KINOVA_CART_REF_FRAME_BASE = 0,
    ROBIF2B_KINOVA_CART_REF_FRAME_TOOL = 1,
    ROBIF2B_KINOVA_CART_REF_FRAME_MIXED = 2
};


enum robif2b_kinova_cart_wrench_mode
{
    // 0 restricts motion in a direction, non-zero allows compliance
    ROBIF2B_KINOVA_CART_WRENCH_MODE_RESTRICTED = 0,
    // allows compliance in all directions
    ROBIF2B_KINOVA_CART_WRENCH_MODE_NORMAL
};


struct robif2b_kinova_gen3_hl_nbx
{
    // configuration
    struct robif2b_kinova_gen3_config conf;
    // ports
    enum robif2b_hl_ctrl_mode *ctrl_mode;
    enum robif2b_kinova_cart_ref_frame   *reference_frame;
    enum robif2b_kinova_cart_wrench_mode *wrench_mode;
    double *jnt_pos_msr;                    // [rad]
    double *jnt_vel_msr;                    // [rad/s]
    double *jnt_trq_msr;                    // [Nm]
    double *act_cur_msr;                    // [A]
    double *imu_ang_vel_msr;                // XYZ [rad/s]
    double *imu_lin_acc_msr;                // XYZ [m/s^2]

    const double *twist_cmd;    // [vx, vy, vz, wx, wy, wz]
    const double *wrench_cmd;   // [N, N, N, Nm, Nm, Nm]
    bool *success;
    // Internal state
    struct robif2b_kinova_gen3_comm *comm;
};


// robotiq gripper
struct robif2b_kg3_robotiq_gripper_nbx
{
    float *gripper_pos_msr;                  // [%]
    float *gripper_vel_msr;                  // [%]
    float *gripper_cur_msr;                  // [%]
    float *gripper_pos_cmd;                  // [%]
    float *gripper_vel_cmd;                  // [%]
    float *gripper_frc_cmd;                  // [%]    
    bool *success;
    // Internal state
    struct robif2b_kinova_gen3_comm *comm;
};

#ifdef __cplusplus
}
#endif

#endif
