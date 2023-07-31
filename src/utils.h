#pragma once
#include <unordered_map>
#include "Arduino.h"

#ifdef IS_DEBUG
#define START_SERIAL Serial.begin(115200)
#define SERIAL_PRINTLN(message) Serial.println(message)
#define SERIAL_PRINT(message) Serial.print(message)
#else
#define START_SERIAL
#define SERIAL_PRINTLN(message)
#define SERIAL_PRINT(message)
#endif

typedef struct TIMER
{
    unsigned long previousMillis;
    bool init = false;

    auto isInit() -> void
    {
        if (!init)
        {
            previousMillis = 0;
            init = true;
        }
    }

} TIMER_;

static std::unordered_map<uint8_t, TIMER_> m_timer;

static auto runEvery(uint8_t id, unsigned long interval) -> bool
{
    m_timer[id].isInit();
    unsigned long currentMillis = millis();
    
    if (currentMillis - m_timer[id].previousMillis >= interval)
    {
        m_timer[id].previousMillis = currentMillis;
        return true;
    }
    return false;
}

#define RUN_FUNCTION(id, intervall, function) \
    do                                        \
    {                                         \
        if (runEvery(id, intervall))          \
        {                                     \
            function;                         \
        }                                     \
    } while (0)