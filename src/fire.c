/*
 * fire.c
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent R�veill�re
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "SDL.h"
#include "SDL_image.h"

#include "../include/game.h"
#include "../include/misc.h"

struct s_fire {
	int time_left;
	int x;
	int y;
};

void fire_set_fire(t_fire* fires, t_map map, int x, int y) {
	assert(fires);
	assert(map && map_is_inside(map,x,y));
	int i = 0;
	while (fires[i] != NULL && i < MAXFIRE) {
		i++;
	}
	if (i < MAXFIRE) {
		t_fire fire = malloc(sizeof(*fire));
		if (!fire)
			error("Memory error");
		fires[i] = fire;
		fires[i]->time_left = FIRE_LIFETIME;
		fires[i]->x = x;
		fires[i]->y = y;
	}
}
void fire_decrement(t_fire* fires, int temps) {
	assert(fires);
	int i;
	for (i = 0; i < MAXFIRE; i++) {
		if (fires[i] != NULL) {
			if (fires[i]->time_left <= 0)
				fire_remove(fires, i);
			else
				fires[i]->time_left -= temps;
		}
	}
}
void fire_display(t_map map, t_fire* fires, SDL_Surface *screen) {
	assert(fires);
	assert(map);
	assert(screen);
	int i;
	for (i = 0; i < MAXFIRE; i++) {
		if (fires[i] != NULL) {
			SDL_Rect place;
			place.x = fires[i]->x * SIZE_BLOC;
			place.y = fires[i]->y * SIZE_BLOC;
			SDL_BlitSurface(map_get_imgfire(map), NULL, screen, &place);
		}
	}
}
int fire_is_at_xy(t_fire* fires, int x, int y) {
	assert(fires);
	int i = 0, again = 1;
	while (i < MAXFIRE && again) {
		if (fires[i] != NULL && fires[i]->x == x && fires[i]->y == y)
			return 1;
		i++;
	}
	return 0;
}
void fire_remove(t_fire* fires, int id) {
	assert(fires);
	assert(fires[id]);
	free(fires[id]);
	fires[id] = NULL;

}
void fire_hurt_player(t_player* players, t_fire* fires, t_map map) {
	assert(players);
	assert(fires);
	assert(map);
	int i;
	int x;
	int y;
	for (i = 0; i < MAXFIRE; i++) {
		if (fires[i] != NULL) {
			x = fires[i]->x;
			y = fires[i]->y;
			if (map_get_cell_type(map, x, y) == CELL_MONSTER
					|| map_get_cell_type(map, x, y) == CELL_PLAYER) {
				player_dec_life(player_get_player_by_xy(players, x, y));
				if (player_get_type(player_get_player_by_xy(players, x, y))
						== PLAYER_STD)
					map_set_cell_type(map, x, y, CELL_PLAYER);
				else
					map_set_cell_type(map, x, y, CELL_MONSTER);
			}
		}
	}
}
