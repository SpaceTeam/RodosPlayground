//! @file
//! @brief  A program for testing the two UARTs of the COBC.
//!
//! If you flash `HelloUart.bin` onto the COBC, the messages "Hello from UART1" and "Hello from
//! UART2" are alternately written every 500 ms to the COBC EDU UART and the COBC UCI UART,
//! respectively. Both UARTs use the same configuration: 115200 baud, 8 data bits, no parity, 1 stop
//! bit.


#include <rodos.h>

#include <string_view>


namespace rpg
{
constexpr auto pa10 = GPIO_010;
constexpr auto pa15 = GPIO_015;
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
