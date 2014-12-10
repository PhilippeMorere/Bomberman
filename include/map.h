/*
 * map.h
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent R�veill�re
 */

#ifndef MAP_H_
#define MAP_H_

#include <stdlib.h>
#include <stdio.h>
#include "SDL.h"
#include "SDL_image.h"

typedef enum {
	CELL_EMPTY = 0,
	CELL_FLAG,
	CELL_TREE,
	CELL_PLAYER,
	CELL_CASE,
	CELL_BONUS,
	CELL_MONSTER,
	CELL_BOMB,
	CELL_SPAWN
} t_cell_type;

typedef enum {
	BONUS_NOTHING = 0,
	BONUS_BOMB_RANGE_INC,
	BONUS_BOMB_RANGE_DEC,
	BONUS_BOMB_NB_INC,
	BONUS_BOMB_NB_DEC,
	BONUS_LAUNCH_BOMB,
	BONUS_MULTI_BOMB,
	BONUS_SPEED,
	BONUS_MONSTER,
	BONUS_LIFE,
	BONUS_DISEASE
} t_bonus_type;
typedef enum {
	MONSTER_CLASSIC = 0, MONSTER_QUEEN, MONSTER_BUILDER, MONSTER_RES
} t_monster_type;

typedef enum {
	EMPTY = 0, GREEN_BLOOD, RED_BLOOD
} t_empty_type;
typedef struct t_map * t_map;

// Create a new empty map
extern t_map map_new(int width, int height);
extern void map_free(t_map map);

// Return the height and width of a map
extern int map_get_width(t_map map);
extern int map_get_height(t_map map);

// Return the type of a cell or the special type of a cell
extern t_cell_type map_get_cell_type(t_map map, int x, int y);
extern int map_get_cell_special(t_map map, int x, int y);

// Set the type of a cell or the special type of a cell
extern void map_set_cell_type(t_map map, int x, int y, t_cell_type type);
extern void map_set_cell_special(t_map map, int x, int y, t_bonus_type type);

// Test if (x,y) is within the map
extern int map_is_inside(t_map map, int x, int y);

// Load a static pre-calculated map
extern t_map map_load_static(void);

//Load a map from a file
extern t_map map_load_from_file(char *srcfile);

//convert case to bonus under case
void map_convert_case(t_map map);
//generates a random bonus
extern int map_bonus_proba();

// Display the map on the screen
extern void map_display(t_map map, SDL_Surface *screen);
//make the trees grow
extern void map_grow_trees(t_map map);
// get parametre
extern int * get_parameter();

// Get sprites
extern SDL_Surface * map_get_imgfire(t_map map);
extern SDL_Surface * map_get_imgbomb4(t_map map);
extern SDL_Surface * map_get_imgbomb3(t_map map);
extern SDL_Surface * map_get_imgbomb2(t_map map);
extern SDL_Surface * map_get_imgbomb1(t_map map);
extern SDL_Surface * map_get_imgegg(t_map map);

#endif /* MAP_H_ */
