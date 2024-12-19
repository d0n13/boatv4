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
#include "esp_log.h"

#include "servo.h"
#include "trottle.h"
#include "joystick.h"
#include "map.h"

#include "lvgl.h"

static const char *TAG = "main";

extern "C"
{

    void display_demo_ui(void);
    void set_rudder_value(void *indic, int32_t v);
    void set_value(void *i, int32_t v);
    bool example_lvgl_lock(int timeout_ms);
    void example_lvgl_unlock(void);

    void app_main(void)
    {
        // Servo servo;
        // Trottle trottle;
        // Joystick joy;

        display_demo_ui();

        uint32_t task_delay_ms = 1;

        /* Blocking loop */
        while (1)
        {
            vTaskDelay(pdMS_TO_TICKS(500));
            lv_task_handler();
        }

        // while (true) {
        //     // ESP_LOGI(TAG, "steer:%3d trottle:%3d", joy.getX(), joy.getY());
        //     int steer = map(joy.getX(), 0, 4095, 0, 100);
        //     int power = map(joy.getY(), 0, 4095, 100, 0);

        //     // if(time_till_next > 1) {
        //     //     ESP_LOGI(TAG, "steer:%3d trottle:%3d. Sleep %lums", steer, power, time_till_next);
        //     //     continue;
        //     // }

        //     if (example_lvgl_lock(-1)) {
        //         task_delay_ms = lv_timer_handler();

        //         set_rudder_value(NULL, steer);
        //         set_value(NULL, power);

        //         // Release the mutex
        //         example_lvgl_unlock();

        //         servo.set(steer);
        //     }
        //     if (task_delay_ms > 50) {
        //         task_delay_ms = 50;
        //     } else if (task_delay_ms < 1) {
        //         task_delay_ms = 1;
        //     }
        //     vTaskDelay(pdMS_TO_TICKS(task_delay_ms));

        // os_delay_ms(time_till_next); /* delay to avoid unnecessary polling */

        // servo.set(steer);
        // trottle.set(power);

        // uint32_t delay = lv_timer_handler();
        // if (delay < 1) {
        //     delay = 1;
        // }
        // usleep(delay * 1000);

        // uint32_t time_till_next = lv_timer_handler();
        // vTaskDelay(pdMS_TO_TICKS(time_till_next));
        // }
    }
}