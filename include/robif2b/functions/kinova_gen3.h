// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_FUNCTIONS_KINOVA_GEN3_H
#define ROBIF2B_FUNCTIONS_KINOVA_GEN3_H

#include <robif2b/types/kinova_gen3.h>


#ifdef __cplusplus
extern "C" {
#endif

// low-level servoing
void robif2b_kinova_gen3_configure(struct robif2b_kinova_gen3_nbx *b);

void robif2b_kinova_gen3_shutdown(struct robif2b_kinova_gen3_nbx *b);

void robif2b_kinova_gen3_start(struct robif2b_kinova_gen3_nbx *b);

void robif2b_kinova_gen3_stop(struct robif2b_kinova_gen3_nbx *b);

void robif2b_kinova_gen3_recover(struct robif2b_kinova_gen3_nbx *b);

void robif2b_kinova_gen3_update(struct robif2b_kinova_gen3_nbx *b);

void publish_measurement(struct robif2b_kinova_gen3_nbx *b);

// high-level servoing
void robif2b_kinova_gen3_hl_configure(struct robif2b_kinova_gen3_hl_nbx *b);

void robif2b_kinova_gen3_hl_shutdown(struct robif2b_kinova_gen3_hl_nbx *b);

void robif2b_kinova_gen3_hl_start(struct robif2b_kinova_gen3_hl_nbx *b);

void robif2b_kinova_gen3_hl_stop(struct robif2b_kinova_gen3_hl_nbx *b);

void robif2b_kinova_gen3_hl_recover(struct robif2b_kinova_gen3_hl_nbx *b);

void robif2b_kinova_gen3_hl_update(struct robif2b_kinova_gen3_hl_nbx *b);

void publish_hl_measurement(struct robif2b_kinova_gen3_hl_nbx *b);

// robotiq gripper
void robif2b_kg3_robotiq_gripper_configure(struct robif2b_kg3_robotiq_gripper_nbx *b, struct robif2b_kinova_gen3_nbx *g);

void robif2b_kg3_robotiq_gripper_start(struct robif2b_kg3_robotiq_gripper_nbx *b);

void robif2b_kg3_robotiq_gripper_stop(struct robif2b_kg3_robotiq_gripper_nbx *b);

void robif2b_kg3_robotiq_gripper_update(struct robif2b_kg3_robotiq_gripper_nbx *b);

void publish_gripper_measurement(struct robif2b_kg3_robotiq_gripper_nbx *b);

#ifdef __cplusplus
}
#endif

#endif
