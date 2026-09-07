// SPDX-License-Identifier: LGPL-3.0
#include <cassert>
#include <chrono>
#include <memory>
#include <new>

#include <robotiq_driver_noros/default_serial.hpp>

#include "robif2b/functions/robotiq_ft_sensor.h"
#include "robif2b/types/robotiq_ft_sensor.h"
#include "robotiq_ft_sensor_comm.hpp"

// Empty reads in a row before the connection counts as gone: 0.1 s at the 10 ms poll.
static constexpr int kMissesBeforeReconnect = 10;

void robif2b_robotiq_ft_configure(struct robif2b_robotiq_ft_sensor_nbx *b) {
    assert(b);
    assert(b->success);
    assert(b->force_msr);
    assert(b->force_offset);
    assert(b->moment_msr);
    assert(b->moment_offset);

    *b->success = false;

    b->comm = new (std::nothrow) struct robif2b_robotiq_ft_sensor_comm;
    if (!b->comm) return;

    struct robif2b_robotiq_ft_sensor_comm * comm = b->comm;
    comm->consecutive_misses = 0;

    auto serial = std::make_unique<rd::DefaultSerial>();
    serial->set_port(b->serial.port);
    serial->set_baudrate(b->serial.baudrate);
    serial->set_timeout(std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::duration<double, std::milli>(b->serial.timeout_ms)));

    comm->sensor = std::make_unique<rd::FTSensor>(std::move(serial));
    comm->sensor->set_slave_address(b->serial.slave_address);

    if (!comm->sensor->connect()) return;

    // A sensor left streaming by an earlier run answers nothing else until stopped.
    if (!comm->sensor->stop_stream()) return;
    if (!comm->sensor->start_stream()) return;

    // Set success here to allow overwrite in update()
    *b->success = true;

    // Read first measurements
    robif2b_robotiq_ft_update(b);
}

void robif2b_robotiq_ft_shutdown(struct robif2b_robotiq_ft_sensor_nbx *b) {
    assert(b);
    assert(b->success);

    if (b->comm) {
        b->comm->sensor->disconnect();
        delete b->comm;
        b->comm = nullptr;
    }

    *b->success = true;
}

void robif2b_robotiq_ft_update(struct robif2b_robotiq_ft_sensor_nbx *b) {
    assert(b);
    assert(b->comm);
    assert(b->success);
    assert(b->force_msr);
    assert(b->force_offset);
    assert(b->moment_msr);
    assert(b->moment_offset);

    struct robif2b_robotiq_ft_sensor_comm * comm = b->comm;

    const auto reading = comm->sensor->read();
    if (!reading) {
        // A re-enumerated converter never comes back on its own: reopen the port and restart.
        if (++comm->consecutive_misses >= kMissesBeforeReconnect) {
            comm->consecutive_misses = 0;
            comm->sensor->reconnect();
        }
        *b->success = false;
        return;
    }
    comm->consecutive_misses = 0;

    for (int i = 0; i < 3; i++) {
        b->force_msr[i] = reading->force[i] - b->force_offset[i];
        b->moment_msr[i] = reading->torque[i] - b->moment_offset[i];
    }

    *b->success = true;
}
