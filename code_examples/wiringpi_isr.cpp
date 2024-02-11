#include <wiringPi.h>
#include <iostream>

constexpr int PIN = 22;

void my_callback() // Funkce, která se zavolá při změně levelu na pinu PIN
{
  std::cout << "Změna levelu na pinu " << PIN << " detekována" << std::endl;
}

int main()
{
  if (wiringPiSetup() < 0) // Inicializace wiringPi knihovny, při úspěchu vrací 0
  {
    std::cerr << "Nepodařilo se inicializovat wiringPi knihovnu" << std::endl;
    return 1;
  }
  pinMode(PIN, INPUT); // Nastavení pinu PIN pro vstup, to je nutné udělat před dalším používím pinu
  pullUpDnControl(PIN, PUD_UP); // Připojení pull-up rezistoru na pin PIN, aby byl level pinu HIGH, když není stisknuto tlačítko
  wiringPiISR(PIN, INT_EDGE_BOTH, *my_callback); // Nastavení callback funkce pro pin PIN. Ta je zavolána při změně levelu na pinu PIN

  while (true)
  {
    delay(1000);
  }
  return 0;
}
