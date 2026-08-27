// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_ROBOTIQ_FT_SENSOR_COMM_HPP
#define ROBIF2B_ROBOTIQ_FT_SENSOR_COMM_HPP

#include <memory>

#include <robotiq_driver_noros/ft_sensor.hpp>

namespace rd = robotiq_driver;

struct robif2b_robotiq_ft_sensor_comm {
    std::unique_ptr<rd::FTSensor> sensor;
    int consecutive_misses;
};

#endif
