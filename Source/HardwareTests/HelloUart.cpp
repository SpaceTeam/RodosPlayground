#include <rodos.h>

#include <string_view>


namespace rpg
{
constexpr auto pa10 = GPIO_010;
constexpr auto pa15 = GPIO_015;
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
auto eduUart = HAL_UART(UART_IDX1, pa15, pa10);


class HelloUart : public StaticThread<>
{
  void init() override
  {
    constexpr auto baudrate = 115'200;
    eduUart.init(baudrate);
  }

  void run() override
  {
    using std::operator""sv;

    auto toggle = true;

    TIME_LOOP(0, 500 * MILLISECONDS)
    {
      if(toggle)
      {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        PRINTF("Hello from UART2\n");
      }
      else
      {
        auto myText = "Hello from UART1\n"sv;
        eduUart.write(myText.data(), myText.size());
      }
      toggle = not toggle;
    }
  }
};


auto const helloUart = HelloUart();
}
