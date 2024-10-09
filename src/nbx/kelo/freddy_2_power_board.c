// SPDX-License-Identifier: LGPL-3.0
#include <robif2b/functions/freddy_2_power_board.h>

#include <string.h>
#include <assert.h>


void robif2b_freddy_2_power_board_update(struct robif2b_freddy_2_power_board *b)
{
    assert(b);
    assert(b->msr_pdo);
    assert(b->cmd_pdo);

    if (b->voltage_msr) *b->voltage_msr = b->msr_pdo->voltage;
    if (b->current_msr) *b->current_msr = b->msr_pdo->current;
    if (b->power_msr)   *b->power_msr   = b->msr_pdo->power;
    if (b->status)      *b->status      = b->msr_pdo->status;
    if (b->time_stamp)  *b->time_stamp  = b->msr_pdo->time_stamp;

    // Commands API currently not used
    b->cmd_pdo->command  = b->cmd_pdo->command;
    b->cmd_pdo->shutdown   = b->cmd_pdo->shutdown;
}
