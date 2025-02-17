#include <stdio.h>
#include "pico/stdlib.h"
#include "ssd1306.h"
#include "hardware/pwm.h"
#include "setup.h"
#include "debouce.h"
#include "joystick.h"

// definições dos pinos

const uint pino_led_vermelho = 13; // pino do led vermelho
const uint pino_led_azul = 12; // pino do led azul
const uint pino_led_verde = 11; // pino do led verde
const uint pino_botao_a = 5; // pino do botão A
const uint pino_botao_b = 6; // pino do botão B
const uint pino_vrx = 27; // pino do eixo X do joystick
const uint pino_vry = 26; // pino do eixo Y do joystick
const uint pino_botao_joystick = 22; // pino do botão do joystick


// variaveis de controle

static Posicao_display posicao_display; // variavel para armazenar a posição do joystick no display
static Dados_leds dados_leds; // variavel para armazenar os dados do joystick
static volatile bool desligar_leds = false; // variavel para controlar o estado dos leds

// prototipos das funções

bool repeating_timer_callback_joystick(struct repeating_timer *t); // prototipo da função para o timer
bool repeating_timer_callback_display(struct repeating_timer *t); // prototipo da função para o timer

void gpio_irq_handler(uint gpio, uint32_t events); // prototipo da função para tratar a interrupção dos botoes


ssd1306_t ssd; // variavel de controle do display


int main()
{

    stdio_init_all(); // inicializa a comunicação serial

    setup_led_pwm(pino_led_vermelho); // configuração do led vermelho
    setup_led_pwm(pino_led_azul); // configuração do led azul
    setup_led(pino_led_verde); // configuração do led verde
    setup_botoes(pino_botao_a, pino_botao_b); // configuração dos botões
    setup_joystick(pino_vrx, pino_vry, pino_botao_joystick); // configuração do joystick
    dados_leds.estado_led_verde = false; // inicializa o estado do botao do joystick

    setup_display(); // configuração do display
    init_display(&ssd); // inicializa o display

    struct repeating_timer timer_joystick; // timer para monitorar o estado do joystick
    add_repeating_timer_ms(100, repeating_timer_callback_joystick, NULL, &timer_joystick); // timer para monitorar o estado do joystick

    struct repeating_timer timer_display; // timer para monitorar o estado do display
    add_repeating_timer_ms(100, repeating_timer_callback_display, NULL, &timer_display); // timer para monitorar o estado do display

    gpio_set_irq_enabled_with_callback(pino_botao_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); // interrupção do botao A
    gpio_set_irq_enabled_with_callback(pino_botao_joystick, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); // interrupção do botao do joystick

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
    if (gpio == pino_botao_joystick) {

        dados_leds.estado_led_verde = !dados_leds.estado_led_verde;

        gpio_put(pino_led_verde, dados_leds.estado_led_verde);
        

        return;
    }

    // caso o botao A seja pressionado, inverte o estado dos leds
    else if (gpio == pino_botao_a) {
        
        desligar_leds = !desligar_leds;

        // desliga os leds ou liga com os valores do joystick
        pwm_set_gpio_level(pino_led_vermelho, desligar_leds ? 0 : dados_leds.wrap_led_vermelho);
        pwm_set_gpio_level(pino_led_azul, desligar_leds ? 0 : dados_leds.wrap_led_led_azul);

        return;
    }
}


/*
* Função para atualizar o display
*/
bool repeating_timer_callback_display(struct repeating_timer *t)
{

    bool ativar_borda = dados_leds.estado_led_verde; // verifica se o botao do joystick foi pressionado
    controle_quadrado(&ssd, posicao_display.x, posicao_display.y, ativar_borda); // desenha o botao na posição do joystick

    return true;
}


/*
* Função para monitorar o estado do joystick
*/
bool repeating_timer_callback_joystick(struct repeating_timer *t)
{

    controle_joystick(&dados_leds, &posicao_display, dados_leds.estado_led_verde); // monitora o estado do joystick

    if (!desligar_leds) { // verifica se os leds estão desligados
        pwm_set_gpio_level(pino_led_azul, dados_leds.wrap_led_led_azul); // liga os leds com os valores do joystick
        pwm_set_gpio_level(pino_led_vermelho, dados_leds.wrap_led_vermelho); // liga os leds com os valores do joystick
    }

    return true;
}


