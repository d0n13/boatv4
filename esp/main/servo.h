#pragma once

#include <iostream>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/mcpwm_prelude.h"

class Servo final {

    private:
        mcpwm_cmpr_handle_t comparator = NULL;
    public:
        Servo();
        void set(int val);
};