/*
 * Copyright (c) 2018, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "sensirion_arch_config.h"
#include "sensirion_common.h"
#include "sensirion_i2c.h"
#include "drivers/i2c_driver.h"
#include "sensor_message.h"
#include "sys/ioctl.h" // from sdk
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <stdarg.h>

#define SHT4X_I2C_ADDRESS 0x44
#define SHT4X_CMD_SOFT_RESET 0x94
#define SHT4X_CMD_MEASURE_HPM 0xFD

LOG_LEVEL_INIT(LOG_LEVEL_INFO);

static int fd = -1;

static i2c_device_t sht4x_device = {
    .port = I2C_PORT_1,
    .address = SHT4X_I2C_ADDRESS
};

/**
 * Initialize all hard- and software components that are needed for the I2C
 * communication.
 */
void sensirion_i2c_init(void)
{
    driver_add("/dev/sht45", &sht4x_device, &i2c_device_devops);

    uint8_t cmd = SHT4X_CMD_MEASURE_HPM;

    // fd = i2c_device_devops.open(&sht4x_device, "/dev/sht45", 0, 0);
    fd = open("/dev/sht45", O_RDWR);
    if (fd < 0)
    {
        ERR("%s", "failed to open file from device\n");
        return;
    }

    int8_t ret = sensirion_i2c_write(SHT4X_I2C_ADDRESS, &cmd, sizeof(cmd));
    if (ret < 0)
    {
        ERR("%s", "SHT4x init failed\n");
        return;
    }
    INFO("%s", "SHT4x init successfully.\n");
}

/**
 * Execute one write transaction on the I2C bus, sending a given number of
 * bytes. The bytes in the supplied buffer must be sent to the given address. If
 * the slave device does not acknowledge any of the bytes, an error shall be
 * returned.
 *
 * @param address 7-bit I2C address to write to
 * @param data    pointer to the buffer containing the data to write
 * @param count   number of bytes to read from the buffer and send over I2C
 * @returns 0 on success, error code otherwise
 */
int8_t sensirion_i2c_write(uint8_t address, const uint8_t *data, uint16_t count) {

    i2c_transfer_t transfer = {
        .addr = {
            .len = 1,
            .data = 0x00
        },
        .value = {
            .len = count,
            .data = (uint8_t*)data
        }
    };

    // long ret = i2c_device_devops.write(fd, &sht4x_device, (const char*)&transfer, sizeof(transfer));
    long ret = ioctl(fd, I2C_WRITE_REG, &transfer);

    if (ret < 0) {
        ERR("%s", "direct write failed");
        return -2;
    }

    return 0;
}

/**
 * Execute one read transaction on the I2C bus, reading a given number of bytes.
 * If the device does not acknowledge the read command, an error shall be
 * returned.
 *
 * @param address 7-bit I2C address to read from
 * @param data    pointer to the buffer where the data is to be stored
 * @param count   number of bytes to read from I2C and store in the buffer
 * @returns 0 on success, error code otherwise
 */
int8_t sensirion_i2c_read(uint8_t address, uint8_t *data, uint16_t count)
{
    i2c_transfer_t transfer = {
        .addr = {
            .len = 0,
            .data = NULL
        },
        .value = {
            .len = count, 
            .data = (uint8_t *)data
        }
    };

    // Perform read operation
    // long ret = i2c_device_devops.ioctl(fd, pd, I2C_READ_REG, &transfer);
    long ret = ioctl(fd, I2C_READ_REG, &transfer);
    

    if (ret < 0)
    {
        ERR("[ERROR] I2C Read Failed for address 0x%02X: %ld\n", address, ret);
        return -1;
    }

    return 0;
}


/**
 * Sleep for a given number of microseconds. The function should delay the
 * execution for at least the given time, but may also sleep longer.
 *
 * Despite the unit, a <10 millisecond precision is sufficient.
 *
 * @param useconds the sleep time in microseconds
 */

void sensirion_sleep_usec(uint32_t useconds)
{
    usleep(useconds);
}
