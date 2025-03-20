#include "drivers/i2c_driver.h"
#include "sht4x/sht4x.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define SHT4X_I2C_ADDRESS 0x44  // Default I2C address for SHT45

// Access registered I2C driver
extern const devoptab_t i2c_device_devops;

// Define SHT4x with existing I2C driver structure
static i2c_device_t sht4x_device = {
    .port = I2C_PORT_1,
    .address = SHT4X_I2C_ADDRESS
};


/**
 * @brief Writes data to the SHT45 sensor using SE I2C
 * 
 * @param address I2C register
 * @param data Pointer to data buffer
 * @param count Number of bytes
 * @return 0 on success, -1 on failure
 */
 int8_t sensirion_i2c_write(uint8_t address, const uint8_t* data, uint16_t count) {
    if (i2c_device_devops.ioctl == NULL) {
        printf("ERROR: I2C ioctl function is NULL! The I2C driver may not be registered.\n");
        return -1;
    }
    
    i2c_transfer_t transfer;

    transfer.addr.len = 1;
    transfer.addr.data = &address;

    transfer.value.len = count;
    transfer.value.data = (uint8_t*)data;

    va_list args;
    va_start(args, &transfer);

    long ret = i2c_device_devops.ioctl(0, &sht4x_device, I2C_WRITE_REG, args);
   
    va_end(args);

    if (ret < 0) {
        printf("ERROR: SHT45 I2C Write Failed: %ld\n", ret);
        return -1;
    }

    return (int8_t)ret;
 }

/**
 * @brief Reads data from the SHT45 using SE I2C
 * 
 * @param address I2C register
 * @param data Pointer to data buffer
 * @param count Number of bytes
 * @return 0 on success, -1 on failure
 */
int8_t sensirion_i2c_read(uint8_t address, uint8_t* data, uint16_t count){
    if (i2c_device_devops.ioctl == NULL) {
        printf("ERROR: I2C ioctl function is NULL! The I2C driver may not be registered.\n");
        return -1;
    }
    
    i2c_transfer_t transfer;

    transfer.addr.len = 1;
    transfer.addr.data = &address;

    transfer.value.len = count;
    transfer.value.data = (uint8_t*)data;

    va_list args;
    va_start(args, &transfer);
    
    long ret = i2c_device_devops.ioctl(0, &sht4x_device, I2C_WRITE_REG, args);
   
    va_end(args);
   
    if (ret < 0) {
        printf("ERROR: SHT45 I2C Read Failed: %ld\n", ret);
        return -1;
    }

    return (int8_t)ret;
}