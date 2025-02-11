#include <stdio.h>
#include "pico/stdlib.h"
#include "ssd1306.h"
#include "hardware/pwm.h"
#include "setup.h"

const uint pino_led_vermelho = 13;
const uint pino_led_azul = 12;
const uint pino_led_verde = 11;
const uint pino_botao_a = 5;
const uint pino_botao_b = 6;
const uint pino_vrx = 27;
const uint pino_vry = 26;
const uint pino_botao_joystick = 22;

ssd1306_t display; // variavel de controle do display

int main()
{

    setup_led_pwm(pino_led_vermelho);
    setup_led_pwm(pino_led_azul);
    setup_led(pino_led_verde);
    setup_botoes(pino_botao_a, pino_botao_b);
    setup_joystick(pino_vrx, pino_vry, pino_botao_joystick);
    setup_display();
    init_display(&display);

    stdio_init_all();

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
