#include <pigpio.h>
#include <iostream>

constexpr int PIN = 17;

int main()
{
  if (gpioInitialise() < 0) // Inicializace knihovny pigpio. Pokud se inicializace nezdaří, program skončí. Funkce musí být zavolána před použitím dalších funkcí knihovny pigpio.
  {
    std::cout << "Nepodařilo se inicializovat knihovnu" << std::endl;
    return 1;
  }

  gpioSetMode(PIN, PI_OUTPUT); // Nastavení pinu 17 pro výstup.
  gpioSetPullUpDown(PIN, PI_PUD_DOWN); // Připojení pull-down rezistoru pro pin 17.
  gpioSetPWMfrequency(PIN, 40000); // Nastavení frekvence PWM na 40 kHz (maximální hodnotu).
  gpioSetPWMrange(PIN, 255); // Nastavení rozsahu PWM na 255.
  gpioPWM(PIN, 127); // Nastavení střídy PWM na 50 %.

  gpioDelay(1000000); // 1 sekunda čekání.

  gpioWrite(PIN, 0); // Vypnutí laseru.
  gpioTerminate(); //deinitializace knihovny pigpio
}