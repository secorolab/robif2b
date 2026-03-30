// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_FUNCTIONS_ROBOTIQ_GRIPPER_H
#define ROBIF2B_FUNCTIONS_ROBOTIQ_GRIPPER_H

#include "robif2b/types/robotiq_gripper.h"

#ifdef __cplusplus
extern "C" {
#endif

void robif2b_robotiq_gripper_configure(struct robif2b_robotiq_gripper_nbx *b);  // Blocking
void robif2b_robotiq_gripper_shutdown(struct robif2b_robotiq_gripper_nbx *b);
void robif2b_robotiq_gripper_update(struct robif2b_robotiq_gripper_nbx *b);

#ifdef __cplusplus
}
#endif

#endif
