#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include "Configuration.h"
#include "Connection.h"
#include "utils.h"
#include "MessageHandler.h"
#include "PositionHandler.h"
#include "PowerManagement.h"

class Beacon
{
public:
    Beacon() { this->_is_valid = false; };
    auto tick() -> void;
    auto initBeacon() -> bool;

private:
    Connection *connection;
    PositionHandler *positionHandler;
    PowerManagement *powerManagement;
    MessageHandler *messageHandler;

protected:
    bool _is_valid;
};