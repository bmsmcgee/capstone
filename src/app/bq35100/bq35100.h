#ifndef BQ35100_H
#define BQ35100_H

#include <stdint.h>

// Default 7-bit I2C address for the BQ35100 fuel gauge
#define BQ35100_I2C_ADDRESS     0x55

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

#endif /* bq35100 */