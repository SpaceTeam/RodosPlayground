#include <rodos.h>

#include <etl/string.h>

#include <string_view>


namespace rpg
{
// SPI1 = Flash SPI
constexpr auto spi1Sck = GPIO_005;   // PA5
constexpr auto spi1Miso = GPIO_006;  // PA6
constexpr auto spi1Mosi = GPIO_007;  // PA7
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
auto spi1 = HAL_SPI(SPI_IDX1, spi1Sck, spi1Miso, spi1Mosi);

// SPI2 = FRAM SPI
constexpr auto spi2Sck = GPIO_039;   // PC7
constexpr auto spi2Miso = GPIO_034;  // PC2
constexpr auto spi2Mosi = GPIO_035;  // PC3
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
auto spi2 = HAL_SPI(SPI_IDX2, spi2Sck, spi2Miso, spi2Mosi);

// SPI3 = COBC SPI
constexpr auto spi3Sck = GPIO_042;   // PC10
constexpr auto spi3Miso = GPIO_043;  // PC11
constexpr auto spi3Mosi = GPIO_044;  // PC12
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
auto spi3 = HAL_SPI(SPI_IDX3, spi3Sck, spi3Miso, spi3Mosi);


auto ChooseSpi(auto counter) -> auto &
{
  counter %= 3;
  switch(counter)
  {
    default:
    case 0:
      return spi1;
    case 1:
      return spi2;
    case 2:
      return spi3;
  }
}


class HelloSpi : public StaticThread<>
{
  void init() override
  {
    spi1.init();
    spi2.init();
    spi3.init();
  }

  void run() override
  {
    using std::operator""sv;

    auto counter = 0;
    TIME_LOOP(0, 1000 * MILLISECONDS)
    {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
      PRINTF("Writing to SPI%i...\n", counter + 1);

      constexpr auto message1 = "Hello from SPI1!"sv;
      constexpr auto message2 = "Hello from SPI2!"sv;
      constexpr auto message3 = "Hello from SPI3!"sv;
      auto message = [counter, message1, message2, message3]()
      {
        switch(counter)
        {
          default:
          case 0:
            return message1;
          case 1:
            return message2;
          case 2:
            return message3;
        }
      }();
      static_assert(std::size(message1) >= std::size(message2), "message1 must be the longest!");
      static_assert(std::size(message1) >= std::size(message3), "message1 must be the longest!");
      auto answer = etl::string<std::size(message1)>();
      answer.initialize_free_space();

      auto & spi = ChooseSpi(counter);
      auto nReceivedBytes =
        spi.writeRead(message.data(), message.size(), answer.data(), answer.capacity());
      answer.trim_to_terminator();

      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
      PRINTF("nReceivedBytes = %i\n", static_cast<int>(nReceivedBytes));
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
      PRINTF("answer = '%s'\n\n", answer.c_str());

      counter++;
      counter %= 3;
    }
  }
};


auto const helloSpi = HelloSpi();
}
