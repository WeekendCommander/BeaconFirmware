#pragma once
#include <TinyGPS++.h>
#include <Arduino.h>
#include "Configuration.h"
#include "Connection.h"

class PositionHandler
{
public:
    PositionHandler();
    auto updatePosition() -> void;
    auto sendPositionMessage(Connection *c) -> void;

private:
    TinyGPSPlus gps[POSITION_SMOOTHING];
    int index;
    bool is_valid;
};