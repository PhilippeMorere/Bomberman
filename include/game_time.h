/*
 * game_time.h
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent R�veill�re
 */

#ifndef __TIME_H__
#define __TIME_H__

#include <SDL.h>

// Number of cycles to process before displaying an image
extern Uint16 game_time_get_cycles_to_calculate();

// Current speed of the game
extern Uint16 game_time_get_speed();

// Duration of a cycle (in ms)
extern Uint16 game_time_get_cycles_length();

// Time initialization
extern void game_time_init();

// Set the speed of the game
extern void game_time_set_speed(Uint16 speed);

// Compute the current time of the game
extern void game_time_update();

#endif /* __TIME_H__ */
