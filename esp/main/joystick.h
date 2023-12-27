#pragma once

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class Joystick {

    private:
        bool calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle);
        void calibration_deinit(adc_cali_handle_t handle);

        adc_oneshot_unit_handle_t adc1_handle;
        bool do_calibration1_chan0;
        bool do_calibration1_chan3;
        adc_cali_handle_t adc1_cali_chan0_handle = NULL;
        adc_cali_handle_t adc1_cali_chan3_handle = NULL;
        int adc_raw[2][10];
        int voltage[2][10];

    public:
        Joystick();
        ~Joystick();
        
        int getX(void);
        int getY(void);
};