#include "PowerManagement.h"
#include "Message.h"
#include "Configuration.h"
#include "DeviceInfoPayload.h"

auto PowerManagement::sendPowerManagementMessage(Connection *c) -> void
{
    Message *m = new Message();
    m->sender = DEVICE_ID;
    m->type = MESSAGE_TYPE_DEVICE_INFO;
    DeviceInfoPayload *p = new DeviceInfoPayload();
    p->vol = _PMU->getBattVoltage();
    p->pct = _PMU->getBatteryPercent();
    p->chg = _PMU->isCharging();
    m->payload = p;
    m->send(c);
    delete m;
}

PowerManagement::PowerManagement()
{
    if (!_PMU)
    {
        _PMU = new XPowersAXP2101(PMU_WIRE_PORT);
        if (!_PMU->init())
        {
            SERIAL_PRINTLN("Warning: Failed to find AXP2101 power management");
            delete _PMU;
            _PMU = NULL;
        }
        else
        {
            SERIAL_PRINTLN("AXP2101 _PMU init succeeded, using AXP2101 _PMU");
        }
    }

    if (!_PMU)
    {
        SERIAL_PRINTLN("PMU FAILED");
        return;
    }

    _PMU->setChargingLedMode(XPOWERS_CHG_LED_BLINK_1HZ);

    pinMode(PMU_IRQ, INPUT_PULLUP);
    attachInterrupt(PMU_IRQ, setPmuFlag, FALLING);

    if (_PMU->getChipModel() == XPOWERS_AXP2101)
    {
        // Unuse power channel
        _PMU->disablePowerOutput(XPOWERS_DCDC2);
        _PMU->disablePowerOutput(XPOWERS_DCDC3);
        _PMU->disablePowerOutput(XPOWERS_DCDC4);
        _PMU->disablePowerOutput(XPOWERS_DCDC5);
        _PMU->disablePowerOutput(XPOWERS_ALDO1);
        _PMU->disablePowerOutput(XPOWERS_ALDO4);
        _PMU->disablePowerOutput(XPOWERS_BLDO1);
        _PMU->disablePowerOutput(XPOWERS_BLDO2);
        _PMU->disablePowerOutput(XPOWERS_DLDO1);
        _PMU->disablePowerOutput(XPOWERS_DLDO2);

        // GNSS RTC PowerVDD 3300mV
        _PMU->setPowerChannelVoltage(XPOWERS_VBACKUP, 3300);
        _PMU->enablePowerOutput(XPOWERS_VBACKUP);

        // ESP32 VDD 3300mV
        //  ! No need to set, automatically open , Don't close it
        //  _PMU->setPowerChannelVoltage(XPOWERS_DCDC1, 3300);
        //  _PMU->setProtectedChannel(XPOWERS_DCDC1);
        _PMU->setProtectedChannel(XPOWERS_DCDC1);

        // LoRa VDD 3300mV
        _PMU->setPowerChannelVoltage(XPOWERS_ALDO2, 3300);
        _PMU->enablePowerOutput(XPOWERS_ALDO2);

        // GNSS VDD 3300mV
        _PMU->setPowerChannelVoltage(XPOWERS_ALDO3, 3300);
        _PMU->enablePowerOutput(XPOWERS_ALDO3);
    }
    else
    {
        SERIAL_PRINTLN("PMU FAILED");
    }

    _PMU->enableSystemVoltageMeasure();
    _PMU->enableVbusVoltageMeasure();
    _PMU->enableBattVoltageMeasure();
    // It is necessary to disable the detection function of the TS pin on the board
    // without the battery temperature detection function, otherwise it will cause abnormal charging
    _PMU->disableTSPinMeasure();

    Serial.printf("=========================================\n");
    if (_PMU->isChannelAvailable(XPOWERS_DCDC1))
    {
        Serial.printf("DC1  : %s   Voltage: %04u mV \n", _PMU->isPowerChannelEnable(XPOWERS_DCDC1) ? "+" : "-", _PMU->getPowerChannelVoltage(XPOWERS_DCDC1));
    }
    if (_PMU->isChannelAvailable(XPOWERS_DCDC2))
    {
        Serial.printf("DC2  : %s   Voltage: %04u mV \n", _PMU->isPowerChannelEnable(XPOWERS_DCDC2) ? "+" : "-", _PMU->getPowerChannelVoltage(XPOWERS_DCDC2));
    }
    if (_PMU->isChannelAvailable(XPOWERS_DCDC3))
    {
        Serial.printf("DC3  : %s   Voltage: %04u mV \n", _PMU->isPowerChannelEnable(XPOWERS_DCDC3) ? "+" : "-", _PMU->getPowerChannelVoltage(XPOWERS_DCDC3));
    }
    if (_PMU->isChannelAvailable(XPOWERS_DCDC4))
    {
        Serial.printf("DC4  : %s   Voltage: %04u mV \n", _PMU->isPowerChannelEnable(XPOWERS_DCDC4) ? "+" : "-", _PMU->getPowerChannelVoltage(XPOWERS_DCDC4));
    }
    if (_PMU->isChannelAvailable(XPOWERS_DCDC5))
    {
        Serial.printf("DC5  : %s   Voltage: %04u mV \n", _PMU->isPowerChannelEnable(XPOWERS_DCDC5) ? "+" : "-", _PMU->getPowerChannelVoltage(XPOWERS_DCDC5));
    }
    if (_PMU->isChannelAvailable(XPOWERS_LDO2))
    {
        Serial.printf("LDO2 : %s   Voltage: %04u mV \n", _PMU->isPowerChannelEnable(XPOWERS_LDO2) ? "+" : "-", _PMU->getPowerChannelVoltage(XPOWERS_LDO2));
    }
    if (_PMU->isChannelAvailable(XPOWERS_LDO3))
    {
        Serial.printf("LDO3 : %s   Voltage: %04u mV \n", _PMU->isPowerChannelEnable(XPOWERS_LDO3) ? "+" : "-", _PMU->getPowerChannelVoltage(XPOWERS_LDO3));
    }
    if (_PMU->isChannelAvailable(XPOWERS_ALDO1))
    {
        Serial.printf("ALDO1: %s   Voltage: %04u mV \n", _PMU->isPowerChannelEnable(XPOWERS_ALDO1) ? "+" : "-", _PMU->getPowerChannelVoltage(XPOWERS_ALDO1));
    }
    if (_PMU->isChannelAvailable(XPOWERS_ALDO2))
    {
        Serial.printf("ALDO2: %s   Voltage: %04u mV \n", _PMU->isPowerChannelEnable(XPOWERS_ALDO2) ? "+" : "-", _PMU->getPowerChannelVoltage(XPOWERS_ALDO2));
    }
    if (_PMU->isChannelAvailable(XPOWERS_ALDO3))
    {
        Serial.printf("ALDO3: %s   Voltage: %04u mV \n", _PMU->isPowerChannelEnable(XPOWERS_ALDO3) ? "+" : "-", _PMU->getPowerChannelVoltage(XPOWERS_ALDO3));
    }
    if (_PMU->isChannelAvailable(XPOWERS_ALDO4))
    {
        Serial.printf("ALDO4: %s   Voltage: %04u mV \n", _PMU->isPowerChannelEnable(XPOWERS_ALDO4) ? "+" : "-", _PMU->getPowerChannelVoltage(XPOWERS_ALDO4));
    }
    if (_PMU->isChannelAvailable(XPOWERS_BLDO1))
    {
        Serial.printf("BLDO1: %s   Voltage: %04u mV \n", _PMU->isPowerChannelEnable(XPOWERS_BLDO1) ? "+" : "-", _PMU->getPowerChannelVoltage(XPOWERS_BLDO1));
    }
    if (_PMU->isChannelAvailable(XPOWERS_BLDO2))
    {
        Serial.printf("BLDO2: %s   Voltage: %04u mV \n", _PMU->isPowerChannelEnable(XPOWERS_BLDO2) ? "+" : "-", _PMU->getPowerChannelVoltage(XPOWERS_BLDO2));
    }
    Serial.printf("=========================================\n");

    // Set the time of pressing the button to turn off
    _PMU->setPowerKeyPressOffTime(XPOWERS_POWEROFF_4S);
    uint8_t opt = _PMU->getPowerKeyPressOffTime();
    Serial.print("PowerKeyPressOffTime:");
    switch (opt)
    {
    case XPOWERS_POWEROFF_4S:
        SERIAL_PRINTLN("4 Second");
        break;
    case XPOWERS_POWEROFF_6S:
        SERIAL_PRINTLN("6 Second");
        break;
    case XPOWERS_POWEROFF_8S:
        SERIAL_PRINTLN("8 Second");
        break;
    case XPOWERS_POWEROFF_10S:
        SERIAL_PRINTLN("10 Second");
        break;
    default:
        break;
    }
}