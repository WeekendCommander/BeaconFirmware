#include "DeviceInfoPayload.h"

#define JSON_DEVICE_OBJECT_NAME "bat"
#define JSON_DEVICE_TYPE_NAME_VOL "vol"
#define JSON_DEVICE_TYPE_NAME_PCT "pct"
#define JSON_DEVICE_TYPE_NAME_CHG "chg"

auto DeviceInfoPayload::toJsonString() -> std::string
{
    StaticJsonDocument<100> object;

    object[JSON_DEVICE_OBJECT_NAME][JSON_DEVICE_TYPE_NAME_VOL] = this->vol;
    object[JSON_DEVICE_OBJECT_NAME][JSON_DEVICE_TYPE_NAME_PCT] = this->pct;
    object[JSON_DEVICE_OBJECT_NAME][JSON_DEVICE_TYPE_NAME_CHG] = this->chg;

    String output;
    serializeJson(object, output);
    return output.c_str();
}
DeviceInfoPayload::~DeviceInfoPayload()
{
    Serial.println("DeviceInfoPayload> ~DeviceInfoPayload()");
}