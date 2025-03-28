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
#include "driver_sht4x.h"
// #include "sht4x/sht4x.h"
// #include "bq35100/bq35100.h"
// #include "sensirion_i2c.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include <time.h>

LOG_LEVEL_INIT(LOG_LEVEL_INFO);

static void send_sht4x_data(void);

int app_main(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;

    INFO("%s", "Initializing Rodeo IoT System...");
    // rodeo_init("demo");

    INFO("%s", "Initializing I2C communication...");

    sht45_init();

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


static void send_sht4x_data(void)
{
    uint16_t raw_temp;
    uint16_t raw_hum;
    float temp_cel;
    float hum_percent;

    if (sht4x_read(&raw_temp, &temp_cel, &raw_hum, &hum_percent) == 0)
    {
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