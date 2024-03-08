/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdlib.h>
#include <time.h>

const int BTN_PIN_R = 9;
const int BTN_PIN_G = 8;
const int BTN_PIN_B = 7;
const int BTN_PIN_Y = 6;

const int LED_PIN_R = 10;
const int LED_PIN_G = 11;
const int LED_PIN_B = 12;
const int LED_PIN_Y = 13;

const int BUZZER = 18; /* eh 17 o certo */

volatile int BTN_FLAG_R = 0;
volatile int BTN_FLAG_G = 0;
volatile int BTN_FLAG_B = 0;
volatile int BTN_FLAG_Y = 0;

volatile int btn_id = 4;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { // fall edge
    if (gpio == BTN_PIN_R){
      BTN_FLAG_R = 1;
      btn_id = 0;
    }
    else if(gpio == BTN_PIN_G){
      BTN_FLAG_G = 1;
      btn_id = 1;
    }
    else if(gpio == BTN_PIN_B){
      BTN_FLAG_B = 1;
      btn_id = 2;
    }
    else if(gpio == BTN_PIN_Y){
      BTN_FLAG_Y = 1;
      btn_id = 3;
    }
  }
}
void gera_tom(int freq, int duracao, int pino){ 
    int i;
    int tempo = 1000000 / (freq * 2);
    int n = duracao * freq / 1000;
    for (i = 0; i < n; i++) {
        gpio_put(pino, 1);
        sleep_us(tempo);
        gpio_put(pino, 0);
        sleep_us(tempo);
    }
}

void gera_tom_R(){
  gera_tom(440, 500, BUZZER);
}

void gera_tom_G(){
  gera_tom(494, 500, BUZZER);
}

void gera_tom_B(){
  gera_tom(554, 500, BUZZER);
}

void gera_tom_Y(){
  gera_tom(587, 500, BUZZER);
}

void gera_tom_acerto(){
  gera_tom(659, 500, BUZZER);
}

void gera_tom_erro(){
  gera_tom(330, 500, BUZZER);
}

void gera_sequencia_aleatoria(int dificuldade, int sequencia[]){

    srand(time(NULL));
  
    for (int i = 0; i < dificuldade; i++){
        sequencia[i] = rand() % 4;
    }
}


int main() {
    stdio_init_all();

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);

    gpio_init(BTN_PIN_B);
    gpio_set_dir(BTN_PIN_B, GPIO_IN);
    gpio_pull_up(BTN_PIN_B);

    gpio_init(BTN_PIN_Y);
    gpio_set_dir(BTN_PIN_Y, GPIO_IN);
    gpio_pull_up(BTN_PIN_Y);

    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);

    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);

    gpio_init(LED_PIN_B);
    gpio_set_dir(LED_PIN_B, GPIO_OUT);

    gpio_init(LED_PIN_Y);
    gpio_set_dir(LED_PIN_Y, GPIO_OUT);

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);

    gpio_set_irq_enabled(BTN_PIN_B, GPIO_IRQ_EDGE_FALL, true);

    gpio_set_irq_enabled(BTN_PIN_Y, GPIO_IRQ_EDGE_FALL, true);

    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);

    int dificuldade = 1;
  
    int sequencia[321];

    while (true) {
        gera_sequencia_aleatoria(dificuldade, sequencia);

        for (int i = 0; i < dificuldade; i++){
            if (sequencia[i] == 0){
                gpio_put(LED_PIN_R, 1);
                gera_tom_R();
                sleep_ms(500);
                gpio_put(LED_PIN_R, 0);
                sleep_ms(500);
            }
            else if (sequencia[i] == 1){
                gpio_put(LED_PIN_G, 1);
                gera_tom_G();
                sleep_ms(500);
                gpio_put(LED_PIN_G, 0);
                sleep_ms(500);
            }
            else if (sequencia[i] == 2){
                gpio_put(LED_PIN_B, 1);
                gera_tom_B();
                sleep_ms(500);
                gpio_put(LED_PIN_B, 0);
                sleep_ms(500);
            }
            else if (sequencia[i] == 3){
                gpio_put(LED_PIN_Y, 1);
                gera_tom_Y();
                sleep_ms(500);
                gpio_put(LED_PIN_Y, 0);
                sleep_ms(500);
            }
        }

        int tentativas = 0;
        int errouu = 0;
        while (tentativas < dificuldade){
          if (btn_id == 0){
              gpio_put(LED_PIN_R, 1);
              gera_tom_R();
              gpio_put(LED_PIN_R, 0);
          }

          else if (btn_id == 1){
              gpio_put(LED_PIN_G, 1);
              gera_tom_G();
              gpio_put(LED_PIN_G, 0);
          }

          else if (btn_id == 2){
              gpio_put(LED_PIN_B, 1);
              gera_tom_B();
              gpio_put(LED_PIN_B, 0);
          }

          else if (btn_id == 3){
              gpio_put(LED_PIN_Y, 1);
              gera_tom_Y();
              gpio_put(LED_PIN_Y, 0);
          }
          while(btn_id != 4){
            if (btn_id != sequencia[tentativas]){
                  errouu = 1;
                  gera_tom_erro();
              }
            else{
                  gera_tom_acerto();
              }
            btn_id = 4;
            tentativas++;
          }
        }
        if (errouu)
          dificuldade = 1 ;
        else
          dificuldade++;
        

    }
}
