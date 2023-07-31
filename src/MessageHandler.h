#pragma once
#include "Message.h"

class MessageHandler
{
public:
    static auto onReceive(std::string) -> void;
};