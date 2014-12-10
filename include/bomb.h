/*
 * bomb.h
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent R�veill�re
 */

#ifndef BOMB_H_
#define BOMB_H_

#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"

#include "../include/constant.h"

enum e_type_bomb {
	BOMB_STD = 0, EGG_CLASSIC, EGG_BUILDER, EGG_RES, EGG_QUEEN
};

typedef struct s_bomb * t_bomb;

/*******GETERS********/
extern int bomb_get_time_left(t_bomb* bombes, int id);
extern int bomb_get_x(t_bomb* bombes, int id);
extern int bomb_get_y(t_bomb* bombes, int id);
extern int bomb_get_type(t_bomb* bombes, int id);
extern int bomb_get_way(t_bomb* bombes, int id);
extern int bomb_is_rolling(t_bomb* bombes, int id);
extern int bomb_get_range(t_bomb* bombes, int id);
extern int bomb_get_owner(t_bomb* bombes, int id);
//gets the id of the bomb wich is at x,y
extern int bomb_get_id_by_xy(t_bomb* bombes, int x, int y);
extern float bomb_get_move_timer(t_bomb bomb);

/*******SETERS********/
//set a bomb at x,y with the range, type, owner
extern void bomb_set_bomb(t_bomb* bombes, int x, int y, int range, int type,
		int owner, int duration);
extern void bomb_set_time_left(t_bomb* bombes, int id, int time);
extern void bomb_set_x(t_bomb* bombes, int id, int x);
extern void bomb_set_y(t_bomb* bombes, int id, int y);
extern void bomb_set_way(t_bomb* bombes, int id, int way);
extern void bomb_set_rolling(t_bomb* bombes, int id, int flag);
extern void bomb_set_range(t_bomb* bombes, int id, int range);
extern void bomb_set_type(t_bomb* bombes, int id, int type);
//the bomb at x,y it will explode
extern void bomb_set_explode_by_xy(t_bomb* bombes, int x, int y);
extern void bomb_set_move_time(t_bomb bomb, float time);

/*******MODIFIERS********/
//Removes the bomb with id=id and free it
extern void bomb_remove(t_bomb* bombes, int id);
//call the function above after it found the id of the bomb in x,y
extern void bomb_remove_by_xy(t_bomb* bombes, int x, int y);
//decrements the time left to explode of bomb[id] of the duration time
extern void bomb_dec_time_left(t_bomb* bombes, int id, int time);
//same thing for all the bombs
extern void bomb_decrement(t_bomb* bombes, int time);
//push the bomb (foot_ability) and it will roll
extern void bomb_be_pushed(t_bomb* bombes, int id, int direction);
//Increase the rolling timer
extern void bomb_inc_move_timer(t_bomb bomb, int time);
//increase rests of the position (for rolling bombs);
extern void bomb_inc_restx(t_bomb bomb, float x);
extern void bomb_inc_resty(t_bomb bomb, float y);

/*******TESTERS********/
//Tests if the bomb with id=id is set
extern int bomb_is_set(t_bomb* bombes, int id);
//tests if there is a bomb at x,y (returns 1 or 0)
extern int bomb_is_at_xy(t_bomb* bombes, int x, int y);
//tests if there will be an explosion at x,y and returns the time left to explode (or -1 if not)
extern int bomb_time_to_explode_at_xy(t_map map, t_bomb* bombes, int x, int y);

/*******OTHER********/
//Displays all the bombs
extern void bomb_display(t_map map, t_bomb* bombes, SDL_Surface *screen);
extern void bomb_destroy_case(t_player* players, t_map map, int x, int y);
//explode everything in the range of the bomb which has id=id.
int bomb_explode_aux2(t_player* players, t_fire* fires, int id, t_map map,
		t_bomb* bombes, int x, int y);
int bomb_explode_aux(t_player* players, t_fire* fires, t_bomb* bombes,
		t_map map, int id, int x, int y);
extern void bomb_explode(t_player* players, t_fire* fires, t_bomb* bombes,
		t_map map);
//roll all the rolling bombs
extern void bomb_roll(t_bomb* bombes, t_map map);

#endif
