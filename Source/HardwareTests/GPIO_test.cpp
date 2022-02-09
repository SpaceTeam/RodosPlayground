#include <rodos.h>


auto const baudrate = 115200;
const GPIO_PIN green  = GPIO_005;
HAL_GPIO greenLed(green);

/*
        GPIO_PIN        RODOS enum
        PA3             GPIO_003
        PA5             GPIO_005
        PA6             GPIO_006
        PA7             GPIO_007
        PC4             GPIO_036
        PC5             GPIO_037
        PB0             GPIO_016
        PB1             GPIO_017
        PB12            GPIO_028
        PB13            GPIO_029
        PB14            GPIO_030
        PB15            GPIO_031
        PC7             GPIO_039
        PC9             GPIO_041
        PA8             GPIO_008
        PA9             GPIO_009
        PA10            GPIO_010
        PA11            GPIO_011
        PA12            GPIO_012
        PA13            GPIO_013
        PA14            GPIO_014
        PA15            GPIO_015
        PC10            GPIO_042
        PC11            GPIO_043
        PC12            GPIO_044
        PD2             GPIO_050
        PB3             GPIO_019
        PB4             GPIO_020
        PB5             GPIO_021
        PB6             GPIO_022
        PB7             GPIO_023
        PB8             GPIO_024
        PB9             GPIO_025
        PC13            GPIO_045
        PC0             GPIO_032
        PC1             GPIO_033
        PC2             GPIO_034
        PC3             GPIO_035
        PA1             GPIO_001
        PA2             GPIO_002
        */

namespace rpg
{

class HelloWorld : public StaticThread<>
{
        void init() override {
                greenLed.init(/*isOutput=*/true, 1, 0);
                //huart1.init(baudrate);
                huart6.init(baudrate);
        }

        void run() override
        {
        auto toggle = true;

        TIME_LOOP(0, 2000 * MILLISECONDS)
        {
                greenLed.setPins(static_cast<uint32_t>(toggle));
                if(toggle) {
                        PRINTF("Hello World ! Toggle true \n");
                }
                else
                {
                        PRINTF("Hello World ! Toggle false \n");

                        uint8_t myText[19] = "Hello from uart6\r\n";
                        huart6.write(myText, sizeof(myText));

                }
                toggle = not toggle;
                }
        }
};

auto const helloWorld = HelloWorld();
}
