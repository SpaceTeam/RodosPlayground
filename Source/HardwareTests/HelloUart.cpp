#include <rodos.h>


auto const baudrate = 115200;
const GPIO_PIN green = GPIO_013;
HAL_GPIO greenLed(green);

// Tx, Rx, RTS, CTS

// UART 1 Board: flying wire from CN10 21 and CN10 33
// HAL_UART huart1(UART_IDX1, GPIO_009,GPIO_010);
// UART6 Board : flying wire from CN10/19 to TX, CN10/04 to Rx
// HAL_UART huart6(UART_IDX6, GPIO_038, GPIO_037);

/* For COBC (not tested yet)
        PA10 USART1 RX
        PA15 USART1 TX
*/
HAL_UART huart1(UART_IDX1, GPIO_015, GPIO_010);

namespace rpg
{
class HelloWorld : public StaticThread<>
{
  void init() override
  {
    greenLed.init(/*isOutput=*/true, 1, 0);
    huart1.init(baudrate);
    // huart6.init(baudrate);
  }

  void run() override
  {
    auto toggle = true;

    TIME_LOOP(0, 500 * MILLISECONDS)
    {
      greenLed.setPins(static_cast<uint32_t>(toggle));
      if(toggle)
      {
        PRINTF("Hello from uart2\n");
      }
      else
      {
        uint8_t myText[19] = "Hello from uart1\r\n";
        huart1.write(myText, sizeof(myText));
      }
      toggle = not toggle;
    }
  }
};

auto const helloWorld = HelloWorld();
}
