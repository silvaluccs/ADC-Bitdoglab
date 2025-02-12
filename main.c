#include <stdio.h>
#include "pico/stdlib.h"
#include "ssd1306.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "setup.h"
#include <math.h>
#include "debouce.h"

const uint pino_led_vermelho = 13; // pino do led vermelho
const uint pino_led_azul = 12; // pino do led azul
const uint pino_led_verde = 11; // pino do led verde
const uint pino_botao_a = 5; // pino do botão A
const uint pino_botao_b = 6; // pino do botão B
const uint pino_vrx = 27; // pino do eixo X do joystick
const uint pino_vry = 26; // pino do eixo Y do joystick
const uint pino_botao_joystick = 22; // pino do botão do joystick

ssd1306_t display; // variavel de controle do display


typedef struct joystick_t {
    uint16_t vrx;
    uint16_t vry;
    bool botao;
} joystick_t; // estrutura para armazenar os dados do joystick

static joystick_t joystick; // variavel para armazenar os dados do joystick

static volatile bool desligar_leds = false; // variavel para controlar o estado dos leds

bool repeating_timer_callback(struct repeating_timer *t); // prototipo da função para o timer
void gpio_irq_handler(uint gpio, uint32_t events); // prototipo da função para tratar a interrupção dos botoes

struct repeating_timer timer;

int main()
{

    setup_led_pwm(pino_led_vermelho); // configuração do led vermelho
    setup_led_pwm(pino_led_azul); // configuração do led azul
    setup_led(pino_led_verde); // configuração do led verde
    setup_botoes(pino_botao_a, pino_botao_b); // configuração dos botões
    setup_joystick(pino_vrx, pino_vry, pino_botao_joystick); // configuração do joystick
    setup_display(); // configuração do display
    init_display(&display); // inicializa o display

    stdio_init_all(); // inicializa a comunicação serial

    add_repeating_timer_ms(100, repeating_timer_callback, &joystick, &timer); // timer para monitorar o estado do joystick

    gpio_set_irq_enabled_with_callback(pino_botao_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); // botao A para desligar os leds
    gpio_set_irq_enabled_with_callback(pino_botao_joystick, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); // botao A para desligar os leds


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

    static uint32_t  ultimo_tempo = 0; 

    if (!debouce(&ultimo_tempo)) { // verifica se o botão foi pressionado recentemente
        return;
    }


    // verifica qual botão foi pressionado
    if (gpio == pino_botao_joystick) {
        bool estado_led_verde = gpio_get(pino_led_verde);
        joystick.botao = !estado_led_verde;
        gpio_put(pino_led_verde, joystick.botao);
        return;
    }


    // desliga os leds se o botão A for pressionado

    desligar_leds = !desligar_leds; // inverte o estado dos leds
    
    if (desligar_leds) { // desliga os leds

        pwm_set_gpio_level(pino_led_vermelho, 0);
        pwm_set_gpio_level(pino_led_azul, 0);
        gpio_put(pino_led_verde, 0);
    }
    else {

        pwm_set_gpio_level(pino_led_vermelho, joystick.vry); // liga os leds com os valores do joystick
        pwm_set_gpio_level(pino_led_azul, joystick.vrx);
        gpio_put(pino_led_verde, joystick.botao);
    }

}


bool repeating_timer_callback(struct repeating_timer *t)
{
 
    if (desligar_leds) { // verifica se os leds estão desligados
        return true;
    }

    joystick_t *joystick_dados = (joystick_t *)t->user_data;

    adc_select_input(1);
    uint16_t vrx = fabs(adc_read() - 2048);
    joystick_dados->vrx = vrx > 200 ? vrx : 0; // Lê o valor do eixo X

    adc_select_input(0);
    uint16_t vry = fabs(adc_read() - 2048);
    joystick_dados->vry = vry > 200 ? vry : 0; // Lê o valor do eixo Y

    pwm_set_gpio_level(pino_led_vermelho, joystick_dados->vry); // liga os leds com os valores do joystick
    pwm_set_gpio_level(pino_led_azul, joystick_dados->vrx);

    return true;
}
