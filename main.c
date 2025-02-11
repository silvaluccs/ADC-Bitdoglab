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

static volatile bool desligar_leds = false;

bool repeating_timer_callback(struct repeating_timer *t); // prototipo da função para o timer
void gpio_irq_handler(uint gpio, uint32_t events); // prototipo da função para tratar a interrupção dos botoes

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

    gpio_set_irq_enabled_with_callback(pino_botao_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); // botao A
    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}


/*
* Função para tratar a interrupção dos botão a
*/
void gpio_irq_handler(uint gpio, uint32_t events)
{
    desligar_leds = !desligar_leds;
    
    if (desligar_leds) {

        pwm_set_gpio_level(pino_led_vermelho, 0);
        pwm_set_gpio_level(pino_led_azul, 0);
        gpio_put(pino_led_verde, 0);
    }

}


bool repeating_timer_callback(struct repeating_timer *t)
{
    // TODO: Implement this function
    return true;
}