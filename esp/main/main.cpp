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

#include "main.h"
#include "servo.h"
#include "trottle.h"
#include "joystick.h"

void HelloCMake::run(void)
{
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

    // for (int i = 10; i >= 0; i--) {
    //     printf("Restarting in %d seconds...\n", i);
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    // }
    // printf("Restarting now.\n");
    fflush(stdout);
    // esp_restart();
}

static int map(long x, long in_min, long in_max, long out_min, long out_max) {

    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
    // int x = (inValue - 0) / (4095 - 0);
    // int ret = 500 + (2500 - 500) * x;
    // printf("%d\n", ret);
    // return ret;
// }

extern "C" void app_main(void)
{
    HelloCMake App;
    Servo servo;
    Trottle trottle;
    Joystick joy;

    App.run();
    // servo.run();
    trottle.set(50);

    for (int i = 0; i < 4095;i++) {
        printf("%d\n", map(i, 0, 4096, 500, 2500));
    }
    while (1)
    {
        servo.set(map(joy.getX(), 0, 4096, 700, 2300));
        // printf("x: %4d y: %4d\n", joy.getX(), joy.getY());
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

template<typename tVal>
tVal map_value(std::pair<tVal,tVal> a, std::pair<tVal, tVal> b, tVal inVal)
{
	tVal inValNorm = inVal - a.first;
	tVal aUpperNorm = a.second - a.first;
	tVal normPosition = inValNorm / aUpperNorm;

	tVal bUpperNorm = b.second - b.first;
	tVal bValNorm = normPosition * bUpperNorm;
	tVal outVal = b.first + bValNorm;

	return outVal;
}