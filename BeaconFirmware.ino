#include <Arduino.h>
#include "./src/Beacon.h"


Beacon *beacon = new Beacon();

void setup()
{
    beacon->initBeacon();
}

void loop()
{
    beacon->tick();
}