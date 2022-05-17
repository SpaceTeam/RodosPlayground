#include "Communication.hpp"
#include "Io.hpp"
#include "Util.hpp"

#include <rodos.h>

namespace rpg
{
HAL_UART eduUart(UART_IDX1, eduUartTxPin, eduUartRxPin);

// Ask edu for data from sensors
class EduData : public StaticThread<>
{
    void init() override
    {
        constexpr auto baudrate = 9600;
        eduUart.init(baudrate);
    }

    void run() override
    {
        constexpr auto temperatureCommand = "$51\n";
        constexpr auto accelerationCommand = "$52\n";
        constexpr auto brightnessCommand = "$53\n";

        TIME_LOOP(0, (beaconPeriod * MILLISECONDS).get())
        {
            WriteTo(&eduUart, temperatureCommand);
            WriteTo(&eduUart, accelerationCommand);
            WriteTo(&eduUart, brightnessCommand);
        }
    }
} eduData;

}
