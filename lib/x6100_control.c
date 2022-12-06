/*
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  Aether Xiegu X6100 Control
 *
 *  Copyright (c) 2022 Belousov Oleg aka R1CBU
 *  Copyright (c) 2022 Rui Oliveira aka CT7ALW
 */

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include "x6100_control.h"


typedef struct __attribute__((__packed__)) {
	uint16_t	addr;
	uint32_t	arg;
} cmd_struct_t;

typedef struct __attribute__((__packed__)) {
	uint16_t	addr;
	uint32_t	arg[x6100_last + 1];
} all_cmd_struct_t;

static int				i2c_fd = 0;
static int				i2c_addr = 0x72;
static all_cmd_struct_t	all_cmd;

/* * */

static bool send_regs(void *regs, size_t size) {
  	struct i2c_msg				message = { i2c_addr, 0, size, regs };
  	struct i2c_rdwr_ioctl_data	ioctl_data = { &message, 1 };

    int res = ioctl(i2c_fd, I2C_RDWR, &ioctl_data);

    return (res > 0);
}

bool x6100_control_init() {
	i2c_fd = open("/dev/i2c-0", O_RDWR);

    if (i2c_fd < 0)
        return false;

    memset(&all_cmd, 0, sizeof(all_cmd));

    all_cmd.arg[x6100_rxvol] = 0;
    all_cmd.arg[x6100_rfg_txpwr] = (10 << 8) | 64;

    all_cmd.arg[x6100_filter1_low] = (uint32_t)50.0f;
    all_cmd.arg[x6100_filter1_high] = (uint32_t)2950.0;
    all_cmd.arg[x6100_filter2_low] = (uint32_t)50.0f;
    all_cmd.arg[x6100_filter2_high] = (uint32_t)2950.0f;

    return send_regs(&all_cmd, sizeof(all_cmd));
}

bool x6100_control_cmd(x6100_cmd_enum_t cmd, uint32_t arg) {
	cmd_struct_t				command;
  	uint16_t					addr = cmd * 4;

    command.addr = (addr & 0xFF) << 8 | (addr >> 8);
    command.arg = arg;

    int res = send_regs(&command, sizeof(command));

    if (res > 0)
    {
        all_cmd.arg[cmd] = arg;
        return true;
    }

    return false;
}
