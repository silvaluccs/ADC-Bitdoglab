#include <stdio.h>
#include "pico/stdlib.h"
#include "setup.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "font.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C


#define IS_RGBW false

/*
* Função para inicializar o led 
*/
void setup_led(uint pino_led) {

    gpio_init(pino_led); // inicializando o pino
    gpio_set_dir(pino_led, GPIO_OUT); // definindo como saida
    gpio_put(pino_led, false); // deixando desligado inicialmente

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


void setup_display() {
    
  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA); // Pull up the data line
  gpio_pull_up(I2C_SCL); // Pull up the clock line
 

}

void init_display(ssd1306_t *ssd) {
    // Inicializa o display usando o ponteiro passado como argumento
    ssd1306_init(ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); 
    ssd1306_config(ssd);  
    ssd1306_send_data(ssd); 

    // Limpa o display
    ssd1306_fill(ssd, false);
    ssd1306_send_data(ssd);
}