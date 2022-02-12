#include <rodos.h>

constexpr RODOS::GPIO_PIN SPI1_MISO = GPIO_006;
constexpr RODOS::GPIO_PIN SPI1_MOSI = GPIO_007;
constexpr RODOS::GPIO_PIN SPI1_SCK = GPIO_005;

// constexpr RODOS::GPIO_PIN USER_LED_NUCLEO = GPIO_005;
RODOS::HAL_SPI SPI1(SPI_IDX1, SPI1_SCK, SPI1_MISO, SPI1_MOSI);

namespace rpg
{
class HelloWorld : public StaticThread<>
{
  void run() override
  {
    SPI1.init();

    TIME_LOOP(0, 2000 * MILLISECONDS)
    {
      char buf[20];
      // Short SPI1 MISO and MOSI Pins (PA6, PA7) and connect UART 2 to PC
      // Serial Terminal should constantly print (test -> STOP -> test -> STOP)
      // Check whether thats the correct SPI for the COBC HW
      SPI1.writeRead("test\n", 6, buf, 6);
      PRINTF(buf);
      PRINTF("STOP\r\n");
    }
  }
};

auto const helloWorld = HelloWorld();
}
