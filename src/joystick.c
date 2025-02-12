#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "joystick.h"
#include <math.h>


void controle_joystick(Dados_leds *dados_leds, Posicao_display *posicao_display, bool botao_joystick) {

    adc_select_input(1);
    uint16_t valor_eixo_x = adc_read();
   
    adc_select_input(0);
    uint16_t valor_eixo_y = adc_read();

    mapear_valores_para_led(dados_leds, valor_eixo_x, valor_eixo_y);
    mapear_valores_display(valor_eixo_x, valor_eixo_y, posicao_display, botao_joystick);


}

/*
* Função para mapear os valores do joystick para os leds
*/
static void mapear_valores_para_led(Dados_leds *dados_leds, uint16_t valor_eixo_X, uint16_t valor_eixo_Y) {
    uint16_t vrx = fabs(valor_eixo_X - 2048);
    dados_leds->wrap_led_led_azul = vrx > 200 ? vrx : 0; // Configura o valor do led azul

    uint16_t vry = fabs(valor_eixo_Y - 2048);
    dados_leds->wrap_led_vermelho = vry > 200 ? vry : 0; // Lê o valor do eixo Y
    
}


/*
* Função para mapear os valores do joystick para o display
*/
static void mapear_valores_display(uint16_t x, uint16_t y, Posicao_display *posicao_display, bool botao_joystick) {

    uint limite_x = botao_joystick ? 118 : 120; // define o limite do eixo X
    uint limite_y = botao_joystick ? 52 : 55; // define o limite do eixo Y

    posicao_display->x = (x *limite_x) / 4096; // mapeia o valor do eixo X
    posicao_display->y = (limite_y - (y * limite_y) / 4096); // mapeia o valor do eixo Y

    if (botao_joystick && posicao_display->x < 3) { // verifica se o limite do eixo X foi atingido
        posicao_display->x += 3;
    }
    
    if (botao_joystick && posicao_display->y < 4) { // verifica se o limite do eixo Y foi atingido
        posicao_display->y = 4;
    }

}