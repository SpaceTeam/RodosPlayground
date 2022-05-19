#include "Communication.hpp"
#include "Io.hpp"
#include "Utility.hpp"

#include <rodos.h>

namespace rpg
{
HAL_UART eduUart(UART_IDX1, eduUartTxPin, eduUartRxPin);

// Ask edu for data from sensors
class EduData : public StaticThread<>
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
            AT(NOW() + delay * MILLISECONDS);
            WriteTo(&eduUart, accelerationCommand);
            AT(NOW() + delay * MILLISECONDS);
            WriteTo(&eduUart, brightnessCommand);
        }
    }
} eduData;

}
