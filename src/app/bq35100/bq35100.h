#ifndef BQ35100_H
#define BQ35100_H

#include "sensirion_i2c.h"
#include <stdint.h>

// Default 7-bit I2C address for the BQ35100 fuel gauge
#define BQ35100_I2C_ADDRESS         0x55

// Standard Operational Commands
#define BQ35100_REG_CONTROL         0x00    // Control register (2 bytes)
#define BQ35100_REG_ACC_CAPACITY    0x02    // Accumulated capacity (4 bytes, micro-Ah)
#define BQ35100_REG_TEMPERATURE     0x06    // Temperature (2 bytes, 0.1 K)
#define BQ35100_REG_VOLTAGE         0x08    // Voltage (2 bytes, mV)
#define BQ35100_REG_BATT_STATUS     0x0A    // Battery status (1 byte)
#define BQ35100_REG_BATT_ALERT      0x0B    // Battery alert (1 byte)
#define BQ35100_REG_CURRENT         0x0C    // Current (2 bytes, mA)
#define BQ35100_REG_SCALED_R        0x16    // Scaled R (2 bytes, milli-ohm)
#define BQ35100_REG_MEASURED_Z      0x22    // Measured Z (2 bytes, milli-ohm)
#define BQ35100_REG_INT_TEMP        0x28    // Internal temperature (2 bytes, 0.1 K)
#define BQ35100_REG_SOH             0x2E    // State of Health (1 byte, %)
#define BQ35100_REG_DESIGN_CAP      0x3C    // Design capacity (2 bytes, mAh)

// Calibration Registers
#define BQ35100_REG_CAL_COUNT       0x79    // Calibration Count (1 byte)
#define BQ35100_REG_CAL_CURRENT     0x7A    // Calibration Current (2 bytes, mA)
#define BQ35100_REG_CAL_VOLTAGE     0x7C    // Calibration Voltage (2 bytes, mV)
#define BQ35100_REG_CAL_TEMP        0x7E    // Calibration Temperature (2 bytes, K)

/**
 * @brief Reads data from a BQ35100 register over I²C.
 *
 * @param reg_addr Register address to read from.
 * @param buffer Pointer to store the read data.
 * @param len Number of bytes to read.
 * @return int16_t 0 on success, negative error code otherwise.
 */
int16_t bq35100_read_register(uint8_t reg_addr, uint8_t *buffer, uint16_t len);

/**
 * @brief Writes data to a BQ35100 register over I²C.
 *
 * @param reg_addr Register address to write to.
 * @param data Pointer to the data to be written.
 * @param len Number of bytes to write.
 * @return int16_t 0 on success, negative error code otherwise.
 */
int16_t bq35100_write_register(uint8_t reg_addr, const uint8_t *data, uint16_t len);

/**
 * @brief Reads a single byte from a BQ35100 register.
 *
 * @param reg_addr Register address to read from.
 * @return int16_t Register value (0-255) on success, negative error code otherwise.
 */
int16_t bq35100_read_byte(uint8_t reg_addr);

/**
 * @brief Reads a 16-bit word (2 bytes) from a BQ35100 register.
 *
 * @param reg_addr Register address to read from.
 * @return int16_t Register value on success, negative error code otherwise.
 */
int16_t bq35100_read_word(uint8_t reg_addr);

/**
 * @brief Writes a single byte to a BQ35100 register.
 *
 * @param reg_addr Register address to write to.
 * @param value Value to write.
 * @return int16_t 0 on success, negative error code otherwise.
 */
int16_t bq35100_write_byte(uint8_t reg_addr, uint8_t value);

/**
 * @brief Writes a 16-bit word (2 bytes) to a BQ35100 register.
 *
 * @param reg_addr Register address to write to.
 * @param value 16-bit value to write.
 * @return int16_t 0 on success, negative error code otherwise.
 */
int16_t bq35100_write_word(uint8_t reg_addr, uint16_t value);

/**
 * @brief Gets data about battery from fuel gauge.
 *
 * @param voltage Displays the voltage of the battery
 * @param current Displays the current of the battery
 * @param temp Displays the internal temperature of the battery
 * @param soh Displays the State of Health of the battery
 * @return 0 on success, negative error code otherwise.
 */
int16_t bq35100_read_all_blocking(int16_t *voltage, int16_t *current, float *temp, int16_t *soh);

#endif /* bq35100 */