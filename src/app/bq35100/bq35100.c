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

    // Read the actual data
    ret = sensirion_i2c_read(BQ35100_I2C_ADDRESS, buffer, len);
    if (ret != 0) {
        printf("[ERROR] BQ35100 Read Failed: Reg 0x%02X, Error: %d\n", reg_addr, ret);
    }

    return ret;
}