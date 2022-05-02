//! @file
//! @brief  A program for testing most GPIO pins of the COBC.
//!
//! If you flash `HelloGpio.bin` onto the COBC, all 40 rpg::pinsToTest will be toggeled with a
//! frequency of 5 Hz.


#include "Io.hpp"

#include <type_safe/types.hpp>

#include <rodos.h>

#include <array>


namespace rpg
{
auto pinsToTest = std::to_array<HAL_GPIO>(
    {pa1, pa2, pa3, pa5, pa6, pa7, pa8, pa9,  pa10, pa11, pa12, pa13, pa14, pa15,
     pb0, pb1, pb3, pb4, pb5, pb6, pb7, pb8,  pb9,  pb12, pb13, pb14, pb15, pc0,
     pc1, pc2, pc3, pc4, pc5, pc7, pc9, pc10, pc11, pc12, pc13, pd2});


class HelloGpio : public StaticThread<>
{
    void init() override
    {
        for(auto & pin : pinsToTest)
        {
            pin.init(/*isOutput=*/true, 1, 0);
        }
    }

    void run() override
    {
        type_safe::bool_t toggle = true;

        TIME_LOOP(0, 100 * MILLISECONDS)
        {
            for(auto & i : pinsToTest)
            {
                i.setPins(static_cast<uint32_t>(bool(toggle)));
            }
            toggle = not toggle;
        }
    }
};


auto const helloGpio = HelloGpio();
}
