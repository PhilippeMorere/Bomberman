/*
 * map.c
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent R�veill�re
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL.h>
#include "SDL_image.h"

#include <time.h>
#include "../include/constant.h"
#include "../include/map.h"
#include "../include/misc.h"
#include "../include/game_time.h"

struct t_map {
	int width;
	int height;

	t_cell_type *grid;

	SDL_Surface *img_wall;
	SDL_Surface *img_wall2;
	SDL_Surface *img_wall3;
	SDL_Surface *img_wall4;
	SDL_Surface *img_wall5;
	SDL_Surface *img_box;
	SDL_Surface *img_goal;
	SDL_Surface *img_bomb1;
	SDL_Surface *img_bomb2;
	SDL_Surface *img_bomb3;
	SDL_Surface *img_bomb4;
	SDL_Surface *img_bonus_bomb_range_inc;
	SDL_Surface *img_bonus_bomb_range_dec;
	SDL_Surface *img_bonus_bomb_nb_inc;
	SDL_Surface *img_bonus_bomb_nb_dec;
	SDL_Surface *img_bonus_multi_bomb;
	SDL_Surface *img_bonus_launch_bomb;
	SDL_Surface *img_bonus_life;
	SDL_Surface *img_fire;
	SDL_Surface *img_egg;
	SDL_Surface *img_spawn;
	SDL_Surface *img_bonus_speed;
	SDL_Surface *img_bonus_disease;
};

#define CELL(i,j) (i +  map->width * j)

t_map map_new(int width, int height) {
	assert(width > 0 && height > 0);
	t_map map = malloc(sizeof(struct t_map));
	if (map == NULL)
		exit(EXIT_FAILURE);

	map->width = width;
	map->height = height;

	map->grid = (t_cell_type *) malloc(height * width * sizeof(t_cell_type));
	if (map->grid == NULL)
		exit(EXIT_FAILURE);

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
			map->grid[CELL(i,j)] = CELL_EMPTY;
		}
	}

	// Sprite loading
	map->img_wall = load_image(IMG_MAP_ROCK);
	map->img_wall2 = load_image(IMG_MAP_ROCK2);
	map->img_wall3 = load_image(IMG_MAP_ROCK3);
	map->img_wall4 = load_image(IMG_MAP_ROCK4);
	map->img_wall5 = load_image(IMG_MAP_ROCK5);
	map->img_box = load_image(IMG_MAP_CASE);
	map->img_goal = load_image(IMG_MAP_FLAG);
	map->img_bomb1 = load_image(IMG_BOMB_TTL1);
	map->img_bomb2 = load_image(IMG_BOMB_TTL2);
	map->img_bomb3 = load_image(IMG_BOMB_TTL3);
	map->img_bomb4 = load_image(IMG_BOMB_TTL4);
	map->img_bonus_bomb_range_inc = load_image(IMG_BONUS_BOMB_RANGE_INC);
	map->img_bonus_bomb_range_dec = load_image(IMG_BONUS_BOMB_RANGE_DEC);
	map->img_bonus_bomb_nb_inc = load_image(IMG_BONUS_BOMB_NB_INC);
	map->img_bonus_bomb_nb_dec = load_image(IMG_BONUS_BOMB_NB_DEC);
	map->img_bonus_multi_bomb = load_image(IMG_BONUS_MULTI_BOMB);
	map->img_bonus_launch_bomb = load_image(IMG_BONUS_LAUNCH_BOMB);
	map->img_bonus_life = load_image(IMG_BANNER_LIFE);
	map->img_fire = load_image(IMG_FIRE);
	map->img_egg = load_image(IMG_EGG);
	map->img_spawn = load_image(IMG_SPAWN);
	map->img_bonus_speed = load_image(IMG_BONUS_SPEED);
	map->img_bonus_disease = load_image(IMG_BONUS_DISEASE);

	return map;
}

int map_is_inside(t_map map, int x, int y) {
	assert(map);
	if (x < 0 || y < 0 || x >= map->height || y >= map->width) {
		return 0;
	}
	return 1;
}

void map_free(t_map map) {
	assert(map);
	if (map == NULL)
		return;

	free(map->grid);
	SDL_FreeSurface(map->img_wall);
	SDL_FreeSurface(map->img_box);
	SDL_FreeSurface(map->img_goal);
	SDL_FreeSurface(map->img_wall2);
	SDL_FreeSurface(map->img_wall3);
	SDL_FreeSurface(map->img_wall4);
	SDL_FreeSurface(map->img_wall5);
	SDL_FreeSurface(map->img_bomb1);
	SDL_FreeSurface(map->img_bomb2);
	SDL_FreeSurface(map->img_bomb3);
	SDL_FreeSurface(map->img_bomb4);
	SDL_FreeSurface(map->img_bonus_bomb_range_inc);
	SDL_FreeSurface(map->img_bonus_bomb_range_dec);
	SDL_FreeSurface(map->img_bonus_bomb_nb_inc);
	SDL_FreeSurface(map->img_bonus_bomb_nb_dec);
	SDL_FreeSurface(map->img_bonus_multi_bomb);
	SDL_FreeSurface(map->img_bonus_launch_bomb);
	SDL_FreeSurface(map->img_bonus_life);
	SDL_FreeSurface(map->img_fire);
	SDL_FreeSurface(map->img_egg);
	SDL_FreeSurface(map->img_spawn);
	SDL_FreeSurface(map->img_bonus_speed);
	SDL_FreeSurface(map->img_bonus_disease);
	free(map);
}

int map_get_width(t_map map) {
	assert(map);
	return map->width;
}

int map_get_height(t_map map) {
	assert(map);
	return map->height;
}
SDL_Surface * map_get_imgfire(t_map map) {
	assert(map);
	return map->img_fire;
}
SDL_Surface * map_get_imgbomb4(t_map map) {
	assert(map);
	return map->img_bomb4;
}
SDL_Surface * map_get_imgbomb3(t_map map) {
	assert(map);
	return map->img_bomb3;
}
SDL_Surface * map_get_imgbomb2(t_map map) {
	assert(map);
	return map->img_bomb2;
}
SDL_Surface * map_get_imgbomb1(t_map map) {
	assert(map);
	return map->img_bomb1;
}
SDL_Surface * map_get_imgegg(t_map map) {
	assert(map);
	return map->img_egg;
}

t_cell_type map_get_cell_type(t_map map, int x, int y) {
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0x0f;
}
int map_get_cell_special(t_map map, int x, int y) {
	assert(map && map_is_inside(map, x, y));
	return (map->grid[CELL(x,y)] & 0xf0) >> 4;
}

void map_set_cell_type(t_map map, int x, int y, t_cell_type type) {
	assert(map && map_is_inside(map, x, y));

	map->grid[CELL(x,y)] = type;
}

void map_set_cell_special(t_map map, int x, int y, t_bonus_type type) {
	assert(map && map_is_inside(map, x, y));

	map->grid[CELL(x,y)] = (type << 4) | map_get_cell_type(map, x, y);
}

int parameter[7];
int * get_parameter() {
	return parameter;
}

t_map map_load_from_file(char *srcfile) {
	FILE* file = NULL;
	char character = 0;
	file = fopen(srcfile, "r+");
	int counter = 0;
	int jumpcounter = 0;
	int width = 0;
	int height = 0;
	int life1 = -1, life2 = -1;
	int bomb1 = -1, bomb2 = -1;
	int range1 = -1, range2 = -1;
	int mode = -1;

	if (file == NULL) {
		error("error with the loading of the file");
		// exit
	}

	int testfile = fscanf(file, "%i:%i:%i:%i:%i:%i:%i:%i:%i", &width, &height,
			&mode, &life1, &bomb1, &range1, &life2, &bomb2, &range2);
	//16:16:QUEEN/CLASSIC/MULTI:VIE1:BOMB1:RANGE1:VIE2:BOMB2:RANGE2
	if (testfile == -1)
		error("error while loading the map");

	if (mode != -1) // it's a save file
	{
		parameter[0] = mode;
		parameter[1] = life1;
		parameter[2] = bomb1;
		parameter[3] = range1;
		parameter[4] = life2;
		parameter[5] = bomb2;
		parameter[6] = range2;

	}

	t_map map = map_new(width, height); // declaring of the map
	t_cell_type themap[width * height];
	character = fgetc(file);
	map->width = width;
	map->height = height;

	// reading character one by one
	while (character != EOF) {

		if (character != '\n') {

			switch (character) {
			case '0':
				themap[counter - jumpcounter] = CELL_EMPTY;
				break;
			case '1':
				themap[counter - jumpcounter] = CELL_FLAG;
				break;
			case '2':
				themap[counter - jumpcounter] = CELL_PLAYER;
				break;
			case '3':
				themap[counter - jumpcounter] = CELL_CASE;
				break;
			case 'a':
				themap[counter - jumpcounter] = 16 * BONUS_BOMB_NB_DEC
						+ CELL_CASE;
				break;
			case 'c':
				themap[counter - jumpcounter] = 16 * BONUS_BOMB_NB_INC
						+ CELL_CASE;
				break;
			case 'D':
				themap[counter - jumpcounter] = 16 * BONUS_BOMB_RANGE_DEC
						+ CELL_CASE;
				break;
			case 'e':
				themap[counter - jumpcounter] = 16 * BONUS_BOMB_RANGE_INC
						+ CELL_CASE;
				break;
			case 'f':
				themap[counter - jumpcounter] = 16 * BONUS_DISEASE + CELL_CASE;
				break;
			case 'g':
				themap[counter - jumpcounter] = 16 * BONUS_LAUNCH_BOMB
						+ CELL_CASE;
				break;
			case 'h':
				themap[counter - jumpcounter] = 16 * BONUS_LIFE + CELL_CASE;
				break;
			case 'i':
				themap[counter - jumpcounter] = 16 * BONUS_MONSTER + CELL_CASE;
				break;
			case 'j':
				themap[counter - jumpcounter] = 16 * BONUS_MULTI_BOMB
						+ CELL_CASE;
				break;
			case 'k':
				themap[counter - jumpcounter] = 16 * BONUS_NOTHING + CELL_CASE;
				break;
			case 's':
				themap[counter - jumpcounter] = 16 * BONUS_SPEED + CELL_CASE;
				break;
			case '4':
				themap[counter - jumpcounter] = CELL_TREE;
				break;
			case '5':
				themap[counter - jumpcounter] = 16 * MONSTER_CLASSIC
						+ CELL_MONSTER;
				break;
			case 'm':
				themap[counter - jumpcounter] = 16 * MONSTER_CLASSIC
						+ CELL_MONSTER;
				break;
			case 'q':
				themap[counter - jumpcounter] = 16 * MONSTER_QUEEN
						+ CELL_MONSTER;
				break;
			case 'b':
				themap[counter - jumpcounter] = 16 * MONSTER_BUILDER
						+ CELL_MONSTER;
				break;
			case 'r':
				themap[counter - jumpcounter] = 16 * MONSTER_RES + CELL_MONSTER;
				break;
			case 'x':
				themap[counter - jumpcounter] = 16 * BONUS_BOMB_NB_DEC
						+ CELL_BONUS;
				break;
			case 'o':
				themap[counter - jumpcounter] = 16 * BONUS_BOMB_NB_INC
						+ CELL_BONUS;
				break;
			case '+':
				themap[counter - jumpcounter] = 16 * BONUS_BOMB_RANGE_INC
						+ CELL_BONUS;
				break;
			case '-':
				themap[counter - jumpcounter] = 16 * BONUS_BOMB_RANGE_DEC
						+ CELL_BONUS;
				break;
			case 'l':
				themap[counter - jumpcounter] = 16 * BONUS_LAUNCH_BOMB
						+ CELL_BONUS;
				break;
			case 'M':
				themap[counter - jumpcounter] = 16 * BONUS_MULTI_BOMB
						+ CELL_BONUS;
				break;
			case 'v':
				themap[counter - jumpcounter] = 16 * BONUS_LIFE + CELL_BONUS;
				break;
			case 'd':
				themap[counter - jumpcounter] = 16 * BONUS_DISEASE + CELL_BONUS;
				break;
			case 'S':
				themap[counter - jumpcounter] = 16 * BONUS_SPEED + CELL_BONUS;
				break;
			default:
				themap[counter - jumpcounter] = CELL_EMPTY;
				break;
			}
		} else {
			jumpcounter++;
		}
		character = fgetc(file); // reading of the next character
		counter++;

	}
	fclose(file);
	for (int i = 0; i < height * width; i++)
		map->grid[i] = themap[i];
	map_convert_case(map);
	return map;

}

void map_convert_case(t_map map) {
	assert(map);
	int i, j;
	for (i = 0; i < map->height; i++) {
		for (j = 0; j < map->width; j++) {
			if (map->grid[CELL(j,i)] == CELL_CASE) {
				map_set_cell_special(map, j, i, map_bonus_proba());
			}
		}
	}
}

extern int map_bonus_proba() {
	int random;
	random = rand() % 100 + 1;
	if (random <= PROBA_BONUS) //bonus
	{
		random = rand() % 100 + 1;
		if (random <= PROBA_BONUS_BOMB_RANGE) //bonus range bomb
		{
			random = rand() % 100 + 1;
			if (random <= PROBA_BONUS_BOMB_RANGE_INC) //bonus range bomb inc
			{
				return BONUS_BOMB_NB_INC;
			} else //bonus range bomb dec
			{
				return BONUS_BOMB_NB_DEC;
			}

		} else if (random - PROBA_BONUS_BOMB_RANGE <= PROBA_BONUS_MONSTER) //bonus monster
		{
			return BONUS_MONSTER;

		} else if (random - PROBA_BONUS_BOMB_RANGE - PROBA_BONUS_MONSTER // skill bonus
				<= PROBA_BONUS_SKILL) {
			random = rand() % 100 + 1;
			if (random <= PROBA_BONUS_LAUNCH) //bonus launch bomb
			{
				return BONUS_LAUNCH_BOMB;
			} else if (random - PROBA_BONUS_LAUNCH <= PROBA_BONUS_MULTI)//bonus multi bomb
			{
				return BONUS_MULTI_BOMB;
			} else {
				return BONUS_SPEED;
			}
		} else if (random - PROBA_BONUS_BOMB_RANGE - PROBA_BONUS_MONSTER // skill bonus
				- PROBA_BONUS_SKILL <= PROBA_BONUS_BOMB_NB) { //bonus number bomb
			random = rand() % 100 + 1;
			if (random <= PROBA_BONUS_BOMB_NB_INC) //bonus number bomb inc
			{
				return BONUS_BOMB_RANGE_INC;
			} else //bonus number bomb dec
			{
				return BONUS_BOMB_RANGE_DEC;
			}
		} else if (random - PROBA_BONUS_BOMB_RANGE - PROBA_BONUS_MONSTER // skill bonus
				- PROBA_BONUS_SKILL - PROBA_BONUS_BOMB_NB <= PROBA_DISEASE) {
			return BONUS_DISEASE;
		} else {
			return BONUS_LIFE;
		}
	} else // on a rien
	{
		return BONUS_NOTHING;
	}
}

t_map map_load_static(void) {
	t_map map = map_new(12, 12);
	t_cell_type themap[144] =
			{ CELL_PLAYER, CELL_CASE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
					CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_CASE,
					CELL_CASE, CELL_EMPTY, CELL_EMPTY, CELL_TREE, CELL_TREE,
					CELL_EMPTY, CELL_EMPTY, CELL_CASE, CELL_EMPTY, CELL_TREE,
					CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
					CELL_CASE, CELL_EMPTY, CELL_EMPTY, CELL_TREE, CELL_TREE,
					CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_CASE, CELL_CASE,
					CELL_EMPTY, CELL_CASE, CELL_CASE, CELL_TREE, CELL_TREE,
					CELL_TREE, CELL_EMPTY, CELL_CASE, CELL_EMPTY, CELL_EMPTY,
					CELL_CASE, CELL_EMPTY, CELL_EMPTY, CELL_CASE, CELL_TREE,
					CELL_CASE, CELL_EMPTY, CELL_EMPTY, CELL_CASE, CELL_CASE,
					CELL_CASE, CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_EMPTY,
					CELL_TREE, CELL_CASE, CELL_CASE, CELL_EMPTY, CELL_CASE,
					CELL_CASE, CELL_CASE, CELL_CASE, CELL_EMPTY, CELL_EMPTY,
					CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
					CELL_TREE, CELL_CASE, CELL_CASE, CELL_CASE, CELL_TREE,
					CELL_TREE, CELL_CASE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
					CELL_CASE, CELL_CASE, CELL_TREE, CELL_CASE, CELL_CASE,
					CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_CASE, CELL_CASE,
					CELL_EMPTY, CELL_CASE, CELL_EMPTY, CELL_EMPTY, CELL_TREE,
					CELL_TREE, CELL_CASE, CELL_EMPTY, CELL_EMPTY, CELL_CASE,
					CELL_CASE, CELL_CASE, CELL_TREE, CELL_EMPTY, CELL_TREE,
					CELL_TREE, CELL_TREE, CELL_CASE, CELL_EMPTY, CELL_TREE,
					CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_CASE, CELL_CASE,
					CELL_EMPTY, CELL_TREE, CELL_EMPTY, CELL_CASE, CELL_EMPTY,
					CELL_EMPTY, CELL_TREE, CELL_EMPTY, CELL_CASE, CELL_CASE,
					CELL_TREE, CELL_CASE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
					CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_TREE, CELL_TREE,
					CELL_EMPTY, CELL_EMPTY, CELL_TREE, CELL_FLAG };
	for (int i = 0; i < 144; i++)
		map->grid[i] = themap[i];
	map_convert_case(map);
	return map;
}

void map_display(t_map map, SDL_Surface *screen) {
	assert(map);
	assert(screen);
	int i, j;

	assert(map != NULL);
	assert(screen != NULL);
	assert(map->height > 0 && map->width > 0);
	assert(map->img_wall != NULL);
	assert(map->img_box != NULL);
	assert(map->img_goal != NULL);

	for (i = 0; i < map->width; i++) {
		for (j = 0; j < map->height; j++) {
			SDL_Rect place;
			place.x = i * SIZE_BLOC;
			place.y = j * SIZE_BLOC;

			t_cell_type type = map->grid[CELL(i,j)];

			switch (type & 0x0f) {
			case CELL_TREE:
				if (((type & 0xf0) >> 4) == 0)
					SDL_BlitSurface(map->img_wall, NULL, screen, &place);
				else if (((type & 0xf0) >> 4) < 3)
					SDL_BlitSurface(map->img_wall2, NULL, screen, &place);
				else if (((type & 0xf0) >> 4) < 5)
					SDL_BlitSurface(map->img_wall3, NULL, screen, &place);
				else if (((type & 0xf0) >> 4) < 7)
					SDL_BlitSurface(map->img_wall4, NULL, screen, &place);
				else if (((type & 0xf0) >> 4) == 7)
					SDL_BlitSurface(map->img_wall5, NULL, screen, &place);
				break;
			case CELL_CASE:
				SDL_BlitSurface(map->img_box, NULL, screen, &place);
				break;
			case CELL_FLAG:
				SDL_BlitSurface(map->img_goal, NULL, screen, &place);
				break;
			case CELL_SPAWN:
				SDL_BlitSurface(map->img_spawn, NULL, screen, &place);
				break;
			case CELL_BONUS:
				switch ((type & 0xf0) >> 4) {
				case BONUS_BOMB_RANGE_INC:
					SDL_BlitSurface(map->img_bonus_bomb_range_inc, NULL,
							screen, &place);
					break;

				case BONUS_BOMB_RANGE_DEC:
					SDL_BlitSurface(map->img_bonus_bomb_range_dec, NULL,
							screen, &place);
					break;

				case BONUS_BOMB_NB_INC:
					SDL_BlitSurface(map->img_bonus_bomb_nb_inc, NULL, screen,
							&place);
					break;

				case BONUS_BOMB_NB_DEC:
					SDL_BlitSurface(map->img_bonus_bomb_nb_dec, NULL, screen,
							&place);
					break;
				case BONUS_LAUNCH_BOMB:
					SDL_BlitSurface(map->img_bonus_launch_bomb, NULL, screen,
							&place);
					break;
				case BONUS_MULTI_BOMB:
					SDL_BlitSurface(map->img_bonus_multi_bomb, NULL, screen,
							&place);
					break;
				case BONUS_LIFE:
					SDL_BlitSurface(map->img_bonus_life, NULL, screen, &place);
					break;
				case BONUS_SPEED:
					SDL_BlitSurface(map->img_bonus_speed, NULL, screen, &place);
					break;
				case BONUS_DISEASE:
					SDL_BlitSurface(map->img_bonus_disease, NULL, screen,
							&place);
					break;
				}
			}
		}
	}
}

void map_grow_trees(t_map map) {
	assert(map);
	int i, j;
	for (i = 0; i < map->width; i++) {
		for (j = 0; j < map->height; j++) {
			t_cell_type type = map->grid[CELL(i,j)];
			if ((type & 0x0f) == CELL_TREE && ((type & 0xf0) >> 4) != 0) {
				map_set_cell_special(map, i, j, ((type & 0xf0) >> 4) - 1);
			}
		}

	}
}
