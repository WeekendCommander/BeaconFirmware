#include "PositionPlayload.h"

#define JSON_POSITION_OBJECT_NAME "cord"
#define JSON_POSITION_TYPE_NAME_LAT "lat"
#define JSON_POSITION_TYPE_NAME_LON "lon"
#define JSON_POSITION_TYPE_NAME_VLD "vld"
#define JSON_POSITION_TYPE_NAME_SAT "sat"
#define JSON_POSITION_TYPE_NAME_AZM "azm"
#define JSON_POSITION_TYPE_NAME_ALT "alt"

auto PositionPayload::toJsonString() -> std::string
{
    StaticJsonDocument<100> object;

    object[JSON_POSITION_OBJECT_NAME][JSON_POSITION_TYPE_NAME_LAT] = this->lat;
    object[JSON_POSITION_OBJECT_NAME][JSON_POSITION_TYPE_NAME_LON] = this->lon;
    object[JSON_POSITION_OBJECT_NAME][JSON_POSITION_TYPE_NAME_VLD] = this->vld;
    object[JSON_POSITION_OBJECT_NAME][JSON_POSITION_TYPE_NAME_SAT] = this->sat;
    object[JSON_POSITION_OBJECT_NAME][JSON_POSITION_TYPE_NAME_AZM] = this->azm;
    object[JSON_POSITION_OBJECT_NAME][JSON_POSITION_TYPE_NAME_ALT] = this->alt;
    String output;
    serializeJson(object, output);
    return output.c_str();
}

PositionPayload::~PositionPayload()
{
    Serial.println("PositionPayload> ~PositionPayload()");
}