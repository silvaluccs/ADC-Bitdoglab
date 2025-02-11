#include <stdio.h>
#include "pico/stdlib.h"
#include "ssd1603.h"
#include "setup.h"

const uint pino_led_vermelho = 13;
const uint pino_led_azul = 12;
const uint pino_led_verde = 11;
const uint pino_botao_a = 5;
const uint pino_botao_b = 6;
const uint pino_vrx = 27;
const uint pino_vry = 26;
const uint pino_botao_joystick = 22;

ssd1603 display;

int main()
{
    stdio_init_all();

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
