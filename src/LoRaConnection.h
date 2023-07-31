#pragma once
#include "Connection.h"
#include <LoRa.h>
#include <SPI.h>
#include "Configuration.h"
#include <atomic>
#include "utils.h"

class LoRaConnection : public Connection
{

public:
    LoRaConnection(long frequency, void (*callback)(std::string));
    virtual auto writeData(std::string message) -> void;
    virtual auto isWritable() -> bool;

protected:
    static auto rxMode() -> void;
    static auto txMode() -> void;
    static auto onTxDone() -> void;
    static auto onReceive(int packetSize) -> void;

private:
    static void (*_onReceive)(std::string);
    static std::atomic<bool> _isWritable;
};