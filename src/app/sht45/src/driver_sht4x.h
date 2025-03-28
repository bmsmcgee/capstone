#ifndef DRIVER_SHT4X_H
#define DRIVER_SHT4X_H

#include "drivers/i2c_driver.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum{
        SHT4X_ADDRESS_0 = 0x44, 
        SHT4X_ADDRESS_1 = 0x45, 
        SHT4X_ADDRESS_2 = 0x46, 
    } sht4x_address_t;

    void sht45_init(void);

    uint8_t sht4x_init(void);

    uint8_t sht4x_read(uint16_t *temperature_raw, float *temperature_s, uint16_t *humidity_raw, float *humidity_s);

    uint8_t sht4x_soft_reset();

    uint8_t sht4x_get_serial_number(uint8_t num[4]);

    uint8_t sht4x_write_read(uint8_t cmd, uint16_t delay_ms, uint8_t *data, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif