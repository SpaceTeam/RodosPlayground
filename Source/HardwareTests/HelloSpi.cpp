#include <rodos.h>

#include <etl/string.h>

#include <string_view>


namespace rpg
{
constexpr auto spi1Miso = GPIO_006;
constexpr auto spi1Mosi = GPIO_007;
constexpr auto spi1Sck = GPIO_005;

auto spi1 = HAL_SPI(SPI_IDX1, spi1Sck, spi1Miso, spi1Mosi);


class HelloSpi : public StaticThread<>
{
  void init() override
  {
    spi1.init();
  }

  void run() override
  {
    using std::operator""sv;

    TIME_LOOP(0, 2000 * MILLISECONDS)
    {
      // Short SPI1 MISO and MOSI Pins (PA6, PA7) and connect UART 2 to PC
      // Serial Terminal should constantly print (test -> STOP -> test -> STOP)
      // TODO: Check whether thats the correct SPI for the COBC HW
      constexpr auto message = "Hello from SPI1!\n"sv;
      auto answer = etl::string<message.size()>();
      answer.initialize_free_space();
      auto nReceivedBytes =
        spi1.writeRead(message.data(), message.size(), answer.data(), answer.capacity());
      if(nReceivedBytes < 0)
      {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        PRINTF("Error during spi1.readWrite()! Return value is %i\n",
               static_cast<int>(nReceivedBytes));
      }
      else
      {
        answer.trim_to_terminator();
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        PRINTF("%s", answer.c_str());
      }
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
      PRINTF("Next attempt\n");
    }
  }
};

auto const helloSpi = HelloSpi();
}
