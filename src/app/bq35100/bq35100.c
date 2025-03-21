#include "bq35100.h"
#include <stdio.h>
#include <unistd.h>

int16_t bq35100_read_register(uint8_t address, uint8_t *data, uint16_t count) {
    int16_t ret;
    
    // Send register address
    ret = sensirion_i2c_write(BQ35100_I2C_ADDRESS, &address, 1);
    if (ret != 0) {
        printf("[ERROR] Failed to send register address 0x%02X\n", address);
        return ret;
    }

    // Read data from the specified register
    ret = sensirion_i2c_read(BQ35100_I2C_ADDRESS, data, count);
    if (ret != 0) {
        printf("[ERROR] Failed to read from register 0x%02X\n", address);
        return ret;
    }

    return 0;
    
}

int16_t bq35100_write_register(uint8_t address, uint8_t *data, uint16_t count) {
    
}