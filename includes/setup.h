#ifndef SETUP_H
#define SETUP_H

#include "pico/stdlib.h"
#include "ssd1306.h"

void setup_led(uint pino_led);
void setup_led_pwm(uint pino_led);
void setup_botoes(uint pino_botao_a, uint pino_botao_b);
void setup_display();
void setup_adc(uint pino_vrx, uint pino_vry, uint pino_sw);
void init_display(ssd1306_t *ssd);

#endif