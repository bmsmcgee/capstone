#include "driver_sht4x.h"

#define CHIP_NAME "Sensirion SHT4X"
#define MANUFACTURER_NAME "Sensirion"
#define SUPPLY_VOLTAGE_MIN 1.08f
#define SUPPLY_VOLTAGE_MAX 3.6f
#define MAX_CURRENT 100.0f
#define TEMPERATURE_MIN -40.0f
#define TEMPERATURE_MAX 125.0f
#define DRIVER_VERSION 1000

#define SHT4X_COMMAND_SOFT_RESET 0x94
#define SHT4X_COMMAND_READ_SERIAL_NUMBER 0x89
#define SHT4X_MODE_HIGH_PRECISION_WITH_NO_HEATER 0xFD

static i2c_device_t sht4x_device = {
    .port = I2C_PORT_1,
    .address = SHT4X_ADDRESS_0};

int fd = -1;

static uint8_t a_sht4x_write_read(uint8_t cmd, uint16_t delay, uint8_t *data, uint16_t len) {
    if (write(fd, &cmd, sizeof(cmd)) < 0){
        return -1;
    }

    if (delay > 0){
        usleep(delay * 1000);
    }

    if (len > 0){
        if (read(fd, data, len) < 0){
            return -1;
        }
    }

    return 0;
}

static uint8_t a_sht4x_crc(uint8_t *data, uint16_t len) {
    const uint8_t POLYNOMIAL = 0x31;
    uint8_t crc = 0xFF;
    uint16_t i;
    uint16_t j;

    for (j = len; j != 0; --j) {
        crc ^= *data++;
        for (i = 8; i != 0; --i) {
            crc = (crc & 0x80) ? (crc << 1) ^ POLYNOMIAL : (crc << 1);
        }
    }
    return crc;
}

void sht45_init(void) {
    if (sht4x_init() < 0) {
        printf("sht45 not initialized");
        return;
    }
}

uint8_t sht4x_init(void) {
    driver_add("/dev/i2c0/sht45", &sht4x_device, &i2c_device_devops);

    fd = open("/dev/i2c0/sht45", 0, 0);
    if (fd < 0) {
        return -1;
    }

    if (a_sht4x_write_read(SHT4X_COMMAND_SOFT_RESET, 10, NULL, 0) < 0) {
        close(fd);
        fd = -1;
        return -1;
    }
    return 0;
}

uint8_t sht4x_read(uint16_t *temperature_raw, float *temperature_s, uint16_t *humidity_raw, float *humidity_s) {
    uint8_t res;
    uint8_t buf[6];

    res = a_sht4x_write_read(SHT4X_MODE_HIGH_PRECISION_WITH_NO_HEATER, 10, buf, 6);

    if (res != 0) {
        printf("sht4x: write command failed.\n");

        return -1;
    }

    if (a_sht4x_crc(buf + 0, 2) != buf[2]) {
        printf("sht4x: crc is error.\n");
        return -1;
    }

    if (a_sht4x_crc(buf + 3, 2) != buf[5]) {
        printf("sht4x: crc is error.\n");
        return -4;
    }

    *temperature_raw = (uint16_t)((((uint16_t)buf[0]) << 8) | buf[1]);

    *humidity_raw = (uint16_t)((((uint16_t)buf[3]) << 8) | buf[4]);

    *temperature_s = (float)(*temperature_raw) / 65535.0f * 175.0f - 45.0f;

    *humidity_s = (((float)(*humidity_raw) / 65535.0f) * 125.0f - 6.0f);

    if ((*humidity_s) > 100.0f) {
        *humidity_s = 100.0f;
    }

    if ((*humidity_s) < 0.0f) {
        *humidity_s = 0.0f;
    }

    return 0;
}

uint8_t sht4x_get_serial_number(uint8_t num[4])
{
    uint8_t res;
    uint8_t buf[6];

    res = a_sht4x_write_read(SHT4X_COMMAND_READ_SERIAL_NUMBER, 10, buf, 6); 
    if (res != 0) {
        printf("sht4x: write command failed.\n");
        return -1;
    }

    if (a_sht4x_crc(buf + 0, 2) != buf[2]) {
        printf("sht4x: crc is error.\n");
        return -1;
    }

    if (a_sht4x_crc(buf + 3, 2) != buf[5]) {
        printf("sht4x: crc is error.\n");
        return -1;
    }

    num[0] = buf[0];
    num[1] = buf[1];
    num[2] = buf[3];
    num[3] = buf[4];

    return 0;
}

uint8_t sht4x_soft_reset() {
    uint8_t res;

    res = a_sht4x_write_read(SHT4X_COMMAND_SOFT_RESET, 10, NULL, 0);

    if (res != 0) {
        printf("sht4x: write command failed.\n");
        return -1;
    }

    return 0;
}

uint8_t sht4x_write_read(uint8_t cmd, uint16_t delay_ms, uint8_t *data, uint16_t len) {
    return a_sht4x_write_read(cmd, delay_ms, data, len);       
}