#include "Lib.hpp"

#include <rodos_no_using_namespace.h>

using RODOS::MILLISECONDS;

namespace rpg
{
class HelloWorld : public RODOS::StaticThread<>
{
    void run() override
    {
        auto const library = Library();
        auto toggle = true;

        TIME_LOOP(0, 500 * MILLISECONDS)
        {
            if(toggle)
            {
                RODOS::PRINTF("Hello World from %s!\n", library.name.data());
            }
            else
            {
                RODOS::PRINTF("Hello World from %s!\n", library.shortName.c_str());
            }
            toggle = not toggle;
        }
    }
};

auto const helloWorld = HelloWorld();
}
