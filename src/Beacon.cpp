#include "Beacon.h"

auto Beacon::initBeacon() -> bool
{
    if (!this->_is_valid)
    {

        START_SERIAL;

        SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);
        Wire.begin(I2C_SDA, I2C_SCL);

        this->powerManagement = new PowerManagement();
        delay(2000);

        this->messageHandler = new MessageHandler();
        this->connection = Connection::createLoRaConnection(LORA_FREQUENCY, messageHandler->onReceive);
        this->positionHandler = new PositionHandler();
        this->_is_valid = true;
    }
    return this->_is_valid;
}

auto Beacon::tick() -> void
{

    RUN_FUNCTION(TIMER_POSITION_UPDATE_ID, GPS_FREQUENZ, positionHandler->updatePosition());
    RUN_FUNCTION(TIMER_POSITION_MESSAGE_ID, POSITION_MESSAGE_INTERVALL, positionHandler->sendPositionMessage(this->connection));
    RUN_FUNCTION(TIMER_DEVICE_INFO_MESSAGE_ID, DEVICE_MESSAGE_INTERVALL, powerManagement->sendPowerManagementMessage(this->connection));
    delay(100);
}