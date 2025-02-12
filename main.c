#include <stdio.h>
#include "pico/stdlib.h"
#include "ssd1306.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "setup.h"
#include <math.h>
#include "debouce.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "ssd1306.h"
#include "font.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "setup.h"
#include <math.h>
#include "debouce.h"
#include "hardware/i2c.h"

const uint pino_led_vermelho = 13; // pino do led vermelho
const uint pino_led_azul = 12; // pino do led azul
const uint pino_led_verde = 11; // pino do led verde
const uint pino_botao_a = 5; // pino do botão A
const uint pino_botao_b = 6; // pino do botão B
const uint pino_vrx = 27; // pino do eixo X do joystick
const uint pino_vry = 26; // pino do eixo Y do joystick
const uint pino_botao_joystick = 22; // pino do botão do joystick


typedef struct joystick_t {
    uint16_t vrx;
    uint16_t vry;
    bool botao;
} joystick_t; // estrutura para armazenar os dados do joystick


typedef struct posicao {
    int x;
    int y;
} Posicao;

static Posicao posicao;

static joystick_t joystick; // variavel para armazenar os dados do joystick


static volatile bool desligar_leds = false; // variavel para controlar o estado dos leds

bool repeating_timer_callback_joystick(struct repeating_timer *t); // prototipo da função para o timer
bool repeating_timer_callback_display(struct repeating_timer *t); // prototipo da função para o timer

void gpio_irq_handler(uint gpio, uint32_t events); // prototipo da função para tratar a interrupção dos botoes


void mapear_valores_display(uint16_t x, uint16_t y, Posicao *posicao);


ssd1306_t ssd; // variavel de controle do display


int main()
{

    setup_led_pwm(pino_led_vermelho); // configuração do led vermelho
    setup_led_pwm(pino_led_azul); // configuração do led azul
    setup_led(pino_led_verde); // configuração do led verde
    setup_botoes(pino_botao_a, pino_botao_b); // configuração dos botões
    setup_joystick(pino_vrx, pino_vry, pino_botao_joystick); // configuração do joystick
   
    setup_display(); // configuração do display
    init_display(&ssd); // inicializa o display

    stdio_init_all(); // inicializa a comunicação serial

    struct repeating_timer timer_joystick; // timer para monitorar o estado do joystick
    add_repeating_timer_ms(100, repeating_timer_callback_joystick, &joystick, &timer_joystick); // timer para monitorar o estado do joystick

    struct repeating_timer timer_display; // timer para monitorar o estado do display
    add_repeating_timer_ms(100, repeating_timer_callback_display, NULL, &timer_display); // timer para monitorar o estado do display

    gpio_set_irq_enabled_with_callback(pino_botao_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); // botao A para desligar os leds
    gpio_set_irq_enabled_with_callback(pino_botao_joystick, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); // botao A para desligar os leds

    while (true) {
        tight_loop_contents();
    }
}


/*
* Função para tratar a interrupção dos botões
*/
void gpio_irq_handler(uint gpio, uint32_t events)
{
    static uint32_t ultimo_tempo = 0;

    if (!debouce(&ultimo_tempo)) {
        return;
    }

    // caso o botao do joystick seja pressionado, inverte o estado do botao
    if (gpio == pino_botao_joystick && !desligar_leds) {
        joystick.botao = !gpio_get(pino_led_verde);
        gpio_put(pino_led_verde, joystick.botao);
        return;
    }
    // caso o botao A seja pressionado, inverte o estado dos leds
    else if (gpio == pino_botao_a) {
        
        desligar_leds = !desligar_leds;

        pwm_set_gpio_level(pino_led_vermelho, desligar_leds ? 0 : joystick.vry);
        pwm_set_gpio_level(pino_led_azul, desligar_leds ? 0 : joystick.vrx);
        gpio_put(pino_led_verde, desligar_leds ? 0 : joystick.botao);
        return;
    }
}


/*
* Função para atualizar o display
*/
bool repeating_timer_callback_display(struct repeating_timer *t)
{

    bool ativar_borda = joystick.botao; // verifica se o botao do joystick foi pressionado
    controle_quadrado(&ssd, posicao.x, posicao.y, ativar_borda); // desenha o botao na posição do joystick

    return true;
}


/*
* Função para monitorar o estado do joystick
*/
bool repeating_timer_callback_joystick(struct repeating_timer *t)
{

    joystick_t *joystick_dados = (joystick_t *)t->user_data;

    adc_select_input(1);

    uint16_t vrx_original = adc_read();

    uint16_t vrx = fabs(vrx_original - 2048);

    joystick_dados->vrx = vrx > 200 ? vrx : 0; // Lê o valor do eixo X

    adc_select_input(0);

    uint16_t vry_original = adc_read();


    uint16_t vry = fabs(vry_original - 2048);
    joystick_dados->vry = vry > 200 ? vry : 0; // Lê o valor do eixo Y

    mapear_valores_display(vrx_original,  vry_original, &posicao);

    if (desligar_leds) { // verifica se os leds estão desligados
        return true;
    }

    pwm_set_gpio_level(pino_led_vermelho, joystick_dados->vry); // liga os leds com os valores do joystick
    pwm_set_gpio_level(pino_led_azul, joystick_dados->vrx);

    return true;
}


/*
* Função para mapear os valores do joystick para o display
*/
void mapear_valores_display(uint16_t x, uint16_t y, Posicao *posicao) {

    uint limite_x = joystick.botao ? 118 : 120;
    uint limite_y = joystick.botao ? 52 : 55;

    posicao->x = (x *limite_x) / 4096;
    posicao->y = (limite_y - (y * limite_y) / 4096);

    if (joystick.botao && posicao->x < 3) {
        posicao->x += 3;
    }
    
    if (joystick.botao && posicao->y < 4) {
        posicao->y = 4;
    }

}
