#include <stdio.h>
#include "pico/stdlib.h"
#include "ssd1306.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "setup.h"
#include <math.h>

const uint pino_led_vermelho = 13;
const uint pino_led_azul = 12;
const uint pino_led_verde = 11;
const uint pino_botao_a = 5;
const uint pino_botao_b = 6;
const uint pino_vrx = 27;
const uint pino_vry = 26;
const uint pino_botao_joystick = 22;

ssd1306_t display; // variavel de controle do display


typedef struct joystick_t {
    uint16_t vrx;
    uint16_t vry;
    bool botao;
} joystick_t;


static joystick_t joystick;

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

    struct repeating_timer timer; 
    add_repeating_timer_ms(100, repeating_timer_callback, &joystick, &timer); // timer para piscar os leds
   
    gpio_set_irq_enabled_with_callback(pino_botao_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); // botao A

    while (true) {
        printf("vrx: %d, vry: %d, botao: %d\n", joystick.vrx, joystick.vry, joystick.botao);
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
    else {
        pwm_set_gpio_level(pino_led_vermelho, joystick.vry);
        pwm_set_gpio_level(pino_led_azul, joystick.vrx);
        gpio_put(pino_led_verde, joystick.botao);
    }

}


bool repeating_timer_callback(struct repeating_timer *t)
{

    if (desligar_leds) {
        return true;
    }

    joystick_t *joystick_dados = (joystick_t *)t->user_data;

    adc_select_input(1);
    uint16_t vrx = fabs(adc_read() - 2048);
    joystick_dados->vrx = vrx > 200 ? vrx : 0; // Lê o valor do eixo X

    adc_select_input(0);
    uint16_t vry = fabs(adc_read() - 2048);
    joystick_dados->vry = vry > 200 ? vry : 0; // Lê o valor do eixo Y

    bool estado_led_verde = gpio_get(pino_led_verde);
    joystick_dados->botao = gpio_get(pino_botao_joystick) == 0 ? !estado_led_verde : estado_led_verde; // 0 indica que o botão está pressionado.

    pwm_set_gpio_level(pino_led_vermelho, joystick_dados->vry);
    pwm_set_gpio_level(pino_led_azul, joystick_dados->vrx);
 
    gpio_put(pino_led_verde, joystick_dados->botao);
    

    return true;
}