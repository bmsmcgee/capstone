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
#include <stdio.h>
#include <unistd.h>
#include <time.h>

/*
 * INSTRUCTIONS
 * ============
 *
 * Implement all functions where they are marked as IMPLEMENT.
 * Follow the function specification in the comments.
 */

/**
 * Select the current i2c bus by index.
 * All following i2c operations will be directed at that bus.
 *
 * THE IMPLEMENTATION IS OPTIONAL ON SINGLE-BUS SETUPS (all sensors on the same
 * bus)
 *
 * @param bus_idx   Bus index to select
 * @returns         0 on success, an error code otherwise
 */
int16_t sensirion_i2c_select_bus(uint8_t bus_idx)
{
    // IMPLEMENT or leave empty if all sensors are located on one single bus
    return STATUS_FAIL;
}

/**
 * Initialize all hard- and software components that are needed for the I2C
 * communication.
 */
void sensirion_i2c_init(void)
{
    printf("[DEBUG] Initializing I2C hardware...\n");

    static i2c_device_t sht4x_device = {
        .port = I2C_PORT_1,
        .address = 0x44
    };

    // Register the I2C driver with the system
    // Path may be different, check when device connected to computer
    driver_add("/dev/i2c", &sht4x_device, &i2c_device_devops);
}

/**
 * Release all resources initialized by sensirion_i2c_init().
 */
void sensirion_i2c_release(void)
{
    // IMPLEMENT or leave empty if no resources need to be freed
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
    if (i2c_device_devops.ioctl == NULL)
    {
        printf("[ERROR] I2C ioctl function is NULL! The I2C driver may not be registered.\n");
        return -1;
    }

    i2c_transfer_t transfer;

    transfer.addr.len = 1;                  // Address length is 1 byte
    transfer.addr.data = &address;          // Register address for read

    transfer.value.len = count;             // Number of bytes to read
    transfer.value.data = (uint8_t *)data;  // Pointer to buffer

    // Perform write operation
    long ret = i2c_device_devops.read(0, NULL, (char*)&transfer, sizeof(transfer));

    if (ret != 0)
    {
        printf("[ERROR] I2C Read Failed for address 0x%02X: %ld\n", address, ret);
        return -1;
    }

    return (int8_t)ret;
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
int8_t sensirion_i2c_write(uint8_t address, const uint8_t *data, uint16_t count)
{
    if (i2c_device_devops.ioctl == NULL)
    {
        printf("[ERROR] I2C ioctl function is NULL! The I2C driver may not be registered.\n");
        return -1;
    }

    i2c_transfer_t transfer;

    transfer.addr.len = 1;                  // Address length is 1 byte
    transfer.addr.data = &address;          // Register address for write

    transfer.value.len = count;             // Number of bytes to write
    transfer.value.data = (uint8_t *)data;  // Pointer to buffer

    // Perform write operation
    long ret = i2c_device_devops.write(0, NULL, (const char*)&transfer, sizeof(transfer));


    if (ret != 0)
    {
        printf("[ERROR] I2C Write Failed for address 0x%02X: %ld\n", address, ret);
        return -1;
    }

    return (int8_t)ret;
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
    struct timespec req;
    req.tv_sec = useconds / 1000000;            // Convert microseconds to seconds
    req.tv_nsec = (useconds % 1000000) * 1000;  // Convert remainder to nanoseconds

    clock_nanosleep(CLOCK_MONOTONIC, 0, &req, NULL);
}
