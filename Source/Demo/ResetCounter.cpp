#include <rodos.h>

#include <stm32f4xx_pwr.h>
#include <stm32f4xx_rtc.h>

namespace rpg
{
class HelloWorld : public StaticThread<>
{
  void init() override
  {
    // 1:  Enable the power controller APB1 interface
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    // 2: Enable access to RTC domain using the PWR_BackupAccessCmd() function.
    PWR_BackupAccessCmd(ENABLE);
  }

  void run() override
  {
    auto backupRegisterValue = RTC_ReadBackupRegister(RTC_BKP_DR0);
    ++backupRegisterValue;
    RTC_WriteBackupRegister(RTC_BKP_DR0, backupRegisterValue);
    PRINTF("Incremented reset counter\n");

    TIME_LOOP(0, 1500 * MILLISECONDS)
    {
      auto regValue = RTC_ReadBackupRegister(RTC_BKP_DR0);
      PRINTF("Successfully read from rtc_bkp_dr0\n");
      PRINTF("Value is %lu\n", regValue);
    }
  }
};

auto const helloWorld = HelloWorld();
}
