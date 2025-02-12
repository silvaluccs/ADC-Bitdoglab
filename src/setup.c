#include <stdio.h>
#include "pico/stdlib.h"
#include "setup.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "ssd1306.h"
#include "font.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C


/*
* Função para inicializar o led com PWM
*/
void setup_led_pwm(uint pino_led) {

    gpio_set_function(pino_led, GPIO_FUNC_PWM);  // Habilitar o pino GPIO como PWM = pwm_gpio_to_slice_num(pino);    // Obter o canal PWM da GPIO
    uint slice = pwm_gpio_to_slice_num(pino_led);  // Obter o canal PWM da GPIO
    pwm_set_clkdiv(slice, 16.f);           // Define o divisor de clock do PWM
    pwm_set_wrap(slice, 2048);              // Define o valor de wrap (0 passos)
    pwm_set_gpio_level(pino_led, 0);       // Define o Duty Cycle inicial (2,5%)
    pwm_set_enabled(slice, true);               // Habilita o canal PWM
}


/*
* Função para inicializar o led 
*/
void setup_led(uint pino_led) {
    gpio_init(pino_led); // inicializando o pino
    gpio_set_dir(pino_led, GPIO_OUT); // definindo como saída
    gpio_put(pino_led, 0); // desligando o led
}



/*
* Função para inicializar os botões a e b
*/
void setup_botoes(uint pino_botao_a, uint pino_botao_b) {

    gpio_init(pino_botao_a); // inicializando o pino
    gpio_set_dir(pino_botao_a, GPIO_IN); // defininfo como entrada
    gpio_pull_up(pino_botao_a); // ativando resistores internos

    gpio_init(pino_botao_b); // inicializando o pino
    gpio_set_dir(pino_botao_b, GPIO_IN); // definindo como entrada
    gpio_pull_up(pino_botao_b); // ativando resistores internos

}

/*
* Função para inicializar o ADC
*/
void setup_joystick(uint pino_vrx, uint pino_vry, uint pino_sw) {
    adc_init();
    adc_gpio_init(pino_vrx);
    adc_gpio_init(pino_vry);
    gpio_init(pino_sw);
    gpio_set_dir(pino_sw, GPIO_IN);
    gpio_pull_up(pino_sw);
}

/*
* Função para inicializar o display
*/
void setup_display() {
    
  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA); // Pull up the data line
  gpio_pull_up(I2C_SCL); // Pull up the clock line
 

}

/*
* Função para inicializar o display
*/
void init_display(ssd1306_t *ssd) {
    // Inicializa o display usando o ponteiro passado como argumento
    ssd1306_init(ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); 
    ssd1306_config(ssd);  
    ssd1306_send_data(ssd); 

    // Limpa o display
    ssd1306_fill(ssd, false);
    ssd1306_send_data(ssd);
}