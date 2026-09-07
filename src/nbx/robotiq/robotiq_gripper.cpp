#include <cassert>
#include <chrono>
#include <memory>
#include <robotiq_driver_noros/default_driver.hpp>
#include <robotiq_driver_noros/driver_exception.hpp>
#include <serial/serial.h>
#include "robif2b/types/robotiq_gripper.h"
#include "robif2b/functions/robotiq_gripper.h"
#include "robotiq_gripper_comm.hpp"

// The gripper reports its motor current in counts worth 10 mA each.
constexpr double kCurrentPerCount = 0.01;

constexpr enum robif2b_robotiq_gripper_obj_status
to_robif2b_obj_detection_status(rd::DefaultDriver::ObjectDetectionStatus status) {
    switch (status) {
        case rd::DefaultDriver::ObjectDetectionStatus::MOVING:
            return ROBIF2B_ROBOTIQ_OBJ_MOVING;
        case rd::DefaultDriver::ObjectDetectionStatus::OBJECT_DETECTED_OPENING:
            return ROBIF2B_ROBOTIQ_OBJ_DETECTED_OPENING;
        case rd::DefaultDriver::ObjectDetectionStatus::OBJECT_DETECTED_CLOSING:
            return ROBIF2B_ROBOTIQ_OBJ_DETECTED_CLOSING;
        case rd::DefaultDriver::ObjectDetectionStatus::AT_REQUESTED_POSITION:
            return ROBIF2B_ROBOTIQ_OBJ_AT_REQUESTED_POSITION;
        default:
            return ROBIF2B_ROBOTIQ_OBJ_UNKNOWN;
    }
}

constexpr enum robif2b_robotiq_gripper_status
to_robif2b_gripper_status(rd::DefaultDriver::GripperStatus status) {
    switch (status) {
        case rd::DefaultDriver::GripperStatus::RESET:
            return ROBIF2B_ROBOTIQ_RESET;
        case rd::DefaultDriver::GripperStatus::IN_PROGRESS:
            return ROBIF2B_ROBOTIQ_IN_PROGRESS;
        case rd::DefaultDriver::GripperStatus::COMPLETED:
            return ROBIF2B_ROBOTIQ_COMPLETED;
        default:
            return ROBIF2B_ROBOTIQ_UNKNOWN;
    }
}

void robif2b_robotiq_gripper_publish_measurements(struct robif2b_robotiq_gripper_nbx *b) {
    assert(b->comm);
    assert(b->position_msr);
    assert(b->obj_detection_status);
    assert(b->gripper_status);
    assert(b->error);
    assert(b->success);

    struct robif2b_robotiq_gripper_comm * comm = b->comm;

    try {
        *b->position_msr = comm->driver->get_gripper_position();
    } catch (rd::DriverException &) {
        *b->error = ROBIF2B_ROBOTIQ_ERR_READ;
        *b->success = false;
        return;
    }
    // Reusing flags updated by above get_gripper_position() call
    if (b->current_msr) *b->current_msr = comm->driver->get_last_gripper_current() * kCurrentPerCount;
    *b->obj_detection_status = to_robif2b_obj_detection_status(comm->driver->get_last_obj_detection_status());
    *b->gripper_status = to_robif2b_gripper_status(comm->driver->get_last_gripper_status());
    // Copy movement check logic from robotiq_driver
    *b->is_gripper_moving = *b->obj_detection_status == ROBIF2B_ROBOTIQ_OBJ_MOVING;
}

void robif2b_robotiq_gripper_configure(struct robif2b_robotiq_gripper_nbx *b) {
    assert(b);
    assert(b->error);
    assert(b->success);

    *b->error = ROBIF2B_ROBOTIQ_NO_ERROR;
    *b->success = true;

    b->comm = new struct robif2b_robotiq_gripper_comm;
    if (!b->comm) {
        *b->error = ROBIF2B_ROBOTIQ_ERR_MEMORY;
        *b->success = false;
        return;
    }

    struct robif2b_robotiq_gripper_comm * comm = b->comm;

    try {
        comm->serial = std::make_unique<rd::DefaultSerial>();
    } catch (serial::IOException &) {
        *b->error = ROBIF2B_ROBOTIQ_ERR_CONNECTION;
        *b->success = false;
        return;
    }

    comm->serial->set_port(b->serial.port);
    comm->serial->set_baudrate(b->serial.baudrate);
    comm->serial->set_timeout(std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::duration<double, std::milli>(b->serial.timeout_ms)));

    comm->driver = std::make_unique<rd::DefaultDriver>(std::move(comm->serial));
    comm->driver->set_slave_address(b->serial.slave_address);

    bool connected = false;
    try {
        connected = comm->driver->connect();
    } catch (serial::IOException &) {
        *b->error = ROBIF2B_ROBOTIQ_ERR_CONNECTION;
        *b->success = false;
        return;
    }
    if (!connected) {
        *b->error = ROBIF2B_ROBOTIQ_ERR_CONNECTION;
        *b->success = false;
        return;
    }

    try {
        comm->driver->deactivate();
    } catch (rd::DriverException &) {
        *b->error = ROBIF2B_ROBOTIQ_ERR_DEACTIVATION;
        *b->success = false;
        return;
    }

    try {
        comm->driver->activate();
    } catch (rd::DriverException &) {
        *b->error = ROBIF2B_ROBOTIQ_ERR_ACTIVATION;
        *b->success = false;
        return;
    }

    robif2b_robotiq_gripper_publish_measurements(b);
}

void robif2b_robotiq_gripper_shutdown(struct robif2b_robotiq_gripper_nbx *b) {
    assert(b);
    assert(b->error);
    assert(b->success);
    assert(b->comm);

    delete b->comm;

    *b->error = ROBIF2B_ROBOTIQ_NO_ERROR;
    *b->success = true;
}

void robif2b_robotiq_gripper_update(struct robif2b_robotiq_gripper_nbx *b) {
    assert(b);
    assert(b->comm);
    assert(b->error);
    assert(b->success);

    *b->error = ROBIF2B_ROBOTIQ_NO_ERROR;
    *b->success = true;

    struct robif2b_robotiq_gripper_comm * comm = b->comm;
    // Only set_gripper_position() will send data via serial
    comm->driver->set_force(*b->force_cmd);
    comm->driver->set_speed(*b->speed_cmd);
    try {
        comm->driver->set_gripper_position(*b->position_cmd);
    } catch (rd::DriverException &) {
        *b->error = ROBIF2B_ROBOTIQ_ERR_WRITE;
        *b->success = false;
        return;
    }

    robif2b_robotiq_gripper_publish_measurements(b);
}
