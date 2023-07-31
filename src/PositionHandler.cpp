#include "PositionHandler.h"
#include "Message.h"
#include "PositionPlayload.h"
#include "utils.h"

PositionHandler::PositionHandler()
{
    Serial1.begin(GPS_BAUD_RATE, GPS_SERIAL_CONFIG, GPS_RX_PIN, GPS_TX_PIN);
    index = 0;
    is_valid = false;
}

auto PositionHandler::updatePosition() -> void
{
    while (Serial1.available() > 0)
    {
        if (gps[index].encode(Serial1.read()))
        {
            index++;
            if (index >= POSITION_SMOOTHING)
            {
                is_valid = true;
                index = 0;
            }
        }
    }
}

auto PositionHandler::sendPositionMessage(Connection *c) -> void
{
    SERIAL_PRINTLN("PositionHandler::sendPositionMessage()> Start");
    if (!is_valid)
    {
        SERIAL_PRINTLN("PositionHandler::sendPositionMessage()> Not Valid!");
        return;
    }
    Message *m = new Message();
    m->sender = DEVICE_ID;
    m->type = MESSAGE_TYPE_LOCATION;
    PositionPayload *p = new PositionPayload();

    double lng = 0;
    double lat = 0;
    int altitude = 0;
    int is_valid_counter = 0;
    int satellites = 0;
    for (int i = 0; i <= POSITION_SMOOTHING; i++)
    {
        if (gps[i].location.isValid())
        {
            is_valid_counter++;

            lng += gps[i].location.lng();
            lat += gps[i].location.lat();
            altitude += gps[i].altitude.meters();
            satellites += gps[i].satellites.value();
        }
    }
    if (is_valid_counter > 5)
    {
        p->lat = lat / is_valid_counter;
        p->lon = lng / is_valid_counter;
        p->alt = altitude / is_valid_counter;
        p->sat = satellites / is_valid_counter;
        p->vld = true;
    }

    m->payload = p;

    SERIAL_PRINTLN("PositionHandler::sendPositionMessage()> Send Message");
    m->send(c);
    delete m;
    SERIAL_PRINTLN("PositionHandler::sendPositionMessage()> End");
}