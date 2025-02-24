# ADC-Bitdoglab

Este projeto utiliza a Raspberry Pi Pico para controlar LEDs RGB, um display OLED SSD1306 e um joystick analógico. O código implementa leitura de ADC, debounce para botões e exibição de informações no display OLED.

## Bibliotecas Utilizadas

O projeto usa as seguintes bibliotecas e arquivos:

### 1. `pico/stdlib.h`
Biblioteca padrão da Raspberry Pi Pico para manipulação de GPIOs, UART, delays e timers.

### 2. `hardware/pwm.h`
Usada para controlar a modulação por largura de pulso (PWM) nos LEDs RGB.

### 3. `hardware/adc.h`
Permite a leitura de valores analógicos, usada para captar os movimentos do joystick.

### 4. `hardware/i2c.h`
Utilizada para comunicação com o display SSD1306 via protocolo I2C.

### 5. `hardware/timer.h`
Fornece suporte para manipulação de timers e interrupções baseadas em tempo.

### 6. `ssd1306.h`
Biblioteca própria do projeto para o controle do display OLED SSD1306.

### 7. `debouce.h`
Implementa um mecanismo de debounce para evitar múltiplas detecções errôneas de pressionamento de botões.

### 8. `joystick.h`
Gerencia a leitura dos valores do joystick e mapeia esses valores para controlar os LEDs e a interface gráfica no display.

### 9. `setup.h`
Inicializa os periféricos necessários, como PWM, ADC e I2C.

---

## Funcionamento do `main.c`

O `main.c` contém a lógica principal do projeto. Ele faz o seguinte:

1. Define os pinos dos LEDs, botões e joystick.
2. Configura os periféricos necessários (I2C, ADC, PWM).
3. Usa um timer para capturar periodicamente os valores do joystick e atualizar o estado dos LEDs e do display OLED.
4. Implementa uma interrupção baseada em timer para realizar a leitura do joystick de forma assíncrona.
5. Monitora a entrada dos botões utilizando o mecanismo de debounce para evitar leituras incorretas.

---

## Uso de Interrupções e Timers

O projeto utiliza timers e interrupções para:

- **Leitura Periódica do Joystick**: Um timer repetitivo é configurado para chamar uma função que lê os valores do joystick em intervalos regulares, garantindo uma atualização fluida.
- **Controle de LEDs com PWM**: O PWM é utilizado para ajustar o brilho dos LEDs RGB com base na posição do joystick.
- **Debounce de Botões**: Usa um temporizador para garantir que um botão pressionado não gere múltiplos eventos acidentais.

Essas estratégias garantem um funcionamento eficiente e sem bloqueios, permitindo que o microcontrolador continue executando outras tarefas enquanto aguarda eventos assíncronos.

---

## Como Compilar e Executar

1. Clone este repositório.
2. Certifique-se de ter o SDK do Raspberry Pi Pico configurado.
3. Compile usando CMake e o ambiente de desenvolvimento apropriado.
4. Carregue o arquivo `.uf2` gerado na Raspberry Pi Pico.

---

Este README fornece uma visão geral do projeto. Para mais detalhes, consulte os arquivos no diretório `includes/` e `src/`.

