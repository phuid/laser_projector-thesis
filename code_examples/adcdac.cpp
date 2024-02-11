#include "ABE_ADCDACPi.h"

int main()
{

  ABElectronics_CPP_Libraries::ADCDACPi adcdac; // Definice třídy ADCDACPi, s jejímiž metodami budeme pracovat.

  if (adcdac.open_dac() != 1) // Inicializace SPI komunikace s D/A přvodníkem (DAC). Pokud se otevření nezdaří, program skončí.
  {
    return (1);
  }

  for (int i = 0; i < 4096; i++)
  {
    adcdac.set_dac_raw(i, 1); //nastavení hodnoty napětí na první kanál DAC, hodnota napětí bude rovna (i/4096 * Vref), kde Vref je referenční napětí DAC.
    adcdac.set_dac_raw(i, 2); //nastavení hodnoty napětí na druhý kanál DAC
  }

  adcdac.close_dac(); // Uzavření SPI komunikace s DAC.
  return (0);
}