#include <rodos.h>

#include <etl/string.h>

#include <string_view>


namespace rpg
{
// SPI1 = Flash SPI
constexpr auto spi1Sck = GPIO_005;   // PA5
constexpr auto spi1Miso = GPIO_006;  // PA6
constexpr auto spi1Mosi = GPIO_007;  // PA7
// SPI2 = FRAM SPI
constexpr auto spi2Sck = GPIO_039;   // PC7
constexpr auto spi2Miso = GPIO_034;  // PC2
constexpr auto spi2Mosi = GPIO_035;  // PC3
// SPI3 = COBC SPI
constexpr auto spi3Sck = GPIO_042;   // PC10
constexpr auto spi3Miso = GPIO_043;  // PC11
constexpr auto spi3Mosi = GPIO_044;  // PC12
// SPI4 = RF SPI
constexpr auto spi4Sck = GPIO_029;   // PB13
constexpr auto spi4Miso = GPIO_011;  // PA11
constexpr auto spi4Mosi = GPIO_001;  // PA1

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
auto spis = std::array{
  HAL_SPI(SPI_IDX1, spi1Sck, spi1Miso, spi1Mosi),
  HAL_SPI(SPI_IDX2, spi2Sck, spi2Miso, spi2Mosi),
  HAL_SPI(SPI_IDX3, spi3Sck, spi3Miso, spi3Mosi),
  HAL_SPI(SPI_IDX4, spi4Sck, spi4Miso, spi4Mosi)
};


class HelloSpi : public StaticThread<>
{
  void init() override
  {
    for(auto & spi : spis)
    {
      spi.init();
    }
  }

  void run() override
  {
    using std::operator""sv;

    constexpr auto messages = std::array{
      "Hello from SPI1!"sv,
      "Hello from SPI2!"sv,
      "Hello from SPI3!"sv,
      "Hello from SPI4!"sv
      };
    static_assert(std::size(spis) == std::size(messages));

    size_t i = 0;
    TIME_LOOP(0, 500 * MILLISECONDS)
    {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
      PRINTF("Writing to SPI%i\n", static_cast<int>(i + 1));

      auto answer = etl::string<std::size(messages[0])>();
      answer.initialize_free_space();
      auto nReceivedBytes = spis[i].writeRead(
        std::data(messages[i]), std::size(messages[i]), std::data(answer), answer.capacity());
      answer.trim_to_terminator();

      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
      PRINTF("nReceivedBytes = %i\n", static_cast<int>(nReceivedBytes));
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
      PRINTF("answer = '%s'\n\n", answer.c_str());

      i++;
      i %= std::size(spis);
    }
  }
};


auto const helloSpi = HelloSpi();
}
