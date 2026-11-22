#pragma once

struct DeviceStatus {
    double temperature = 0.0;
    double target_temperature = 0.0;
    bool heater_enabled = false;
    bool sensor_ok = false;
};