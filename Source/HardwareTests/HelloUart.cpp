#include <rodos.h>

const GPIO_PIN green  = GPIO_005;
HAL_GPIO greenLed(green);

// Tx, Rx, RTS, CTS
// UART 1 Board: flying wire from CN10 21 and CN10 33
// HAL_UART huart1(UART_IDX1, GPIO_009,GPIO_010);
// UART6 Board : flying wire from CN10/19 to TX, CN10/04 to Rx
HAL_UART huart6(UART_IDX6, GPIO_038, GPIO_037);


namespace rpg
{

class HelloWorld : public StaticThread<>
{
        void init() override {
                greenLed.init(/*isOutput=*/true, 1, 0);
                //huart1.init(115200);
                huart6.init(115200);
        }

        void run() override
        {
        auto toggle = true;

        TIME_LOOP(0, 2000 * MILLISECONDS)
        {
                greenLed.setPins(toggle);
                if(toggle) {
                        PRINTF("Hello World ! Toggle true \n");
                }
                else
                {
                        PRINTF("Hello World ! Toggle true \n");

                        uint8_t myText[19] = "Hello from uart6\r\n";
                        huart6.write(myText, sizeof(myText));

                }
                toggle = not toggle;
                }
        }
};

auto const helloWorld = HelloWorld();
}
