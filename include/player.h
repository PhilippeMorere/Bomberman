/*
 * player.h
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent R�veill�re
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "../include/constant.h"

enum e_way {
	NORTH = 0, SOUTH, WEST, EAST
};

enum e_player {
	PLAYER_STD, PLAYER_MONSTER
};

enum e_type {
	CLASSIC = 0, QUEEN, BUILDER, RES
};

typedef struct s_player * t_player;
/************
 * GETERS
 */
extern int player_get_x(t_player player);
extern int player_get_y(t_player player);
extern int player_get_respawn_time(t_player player);
extern int player_get_advanced_type(t_player player);
extern int player_get_way(t_player player);
extern int player_get_type(t_player player);
extern int player_get_move_timer(t_player player);
extern int player_get_life(t_player player);
extern int player_get_range(t_player player);
extern int player_get_nb_bomb(t_player player); //bomb left
extern int player_get_nb_bomb_max(t_player player); //bomb max
extern int player_get_ressources(t_player player);
extern int player_get_speed(t_player player);
extern float player_get_restx(t_player player);
extern float player_get_resty(t_player player);
extern int player_get_next_move(t_player player);
extern t_player player_get_player(t_player* players, int id);
extern t_player player_get_player_by_xy(t_player* players, int x1, int y1);
extern int player_get_disease(t_player player);

/************
 * SETERS
 */
extern void player_set_x(t_player player, int x);
extern void player_set_y(t_player player, int y);
extern void player_set_life(t_player player, int life);
extern void player_set_respawn_time(t_player player, int time);
extern void player_set_current_way(t_player player, enum e_way way);
extern void player_set_movable(t_player player, int flag);
extern void player_set_restx(t_player player, float x);
extern void player_set_resty(t_player player, float y);
extern void player_set_nb_bomb(t_player* players, int id, int nb);
extern void player_set_range(t_player* players, int id, int nb);
extern void player_set_next_move(t_player player, int type);
extern void player_set_move_timer(t_player player, int value);
extern void player_set_speed(t_player player, int speed);

/**************
 * MODIFIERS
 */
extern void player_inc_posx(t_player player, int x);
extern void player_inc_posy(t_player player, int y);
extern void player_dec_respawn_time(t_player player, int time);
extern void player_inc_nb_bomb(t_player* players, int id);
extern void player_dec_nb_bomb(t_player player);
extern void player_inc_nb_bomb_total(t_player player);
extern void player_dec_nb_bomb_total(t_player player);
extern void player_dec_range_bomb(t_player player);
extern void player_inc_range_bomb(t_player player);
extern void player_dec_life(t_player player);
extern void player_inc_resty(t_player player, float y);
extern void player_inc_restx(t_player player, float x);
extern void player_dec_hit_time(t_player player, int time);
extern void player_dec_ressources(t_player player, int res);
extern void player_inc_move_timer(t_player player, int value);
extern void player_disease_decrement(t_player* players, int time);
//gives the bonus to the player
extern void player_apply_bonus(t_player player, t_bonus_type bonus);
//make the move of the player
extern int player_do_move(t_player* players, t_map map, int id);
extern int player_move(t_player* players, t_map map, int id);
extern int player_move_aux(t_player* players, int id, t_map map, int x1,
		int y1, int x2, int y2);

/**************
 * TESTERS
 */
//returns 1 if there is a player, 2 a monster
extern int player_on_map(t_map map);
extern int player_is_at_xy(t_player* players, int x, int y);
extern int player_is_movable(t_player player);
extern int player_exists(t_player* players, int id);
extern int player_has_launch(t_player player);
extern int player_has_multi(t_player player);
extern int player_has_foot(t_player player);
//return player_std or player_monster or -1
extern int player_is_player_at_xy(t_player* players, int x, int y);

/*************
 * OTHER
 */
//count the number of players of the type 'type'
extern int player_count(t_player* players, int type);
// Creates a new player with a given number of available bombs
extern t_player init_player_from_map(t_map map, int bomb_number);
extern t_player player_init(int bomb_number, int type, int type_advanced);
//free the player
extern void player_free(t_player player);
// Load the player/monster position from the map
extern void player_from_map(t_player player, t_map map);
//displays the player
extern void player_display(t_player player, SDL_Surface *screen);
//if the case can be pushed (nothing behind in x,y)
int case_is_pushable(t_map map, int x, int y);
//create a monster
void player_new_monster(t_player* players, int x, int y, t_monster_type type);
//this is for monsters_collectors, they give ressources to the queen and the tree is cut a little
extern void player_collect_ressources(t_player* players, t_map map, int x,
		int y, int queen_id);
//this is for monsters_builders, they can add a case in x,y
extern void player_put_a_case(t_map map, int x, int y);

#endif /* PLAYER_H_ */
