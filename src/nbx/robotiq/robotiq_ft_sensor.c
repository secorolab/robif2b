#include <assert.h>
#include <robotiq_ft/rq_sensor_com.h>
#include <robotiq_ft/rq_sensor_state.h>
#include "robif2b/functions/robotiq_ft_sensor.h"

#define IDX_FORCE_X 0
#define IDX_FORCE_Y 1
#define IDX_FORCE_Z 2
#define IDX_MOMENT_X 3
#define IDX_MOMENT_Y 4
#define IDX_MOMENT_Z 5

void robif2b_robotiq_ft_configure(struct robif2b_robotiq_ft_sensor_nbx *b) {
    assert(b);
    assert(b->success);
    assert(b->force_msr);
    assert(b->force_offset);
    assert(b->moment_msr);
    assert(b->moment_offset);

    uint8_t dev_found = rq_com_identify_device(b->serial.port);
    if (!dev_found) {
        *b->success = false;
        return;
    }
    rq_com_start_stream();

    // Set success here to allow overwrite in update()
    *b->success = true;

    // Read first measurements
    robif2b_robotiq_ft_update(b);
}

void robif2b_robotiq_ft_shutdown(struct robif2b_robotiq_ft_sensor_nbx *b) {
    assert(b);
    assert(b->success);

    rq_com_send_jam_signal();
    stop_connection();

    *b->success = true;
}

void robif2b_robotiq_ft_update(struct robif2b_robotiq_ft_sensor_nbx *b) {
    assert(b);
    assert(b->success);
    assert(b->force_msr);
    assert(b->force_offset);
    assert(b->moment_msr);
    assert(b->moment_offset);

    rq_com_listen_stream();
    if (rq_com_get_valid_stream() == false) {
        *b->success = false;
        return;
    }

    b->force_msr[0] = rq_state_get_received_data(IDX_FORCE_X) - b->force_offset[0];
    b->force_msr[1] = rq_state_get_received_data(IDX_FORCE_Y) - b->force_offset[1];
    b->force_msr[2] = rq_state_get_received_data(IDX_FORCE_Z) - b->force_offset[2];

    b->moment_msr[0] = rq_state_get_received_data(IDX_MOMENT_X) - b->moment_offset[0];
    b->moment_msr[1] = rq_state_get_received_data(IDX_MOMENT_Y) - b->moment_offset[1];
    b->moment_msr[2] = rq_state_get_received_data(IDX_MOMENT_Z) - b->moment_offset[2];

    *b->success = true;
}

void robif2b_robotiq_ft_zero(struct robif2b_robotiq_ft_sensor_nbx *b) {
    assert(b);
    assert(b->success);

    rq_set_zero();
    *b->success = true;
}
