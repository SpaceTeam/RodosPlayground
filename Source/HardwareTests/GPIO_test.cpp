#include <rodos.h>


/*
        GPIO_PIN        RODOS enum
        PA1             GPIO_001
        PA2             GPIO_002
        PA3             GPIO_003
        PA5             GPIO_005
        PA6             GPIO_006
        PA7             GPIO_007
        PA8             GPIO_008
        PA9             GPIO_009
        PA10            GPIO_010
        PA11            GPIO_011
        PA12            GPIO_012
        PA13            GPIO_013
        PA14            GPIO_014
        PA15            GPIO_015
        PB0             GPIO_016
        PB1             GPIO_017
        PB3             GPIO_019
        PB4             GPIO_020
        PB5             GPIO_021
        PB6             GPIO_022
        PB7             GPIO_023
        PB8             GPIO_024
        PB9             GPIO_025
        PB12            GPIO_028
        PB13            GPIO_029
        PB14            GPIO_030
        PB15            GPIO_031
        PC0             GPIO_032
        PC1             GPIO_033
        PC2             GPIO_034
        PC3             GPIO_035
        PC4             GPIO_036
        PC5             GPIO_037
        PC7             GPIO_039
        PC10            GPIO_042
        PC11            GPIO_043
        PC12            GPIO_044
        PC13            GPIO_045
        PC9             GPIO_041
        PD2             GPIO_050
        */

HAL_GPIO PA3(GPIO_003);
HAL_GPIO PA5(GPIO_005);
HAL_GPIO PA6(GPIO_006);
HAL_GPIO PA7(GPIO_007);
HAL_GPIO PC4(GPIO_036);
HAL_GPIO PC5(GPIO_037);
HAL_GPIO PB0(GPIO_016);
HAL_GPIO PB1(GPIO_017);
HAL_GPIO PB12(GPIO_028);
HAL_GPIO PB13(GPIO_029);
HAL_GPIO PB14(GPIO_030);
HAL_GPIO PB15(GPIO_031);
HAL_GPIO PC7(GPIO_039);
HAL_GPIO PC9(GPIO_041);
HAL_GPIO PA8(GPIO_008);
HAL_GPIO PA9(GPIO_009);
HAL_GPIO PA10(GPIO_010);
HAL_GPIO PA11(GPIO_011);
HAL_GPIO PA12(GPIO_012);
HAL_GPIO PA13(GPIO_013);
HAL_GPIO PA14(GPIO_014);
HAL_GPIO PA15(GPIO_015);
HAL_GPIO PC10(GPIO_042);
HAL_GPIO PC11(GPIO_043);
HAL_GPIO PC12(GPIO_044);
HAL_GPIO PD2(GPIO_050);
HAL_GPIO PB3(GPIO_019);
HAL_GPIO PB4(GPIO_020);
HAL_GPIO PB5(GPIO_021);
HAL_GPIO PB6(GPIO_022);
HAL_GPIO PB7(GPIO_023);
HAL_GPIO PB8(GPIO_024);
HAL_GPIO PB9(GPIO_025);
HAL_GPIO PC13(GPIO_045);
HAL_GPIO PC0(GPIO_032);
HAL_GPIO PC1(GPIO_033);
HAL_GPIO PC2(GPIO_034);
HAL_GPIO PC3(GPIO_035);
HAL_GPIO PA1(GPIO_001);
HAL_GPIO PA2(GPIO_002);

const int nbPinsToTest = 40;
HAL_GPIO  pinsToTest[nbPinsToTest] = {PA3,PA5,PA6,PA7,PC4,PC5,PB0,PB1,PB12,PB13,PB14,PB15,PC7,PC9,PA8,PA9,PA10,PA11,PA12,PA13,PA14,PA15,PC10,PC11,PC12,PD2,PB3,PB4,PB5,PB6,PB7,PB8,PB9,PC13,PC0,PC1,PC2,PC3,PA1,PA2};



namespace rpg
{

class HelloWorld : public StaticThread<>
{
        void init() override {
                for (auto & i : pinsToTest) {
                        i.init(/*isOutput=*/true, 1, 0);
                }
        }

        void run() override
        {
        auto toggle = true;

        TIME_LOOP(0, 500 * MILLISECONDS)
        {
                for (auto & i : pinsToTest) {
                        i.setPins(static_cast<uint32_t>(toggle));
                }

                toggle = not toggle;

                }
        }
};

auto const helloWorld = HelloWorld();
}
