#pragma once

#include "driver/ledc.h"

class Trottle final {

    private :
        ledc_channel_config_t ledc_channel;

    public:
        Trottle();
        void set(int percentage);
};