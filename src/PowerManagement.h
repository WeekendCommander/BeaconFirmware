#pragma once
#include <XPowersLib.h>
#include "utils.h"
#include "Connection.h"

#ifndef PMU_WIRE_PORT
#define PMU_WIRE_PORT Wire
#endif

class PowerManagement
{
public:
    PowerManagement();

    auto sendPowerManagementMessage(Connection *c) -> void;

    static auto setPmuFlag() -> void
    {
        bool _pmuInterrupt = true;
    }

private:
    XPowersLibInterface *_PMU = NULL;
};