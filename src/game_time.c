/*
 * game_time.c
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent R�veill�re
 */

#include "../include/game_time.h"
#include "../include/constant.h"

static Uint32 timer1, timer2;

struct s_game_time {
	// Number of cycles to process before displaying an image
	Uint16 cycles_to_calculate;

	// Current speed of the game
	Uint16 speed;

	// Duration of a cycle (in ms)
	Uint16 cycle_length;
};

static struct s_game_time the_game_time;

Uint16 game_time_get_cycles_to_calculate() {
	return the_game_time.cycles_to_calculate;
}

Uint16 game_time_get_speed() {
	return the_game_time.speed;
}

Uint16 game_time_get_cycles_length() {
	return the_game_time.cycle_length;
}

void game_time_init() {
	the_game_time.cycles_to_calculate = 0;
	game_time_set_speed(DEFAULT_GAME_SPEED);
}

// Set the speed of the game (number of cycles per second)
void game_time_set_speed(Uint16 speed) {
	if (speed == 0)
		speed = 1;
	the_game_time.speed = speed;
	the_game_time.cycle_length = 1000 / speed;
	timer1 = SDL_GetTicks();
}

void game_time_update() {
	while (1) {
		timer2 = SDL_GetTicks() - timer1;

		if (timer2 >= the_game_time.cycle_length)
			break;
		else
			SDL_Delay(3);
	}

	timer1 = SDL_GetTicks() - (timer2 % the_game_time.cycle_length);
	the_game_time.cycles_to_calculate = timer2 / the_game_time.cycle_length;

	if (the_game_time.cycles_to_calculate > MAX_SKIPPED_FRAMES)
		the_game_time.cycles_to_calculate = MAX_SKIPPED_FRAMES;
}
