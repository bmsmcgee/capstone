#include "bq35100.h"
#include <stdio.h>
#include <unistd.h>

int16_t bq35100_read_register(uint8_t reg_addr, uint8_t *buffer, uint16_t len) {
    int16_t ret;

    // Send register address first as a write operation
    // Tells the BQ35100 which register we want to read from
    ret = sensirion_i2c_write(BQ35100_I2C_ADDRESS, &reg_addr, 1);
    if (ret != 0) {
        printf("[ERROR] BQ35100 Read Failed (Write Phase): Reg 0x%02X, Error: %d\n", reg_addr, ret);
        return ret;
    }

    // Perform the I2C read to fetch data from the specified register
    ret = sensirion_i2c_read(BQ35100_I2C_ADDRESS, buffer, len);
    if (ret != 0) {
        printf("[ERROR] BQ35100 Read Failed: Reg 0x%02X, Error: %d\n", reg_addr, ret);
    }

    // Return success or error code
    return ret;
}

int16_t bq35100_write_register(uint8_t reg_addr, const uint8_t *data, uint16_t len) {
    // Create a temporary buffer to hold the register address and data
    uint8_t buffer[len + 1];  

    // Set register address as first byte
    buffer[0] = reg_addr;  

    // Copy data that is to be written into the buffer
    for (uint16_t i = 0; i < len; i++) {
        buffer[i + 1] = data[i];  
    }

    // Perform the I2C write with the combined buffer
    int16_t ret = sensirion_i2c_write(BQ35100_I2C_ADDRESS, buffer, len + 1);
    if (ret != 0) {
        printf("[ERROR] BQ35100 Write Failed: Reg 0x%02X, Error: %d\n", reg_addr, ret);
    }

    // Return success or error code
    return ret;
}

int16_t bq35100_read_byte(uint8_t reg_addr) {
    // Temporary storage for single byte
    uint8_t buffer;     

    // Read the single byte
    int16_t ret = bq35100_read_register(reg_addr, &buffer, 1);

    // If operation successful, return byte value
    return (ret == 0) ? buffer : ret;
}

int16_t bq35100_read_word(uint8_t reg_addr) {
    // Temporary storage to hold 2 bytes
    uint8_t buffer[2];  

    // Read the 2 bytes
    int16_t ret = bq35100_read_register(reg_addr, buffer, 2);

    // Return error code if failed
    if (ret != 0) {
        return ret;
    }

    // Return combination of bytes in little endian format
    return (buffer[1] << 8) | buffer[0];
}

int16_t bq35100_write_byte(uint8_t reg_addr, uint8_t value) {
    return bq35100_write_register(reg_addr, &value, 1);
}

int16_t bq35100_write_word(uint8_t reg_addr, uint16_t value) {
    // Convert 16-bit value into 2 seperate bytes
    uint8_t buffer[2] = {
        (uint8_t)(value & 0xFF),        // Lower byte
        (uint8_t)((value >> 8) & 0xFF)  // Upper byte
    };

    // Write the 2 bytes
    return bq35100_write_register(reg_addr, buffer, 2);
}