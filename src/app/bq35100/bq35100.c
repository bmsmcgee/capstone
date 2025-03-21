#include "bq35100.h"
#include <stdio.h>
#include <unistd.h>

int16_t bq35100_read_register(uint8_t reg_addr, uint8_t *buffer, uint16_t len) {
    int16_t ret;

    // Send register address first
    ret = sensirion_i2c_write(BQ35100_I2C_ADDRESS, &reg_addr, 1);
    if (ret != 0) {
        printf("[ERROR] BQ35100 Read Failed (Write Phase): Reg 0x%02X, Error: %d\n", reg_addr, ret);
        return ret;
    }

    // Perform I2C read
    ret = sensirion_i2c_read(BQ35100_I2C_ADDRESS, buffer, len);
    if (ret != 0) {
        printf("[ERROR] BQ35100 Read Failed: Reg 0x%02X, Error: %d\n", reg_addr, ret);
    }

    return ret;
}

int16_t bq35100_write_register(uint8_t reg_addr, const uint8_t *data, uint16_t len) {
    // First byte: register address, rest: data
    uint8_t buffer[len + 1];  

    buffer[0] = reg_addr;  // Set register address
    for (uint16_t i = 0; i < len; i++) {
        buffer[i + 1] = data[i];  // Copy data into buffer
    }

    // Perform I2C write
    int16_t ret = sensirion_i2c_write(BQ35100_I2C_ADDRESS, buffer, len + 1);
    if (ret != 0) {
        printf("[ERROR] BQ35100 Write Failed: Reg 0x%02X, Error: %d\n", reg_addr, ret);
    }

    return ret;
}

int16_t bq35100_read_byte(uint8_t reg_addr) {
    uint8_t buffer;
    int16_t ret = bq35100_read_register(reg_addr, &buffer, 1);
    return (ret == 0) ? buffer : ret;
}