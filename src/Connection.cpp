#include "Connection.h"
#include "LoRaConnection.h"

auto Connection::createLoRaConnection(long frequency, void (*callback)(std::string)) -> Connection *
{
    return new LoRaConnection(frequency, callback);
}