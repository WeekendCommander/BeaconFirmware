#pragma once
#include <string>
#include <ArduinoJson.h>
class Payload
{
public:
    virtual auto toJsonString() -> std::string;
};