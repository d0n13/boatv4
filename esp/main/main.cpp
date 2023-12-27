/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <iostream>

#include "servo.h"
#include "trottle.h"
#include "joystick.h"
#include "map.h"
#include "esp_log.h"


static const char *TAG = "main";

extern "C" void app_main(void)
{
    Servo servo;
    Trottle trottle;
    Joystick joy;

    while (true)
    {
        int steer = map(joy.getX(), 0, 4095, 0, 100);
        int power = map(joy.getY(), 0, 4095, 0, 100);

        servo.set(steer);
        trottle.set(power);

        ESP_LOGI(TAG, "steer:%3d trottle:%3d", steer, power);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}