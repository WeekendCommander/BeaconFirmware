#pragma once
#include <string>


class Connection
{
public:
    // factory Methods
    static auto createLoRaConnection(long frequency, void (*callback)(std::string)) -> Connection *;

    virtual auto writeData(std::string Message) -> void;
    virtual auto isWritable() -> bool;
};