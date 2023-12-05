/**
 * @file r2d2.h
 * @author Charly Batista (carlbsb@gmail.com)
 * @brief R2D2 is am autonomous guided vehicle (AGV) created as part of the Mechatronics undergraduate program in UniSenai Joinville/Brazil
 * @version 0.1
 * @date 2023-11-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __R2D2_H__
#define __R2D2_H__

#include "r2d2_const.h"

//
enum POSITION
{
  POS_INITIAL = 0,
  POS_OBSTACLE_1,
  POS_OBSTACLE_2,
  POS_OBSTACLE_3,
  POS_WALL,
  POS_END
};

// 
extern volatile int64_t started;

// 
extern uint8_t position;

// 

// Function that starts and/or shuts down the AGV
void R2C_power_watcher_2();

// 
void print_logo();

//
/**
 * Mostra a mensagem no display OLED
 * O display tem dimensoes de 128x48
 * @pos_x Posicao horizontal aceitando valores entre 0 e 128
 * @pos_y Posicao vertical aceitando valores entre 0 e 32
 * @size Tamanho da fonte
 * @color Codigo para a cor
 * @render Booleano pra indicar se e para renderizar a mensagem na tela ou apenas "bufferizar"
 * @msg Mensagem de texto a ser exibida no display
*/
void r2d2_print(const uint8_t pos_x, const uint8_t pos_y,
                const uint8_t size, const uint8_t color,
                bool render, char *msg);


/**
 * 
*/
uint16_t R2C_get_distance(uint8_t num_avg);
void R2C_controller(uint16_t distance);

#endif  // __R2D2_H__