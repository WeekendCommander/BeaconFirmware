#pragma once
#include "Payload.h"
#include "MessageTypes.h"
#include "Connection.h"

class Message
{
public:
    static auto parseMessage(std::string &s) -> Message *;
    static auto checkRepeatCount(std::string &s) -> uint8_t;
    auto send(Connection *c) -> void;

public:
    ~Message()
    {
        Serial.println("Message:: ~Message()");
        delete payload;
    };
    int rc{0};
    int receiver{0};
    int sender{0};
    MESSAGE_TYPES type{MESSAGE_TYPE_UNKNOWN};
    Payload *payload = nullptr;
};
