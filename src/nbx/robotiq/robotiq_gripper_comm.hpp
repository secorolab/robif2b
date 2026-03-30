// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_ROBOTIQ_GRIPPER_COMM_HPP
#define ROBIF2B_ROBOTIQ_GRIPPER_COMM_HPP

#include <robotiq_driver_noros/default_serial.hpp>
#include <robotiq_driver_noros/default_driver.hpp>

namespace rd = robotiq_driver;

struct robif2b_robotiq_gripper_comm {
    std::unique_ptr<rd::DefaultSerial> serial;
    std::unique_ptr<rd::DefaultDriver> driver;
};

#endif
