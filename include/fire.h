/*
 * fire.h
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent R�veill�re
 */

#ifndef FIRE_H_
#define FIRE_H_

#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"

#include "../include/constant.h"

typedef struct s_fire* t_fire;

// Make a new fire at x,y
extern void fire_set_fire(t_fire* fires, t_map map, int x, int y);
// Decrement the time of the fire and remove it if it has to be removed
extern void fire_decrement(t_fire* fires, int temps);
// Displays all the fires
extern void fire_display(t_map map, t_fire* fires, SDL_Surface *screen);
// Tests if there is a fire at x,y
extern int fire_is_at_xy(t_fire* fires, int x, int y);
// Removes the fire
extern void fire_remove(t_fire* fires, int id);
// Hurt everyone that is in the fire
extern void fire_hurt_player(t_player* players, t_fire* fires, t_map map);

#endif
