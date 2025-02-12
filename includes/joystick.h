#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdlib.h>
#include "pico/stdlib.h"

typedef struct dados_leds {
    uint16_t wrap_led_vermelho;
    uint16_t wrap_led_led_azul;
    bool estado_led_verde;
} Dados_leds; // estrutura para armazenar os dados do joystick

typedef struct posicao {
    int x;
    int y;
} Posicao_display; // estrutura para armazenar a posição do joystick no display

void controle_joystick(Dados_leds *dados_leds, Posicao_display *posicao_display, bool botao_joystick);
static void mapear_valores_para_led(Dados_leds *dados_leds, uint16_t valor_eixo_X, uint16_t valor_eixo_Y);
static void mapear_valores_display(uint16_t x, uint16_t y, Posicao_display *posicao_display, bool botao_joystick);

#endif