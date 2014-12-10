/*
 * game.h
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent R�veill�re
 */

#ifndef GAME_H_
#define GAME_H_

#include "../include/constant.h"
#include "../include/misc.h"

#include "../include/map.h"
#include "../include/player.h"
#include "../include/fire.h"
#include "../include/bomb.h"
#include "../include/IA.h"

enum game_mod {
	MOD_SOLO_CLASSIC = 0, MOD_MULTI_CLASSIC, MOD_MULTI_QUEEN
};

typedef struct s_game * t_game;
typedef struct s_blood * t_blood;
/**************
 * INIT/FREE
 */
// Create a new game
extern t_game game_new(int map, int compteurmode, int compte);
//free the game
extern void game_free(t_game game);
// Free the player
extern void game_free_player(t_game game, int i);
//free every player/monster with 0 lives
extern int game_free_dead_players(t_game game);
// Free the blood
extern void game_free_blood(t_game game, int i);

/**************
 * GETERS
 */
// Return the player of the current game
extern t_player game_get_player(t_game game, int id);
// Return the list of the players of the current game
extern t_player* game_get_players(t_game game);
// Return the id of player 1 or 2 or queen
extern int game_get_player1_id(t_game game);
extern int game_get_player2_id(t_game game);
extern int game_get_queen_id(t_game game);
// Return the map of the current game
extern t_map game_get_map(t_game game);
//return the list of the bombs of the current game
extern t_bomb* game_get_bombes(t_game game);

/***************
 * SETERS
 */
//set the id of the player 1 or 2 or queen to 'i'
extern void game_set_player1_id(t_game game, int i);
extern void game_set_player2_id(t_game game, int i);
extern void game_set_queen_id(t_game game, int i);
//set blood at x,y green(type=1) or red (type=0)
extern void game_set_blood(t_game game, int x, int y, int type);
//set a bomb at x,y (it can be an egg too, defined by the type)
extern void game_set_bomb(t_game game, int id, int type);
//set the mod of the game(solo/multi/queen)
extern void game_set_mod(t_game game, int mode);

/***************
 * OTHERS
 */
//save the game (save.lvl)
extern void save(t_game game, int countermode);
// Returns 1 if the player exists, 0 else
extern int game_is_player(t_game game, int i);
//Returns 1 if it is a monster, 0 else
extern int game_player_is_monster(t_game game, int i);
//make the player move (calling player_move)
extern void game_move_player(t_game game, int id1);
extern void game_do_move(t_game);
// This is for the smooth move
void game_smooth_rolling_bombs(t_game game);
void game_smooth_move(t_game game);
//checks if someone wins or lose
extern int game_test_win_lose(t_game game);
//Treatment of each cycle
extern void game_treatment(t_game game, int counter1);

/******************
 * Display
 */
//displays the map, the players, the bombs, the fire
extern void
game_display(t_game game, SDL_Surface *screen, int mode, int status);
//displays the blood
extern void game_blood_display(t_game game, SDL_Surface *screen);
//displays the banner for player1
extern void game_banner_display(t_game game, SDL_Surface *screen);
//displayes the banner for player2
extern void game_banner_display2(t_game game, SDL_Surface *screen);
//displays "gagné" or "perdu"
extern void game_display_end(t_game game, SDL_Surface *screen, int status);

#endif /* GAME_H_ */
