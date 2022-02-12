#include <rodos.h>

#include <string_view>


namespace rpg
{
// UART 1 Board: flying wire from CN10 21 and CN10 33
// auto uart1 = HAL_UART(UART_IDX1, GPIO_009, GPIO_010);
// UART6 Board : flying wire from CN10/19 to TX, CN10/04 to Rx
// auto uart6 = HAL_UART(UART_IDX6, GPIO_038, GPIO_037);

// For COBC (not tested yet)
// PA10 USART1 RX
// PA15 USART1 TX
// TODO: See if this still works if uart1 is made const
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
auto uart1 = HAL_UART(UART_IDX1, GPIO_015, GPIO_010);


class HelloUart : public StaticThread<>
{
  void init() override
  {
    constexpr auto baudrate = 115'200;
    uart1.init(baudrate);
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
        uart1.write(myText.data(), myText.size());
      }
      toggle = not toggle;
    }
  }
};


auto const helloUart = HelloUart();
}
