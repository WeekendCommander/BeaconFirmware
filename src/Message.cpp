#include "Message.h"
#include <sstream>
#include "utils.h"

auto Message::send(Connection *c) -> void
{
    SERIAL_PRINTLN("Message::send> Start");
    std::stringstream ss;
    ss << this->rc << ";";
    ss << this->receiver << ";";
    ss << this->sender << ";";
    ss << this->type << ";";
    ss << this->payload->toJsonString();
    c->writeData(ss.str());
}