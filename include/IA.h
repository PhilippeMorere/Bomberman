/*
 * IA.h
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent R�veill�re
 */

#ifndef IA_H_
#define IA_H_

#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"

#include "../include/constant.h"

typedef enum {
	DANGER0 = 0, DANGER1, DANGER2, DANGER3, DANGER4, DANGER5, WALL, OBJECTIVE
} t_grid_type;

typedef struct s_ia* t_ia;

//init the ia
extern t_ia IA_new(int width, int height, int level);
//free the ia
extern void  IA_free(t_ia ia);
//upgrad the grid of the map when something has changed
//extern void IA_upgrade_grid(t_ia ia, t_map map, t_fire* fires);
//call this function at each cycle for the IA to work
extern void IA_do(t_map map, t_player* players, t_bomb* bombes, t_fire* fires,
		t_ia ia, int queenid, int id1, int id2);
//for IA4, if we dont want to go x,y again
//void IA_set_grid_done(t_ia ia, int x, int y);
//void IA_set_grid_done(t_ia ia, int x, int y);
//int IA_grid_is_done(t_ia ia, int x, int y);
//different types of IA
//easy
void IA_1(t_map map, t_player* players, t_bomb* bombes, t_fire* fires, t_ia ia,
		int id);
//medium
void IA_2(t_map map, t_player* players, t_bomb* bombes, t_fire* fires, t_ia ia,
		int id, int id1, int id2);
//hard
int IArecursive(t_map map, int id, int i, int j, int dir);
void IA_3(t_map map, t_player* players, t_bomb* bombes, t_fire* fires, t_ia ia,
		int id, int id1, int id2);
//impressive
/*int IA_4_recursive(t_map map, t_ia ia, int x, int y, int limit);
void IA_4(t_map map, t_player* players, t_bomb* bombes, t_fire* fires, t_ia ia,
		int id, int id1, int id2);*/

//queen_monster
void IA_queen(t_map map, t_player* players, t_bomb* bombes, t_fire* fires,
		t_ia ia, int id);
//ressource_monster
void IA_res(t_map map, t_player* players, t_bomb* bombes, t_fire* fires,
		t_ia ia, int id, int queenid);
//builder_monster
void IA_builder(t_map map, t_player* players, t_bomb* bombes, t_fire* fires,
		t_ia ia, int id, int queenid);
#endif
