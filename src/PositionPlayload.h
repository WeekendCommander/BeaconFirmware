#pragma once
#include "Payload.h"
#include "utils.h"

class PositionPayload : public Payload
{
public:
    virtual auto toJsonString() -> std::string;
    virtual ~PositionPayload();
public:
    double lat{0}, lon{0};
    bool vld{0};
    int sat{0}, alt{0}, azm{0};
};