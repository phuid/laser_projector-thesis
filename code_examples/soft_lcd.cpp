#include <stdio.h>
#include <unistd.h>
#include "soft_lcd.h"

int main()
{
  if (geteuid() != 0) // Ověření, zda je program spuštěn s administrátorskými právy, kdyby nebyl, mohl by mít systém problém s nastavením PWM podsvícení
  {
    printf("This program must be run as root.\n");
    return 1;
  }

  lcd_t *lcd = lcd_create(23, 24, 0x3f, 2); // Inicializace komunikace s I2C převodníkem na LCD (argumenty (číslo pinu s připojením SCL, číslo pinu s připojením SDA, I2C adresa zařízení, počet řádků LCD))
  if (lcd == NULL)
  { // Funkce lcd_create vrací NULL, pokud se inicializace nezdařila
    printf("LCD: Cannot initialize\n");
    return 1;
  }

  char battery[] = {// definice vlastního znaku
                    0b01110,
                    0b10001,
                    0b10001,
                    0b10001,
                    0b10001,
                    0b10001,
                    0b10001,
                    0b11111};

  lcd_create_char(lcd, 1, battery); // Odeslání vlastního znaku do LCD, druhý argument úrčuje, jaký ascii kód znak nahrazuje

  lcd_clear(lcd); // Vymazání obsahu LCD

  lcd_print(lcd, "Stav baterie \01"); // Vypsání prvního řádku

  for (int i = 100; i >= 0; i--)
  {
    lcd_pos(lcd, 1, 0);         // Nastavení pozice kurzoru na začátek druhého řádku
    lcd_printf(lcd, "%d%%", i); // Vypsání hodnoty i v procentech
    lcd_backlight_dim(lcd, static_cast<float>(i) / 100.f); // Nastavení jasu podsvícení LCD
    usleep(1000); // Čekání po dobu jedné ms
  }

  return 0;
}