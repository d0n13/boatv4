/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "servo.h"

#include "esp_log.h"

static const char *TAG = "example";

// Please consult the datasheet of your servo before changing the following parameters
#define SERVO_MIN_PULSEWIDTH_US 500  // Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH_US 2500  // Maximum pulse width in microsecond
#define SERVO_MIN_DEGREE        -90   // Minimum angle
#define SERVO_MAX_DEGREE        90    // Maximum angle

#define SERVO_PULSE_GPIO             0        // GPIO connects to the PWM signal line
#define SERVO_TIMEBASE_RESOLUTION_HZ 1000000  // 1MHz, 1us per tick
#define SERVO_TIMEBASE_PERIOD        20000    // 20000 ticks, 20ms

static inline uint32_t example_angle_to_compare(int angle)
{
    return (angle - SERVO_MIN_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) / (SERVO_MAX_DEGREE - SERVO_MIN_DEGREE) + SERVO_MIN_PULSEWIDTH_US;
}

Servo::Servo() {
    ESP_LOGI(TAG, "Create timer and operator");
    mcpwm_timer_handle_t timer = NULL;
    mcpwm_timer_config_t timer_config = {
    .group_id = 0,
    .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
    .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
    .count_mode = MCPWM_TIMER_COUNT_MODE_UP_DOWN,
    .period_ticks = SERVO_TIMEBASE_PERIOD,
    .intr_priority = 0,

    };
    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &timer));

    mcpwm_oper_handle_t oper = NULL;
    mcpwm_operator_config_t operator_config = {
        .group_id = 0, // operator must be in the same group to the timer
    };
    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &oper));

    ESP_LOGI(TAG, "Connect timer and operator");
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper, timer));

    ESP_LOGI(TAG, "Create comparator and generator from the operator");
    mcpwm_comparator_config_t comparator_config = {
        .intr_priority = 0,

    };
    ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &comparator_config, &comparator));

    mcpwm_gen_handle_t generator = NULL;
    mcpwm_generator_config_t generator_config = {

        .gen_gpio_num = SERVO_PULSE_GPIO,
    };
    ESP_ERROR_CHECK(mcpwm_new_generator(oper, &generator_config, &generator));

    // set the initial compare value, so that the servo will spin to the center position
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(0)));

    ESP_LOGI(TAG, "Set generator action on timer and compare event");
    // go high on counter empty
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator,
                                                              MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    // go low on compare threshold
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator,
                                                                MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator, MCPWM_GEN_ACTION_LOW)));

    ESP_LOGI(TAG, "Enable and start timer");
    ESP_ERROR_CHECK(mcpwm_timer_enable(timer));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP));
}
void Servo::set(int val)
{

    ESP_LOGI(TAG, "Pulse width=%d", val);
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, val));
}

//     int angle = 0;
//     int step = 5;

//     for(int x = 0; x < 1; x++) {

//         ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, 1255));
//         vTaskDelay(pdMS_TO_TICKS(500));
//         ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, 2500));
//         vTaskDelay(pdMS_TO_TICKS(500));
//         // ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(-45)));
//         // vTaskDelay(pdMS_TO_TICKS(2000));
//     }
//     ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(-2)));
//     // while (1) {
//     //     printf("Angle of rotation: %d\n", angle);
//     //     fflush(stdout);
//     //     ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(angle)));
//     //     //Add delay, since it takes time for servo to rotate, usually 200ms/60degree rotation under 5V power supply
//     //     vTaskDelay(pdMS_TO_TICKS(100));
//     //     if ((angle + step) > 90 || (angle + step) < -90) {
//     //         step *= -1;
//     //     }
//     //     angle += step;
//     // }
// }