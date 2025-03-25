/**
 * \file main.c
 * \brief Rodeo IoT environmental sensor example application.
 *
 * \copyright Copyright (c) 2024 Simply Embedded Inc. All Rights Reserved.
 *
 * This software is licensed under terms that can be found in the
 * LICENSE file in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 */

#include "rodeo.h"
#include "sensor_message.h"
#include "sht4x/sht4x.h"
// #include "bq35100/bq35100.h"
#include "sensirion_i2c.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include <time.h>

LOG_LEVEL_INIT(LOG_LEVEL_INFO);

static void send_sht4x_data(void);
// static void send_bq35100_data(void);

int app_main(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;

    INFO("%s", "Initializing Rodeo IoT System...");
    // rodeo_init("demo");

    INFO("%s", "Initializing I2C communication...");
    sensirion_i2c_init();

    INFO("%s", "Probing SHT4x sensor...");
    if (sht4x_probe() != 0)
    {
        ERR("%s", "SHT4x sensor not detected! Exiting...");
        return -1;
    }

    // INFO("%s", "Triggering BQ35100 fuel gauge to start...");
    // if (bq35100_send_subcommand(BQ35100_SUB_GAUGE_START) != 0)
    // {
    //     ERR("%s", "Failed to trigger BQ35100 into ACTIVE mode.");
    //     return -1;
    // }

    INFO("%s", "Sensors detected. Starting data collection...");

    while (1)
    {
        send_sht4x_data();
        // sleep(1);
        // send_bq35100_data();
        sleep(5);
    }

    return 0;
}
/*
 * EVERY FUNCTION AFTER THIS POINT WILL EVENTUALLY BE CONSOLIDATED INTO ONE FUNCTION TO SEND ALL VARIABLES TO CLOUD THROUGH MQTT
 */
static void send_sht4x_data(void)
{
    int32_t sht_temp;
    int32_t sht_hum;

    if (sht4x_measure_blocking_read(&sht_temp, &sht_hum) == 0)
    {
        float temp_cel = (float)sht_temp / 1000.0;
        float hum_percent = (float)sht_hum / 1000.0;

        INFO("SHT4x Sensor Data: Temperature: %.2f Celsius, Humidity: %.2f%%", temp_cel, hum_percent);

        float sht_data[2] = {temp_cel, hum_percent};

        if (send_sensor_msg_float_array_by_id(SENSOR_ID_ENV, sht_data, 2) != 0)
        {
            ERR("%s", "Failed to send SHT4x sensor data!");
        }
    }
    else
    {
        ERR("%s", "SHT4x measurement failed!");
    }
}

// static void send_bq35100_data(void)
// {
//     int16_t voltage;
//     int16_t current;
//     float temp;
//     int16_t soh;

//     if (bq35100_read_all_blocking(&voltage, &current, &temp, &soh) == 0)
//     {
//         INFO("BQ35100 Fuel Gauge Data: Voltage: %d mV, Current: %d mA, Temp: %.2f °C, SOH: %d%%",
//              voltage, current, temp, soh);

//         float fuel_data[4] = {
//             (float)voltage,
//             (float)current,
//             temp,
//             (float)soh};

//         if (send_sensor_msg_float_array_by_id(SENSOR_ID_ENV, fuel_data, 4) != 0)
//         {
//             ERR("%s", "Failed to send BQ35100 sensor data!");
//         }
//     }
//     else
//     {
//         ERR("%s", "BQ35100 fuel gauge read failed!");
//     }
// }