#pragma once
#include "Payload.h"
#include "utils.h"

class DeviceInfoPayload : public Payload
{
public:
    virtual auto toJsonString() -> std::string;
    virtual ~DeviceInfoPayload();
public:
    float vol{0};
    int pct{0};
    int chg{0};
};