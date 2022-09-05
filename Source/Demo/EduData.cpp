#include "Beacon.hpp"
#include "Communication.hpp"
#include "Io.hpp"
#include "Utility.hpp"

#include <rodos_no_using_namespace.h>

namespace rpg
{
RODOS::HAL_UART eduUart(RODOS::UART_IDX1, eduUartTxPin, eduUartRxPin);

// Ask edu for data from sensors
class EduData : public RODOS::StaticThread<>
{
    void init() override
    {
        constexpr auto baudrate = 9'600;
        eduUart.init(baudrate);
    }

    void run() override
    {
        constexpr auto temperatureCommand = "$51\n";
        constexpr auto accelerationCommand = "$52\n";
        constexpr auto brightnessCommand = "$53\n";
        constexpr auto delay = 8;

        TIME_LOOP(0, beaconPeriod.get())
        {
            WriteTo(&eduUart, temperatureCommand);
            RODOS::AT(RODOS::NOW() + delay * RODOS::MILLISECONDS);
            WriteTo(&eduUart, accelerationCommand);
            RODOS::AT(RODOS::NOW() + delay * RODOS::MILLISECONDS);
            WriteTo(&eduUart, brightnessCommand);
        }
    }
} eduData;

}
