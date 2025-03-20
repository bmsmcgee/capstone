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
#include "log.h"
#include "sht4x/sht4x.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include <time.h>



LOG_LEVEL_INIT(LOG_LEVEL_INFO);


static void send_sht45_data(void);

int app_main(int argc, const char *argv[]) {
    (void)argc;
    (void)argv;

    // Initialize Rodeo system
    rodeo_init("demo");

    while (1) {
        send_sht45_data();
        INFO("Example log %lld\n", (long long int) time(0));
        sleep(5);
    }

    return 0;
}

static void send_sht45_data(void) {
    int32_t sht_temp, sht_hum;

    if (sht4x_measure_blocking_read(&sht_temp, &sht_hum) == 0) {
        INFO("SHT45 Temp: %.2f°C, Humidity: %.2f%%\n", (float)sht_temp / 1000, (float)sht_hum / 1000);
    } else {
        ERROR("SHT45 read failed\n");
        return;
    }

    float sht_data[2] = {sht_temp, sht_hum};
    if (send_sensor_msg_float_array_by_id(SENSOR_ID_ENV, sht_data, 2) != 0) {
        ERROR("Failed to send SHT45 sensor data\n");
    }
}