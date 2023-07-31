#include "LoRaConnection.h"

std::atomic<bool> LoRaConnection::_isWritable;
void (*LoRaConnection::_onReceive)(std::string) = nullptr;

LoRaConnection::LoRaConnection(long frequency, void (*callback)(std::string))
{
    LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);


    if (!LoRa.begin(frequency))
    {
        SERIAL_PRINTLN("Starting LoRa failed!");
        while (1)
            ;
    }

    LoRa.onReceive(onReceive);
    _onReceive = callback;
    LoRa.onTxDone(this->onTxDone);
    rxMode();
    _isWritable = true;
}

auto LoRaConnection::isWritable() -> bool
{
    return _isWritable;
}

auto LoRaConnection::writeData(std::string message) -> void
{
    SERIAL_PRINTLN("writeData()> Start");
    this->txMode();
    LoRa.beginPacket();          // start packet
    SERIAL_PRINT("writeData()> Message: ");
    SERIAL_PRINTLN(message.c_str());
    LoRa.print(message.c_str()); // add payload
    LoRa.endPacket(true);        // finish packet and send it
    SERIAL_PRINTLN("writeData()> End");
}

auto LoRaConnection::rxMode() -> void
{
    SERIAL_PRINTLN("rxMode()> Start");
    LoRa.enableInvertIQ(); // active invert I and Q signals
    LoRa.receive();        // set receive mode
    SERIAL_PRINTLN("rxMode()> End");
}

auto LoRaConnection::txMode() -> void
{
    SERIAL_PRINTLN("txMode()> Start");
    LoRa.idle();            // set standby mode
    LoRa.disableInvertIQ(); // normal mode
    SERIAL_PRINTLN("txMode()> End");
}

auto LoRaConnection::onTxDone() -> void
{
    SERIAL_PRINTLN("onTxDone()> Start");
    rxMode();
    _isWritable = true;
    SERIAL_PRINTLN("onTxDone()> End");
}

auto LoRaConnection::onReceive(int packetSize) -> void
{
    _isWritable = false;
    std::string message = "";

    while (LoRa.available())
    {
        message += (char)LoRa.read();
    }

    if (_onReceive)
    {
        _onReceive(message);
    }
    _isWritable = true;
}