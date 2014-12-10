/*
 * constant.h
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent R�veill�re
 */

#ifndef CONSTANT_H_
#define CONSTANT_H_

// Name of the file that contains the map
#define SRC_MAP_SAVE         "data/save.lvl"
#define SRC_MAP1             "data/level1.lvl"
#define SRC_MAP2			 "data/level2.lvl"
#define SRC_MAP3             "data/level3.lvl"
#define SRC_MAP4			 "data/level4.lvl"
#define SRC_MAP5             "data/level5.lvl"
#define SRC_MAP6			 "data/level6.lvl"
#define SRC_MAP7			 "data/level7.lvl"
#define SRC_MAP8			 "data/level8.lvl"
#define SRC_MAP9			 "data/level9.lvl"

#define WINDOW_NAME "Atomic Bomberman!" //"[PG110] Projet 2011"
// Size (# of pixels) of a cell of the map
#define SIZE_BLOC       40
#define WINDOW_BPP      16

// Size (# of pixels) of banner
#define SIZE_LINE		4
#define SIZE_BANNER		40

// Time management
#define MAX_SKIPPED_FRAMES 20
#define DEFAULT_GAME_SPEED 70

/***********************
 * SPRITES
 */

// Sprites general
#define IMG_MAP_ROCK        "sprite/tree.png"
#define IMG_MAP_ROCK2       "sprite/tree2.png"
#define IMG_MAP_ROCK3       "sprite/tree3.png"
#define IMG_MAP_ROCK4       "sprite/tree4.png"
#define IMG_MAP_ROCK5       "sprite/tree5.png"
#define IMG_MAP_CASE        "sprite/wood_box.png"
#define IMG_MAP_FLAG        "sprite/flag.png"
#define IMG_RED_BLOOD       "sprite/red_blood.png"
#define IMG_GREEN_BLOOD     "sprite/green_blood.png"

// Sprites of Banner
#define IMG_BANNER_LINE		"sprite/banner_line.png"
#define IMG_BANNER_LIFE		"sprite/banner_life.png"
#define IMG_BANNER_BOMB		"sprite/bomb3.png"
#define IMG_BANNER_RANGE	"sprite/banner_range.png"
#define IMG_BANNER_0		"sprite/banner_0.jpg"
#define IMG_BANNER_1		"sprite/banner_1.jpg"
#define IMG_BANNER_2		"sprite/banner_2.jpg"
#define IMG_BANNER_3		"sprite/banner_3.jpg"
#define IMG_BANNER_4		"sprite/banner_4.jpg"
#define IMG_BANNER_5		"sprite/banner_5.jpg"
#define IMG_BANNER_6		"sprite/banner_6.jpg"
#define IMG_BANNER_7		"sprite/banner_7.jpg"
#define IMG_BANNER_8		"sprite/banner_8.jpg"
#define IMG_BANNER_9		"sprite/banner_9.jpg"

// Sprites of Bombs
#define IMG_BOMB_TTL1       "sprite/bomb1.png"
#define IMG_BOMB_TTL2       "sprite/bomb2.png"
#define IMG_BOMB_TTL3       "sprite/bomb3.png"
#define IMG_BOMB_TTL4       "sprite/bomb4.png"

// Sprites of fire, egg, spawn, win, lost, "J1","J2"
#define IMG_FIRE            "sprite/fire.png"
#define IMG_EGG				"sprite/egg.png"
#define IMG_SPAWN           "sprite/spawn.png"
#define IMG_GAGNE 			"sprite/gagne.png"
#define IMG_PERDU			"sprite/perdu.png"
#define IMG_J1              "sprite/J1.png"
#define IMG_J2              "sprite/J2.png"

// Sprites of Bonus
#define IMG_BONUS_BOMB_RANGE_INC  "sprite/bonus_bomb_range_inc.png"
#define IMG_BONUS_BOMB_RANGE_DEC  "sprite/bonus_bomb_range_dec.png"
#define IMG_BONUS_BOMB_NB_INC     "sprite/bonus_bomb_nb_inc.png"
#define IMG_BONUS_BOMB_NB_DEC     "sprite/bonus_bomb_nb_dec.png"
#define IMG_BONUS_MULTI_BOMB      "sprite/bonus_multi_bomb.png"
#define IMG_BONUS_LAUNCH_BOMB     "sprite/bonus_launch_bomb.png"
#define IMG_BONUS_SPEED           "sprite/bonus_speed.png"
#define IMG_BONUS_DISEASE         "sprite/bonus_disease.png"

// Sprites of Players
#define IMG_PLAYER_LEFT     "sprite/player_left.png"
#define IMG_PLAYER_UP       "sprite/player_up.png"
#define IMG_PLAYER_RIGHT    "sprite/player_right.png"
#define IMG_PLAYER_DOWN     "sprite/player_down.png"

// Sprites of Monsters
//classic
#define IMG_MONSTER_LEFT     "sprite/monster_left.png"
#define IMG_MONSTER_UP       "sprite/monster_up.png"
#define IMG_MONSTER_RIGHT    "sprite/monster_right.png"
#define IMG_MONSTER_DOWN     "sprite/monster_down.png"
//queen
#define IMG_MONSTER_QUEEN_LEFT     "sprite/queen_left.png"
#define IMG_MONSTER_QUEEN_UP       "sprite/queen_up.png"
#define IMG_MONSTER_QUEEN_RIGHT    "sprite/queen_right.png"
#define IMG_MONSTER_QUEEN_DOWN     "sprite/queen_down.png"
//builder
#define IMG_MONSTER_BUILDER_LEFT     "sprite/builder_left.png"
#define IMG_MONSTER_BUILDER_UP       "sprite/builder_up.png"
#define IMG_MONSTER_BUILDER_RIGHT    "sprite/builder_right.png"
#define IMG_MONSTER_BUILDER_DOWN     "sprite/builder_down.png"
//res
#define IMG_MONSTER_RES_LEFT     "sprite/res_left.png"
#define IMG_MONSTER_RES_UP       "sprite/res_up.png"
#define IMG_MONSTER_RES_RIGHT    "sprite/res_right.png"
#define IMG_MONSTER_RES_DOWN     "sprite/res_down.png"

/***********************
 * BOMBS and FIRE
 */
#define MAXBOMBES 30
#define MAXFIRE 200
// Maximum power of a bomb
#define BOMB_RANGE_MAX      9
// Default power of a bomb
#define BOMB_RANGE_DEFAULT	2
//Distance you can launch a bomb
#define LAUNCH_DISTANCE 1
// Lifetime of a bomb (ms)
#define BOMB_LIFETIME		4000
//Number of roll per sec
#define ROLL_BOMB_PER_SEC 10

// Lifetime of fire (ms)
#define FIRE_LIFETIME		500

/***********************
 * PLAYER
 */
#define MAXPLAYER 50
#define MAXBONUS 50
#define MAXBLOOD 50
#define MAXLIFE 9
// Number of Monster move per second
#define MONSTER_MOVE_PER_SEC 3
// Number of player move per second
#define PLAYER_MOVE_PER_SEC 5
// How much speed you earn when you pick up a speed bonus
#define AMOUNT_SPEED_BONUS 1

#define NB_START_LIFE 2
#define NB_START_BOMB 1
#define NB_MAX_BOMB 9
#define MAXSPEED 10
#define HP_QUEEN 3
//Queen
#define NB_START_RESSOURCES 20
#define PRICE_MONSTER_CLASSIC 20
#define PRICE_MONSTER_BUILDER 30
#define PRICE_MONSTER_RES 10
//builders
#define COST_RES_CASE 3
#define PROBA_SPAWN_CASE 10
#define MAX_BUILDER 3
#define MAX_RES 4
#define MAX_CLASSIC 10
//collecters
//number of collected resources in a cycle
#define NB_RES_PICK 1

/***********************
 * PROBABILITIES
 */
// Bonus
// Probability that a box contains a bonus
#define PROBA_BONUS 80

#define PROBA_BONUS_BOMB_RANGE 20
// Increase the power of a bomb +1
#define PROBA_BONUS_BOMB_RANGE_INC 50
// Decrease the power of a bomb -1
#define PROBA_BONUS_BOMB_RANGE_DEC 50

#define PROBA_BONUS_BOMB_NB    20
// Increase the number of bombs
#define PROBA_BONUS_BOMB_NB_INC  30
// Decrease the number of bombs
#define PROBA_BONUS_BOMB_NB_DEC  70

#define PROBA_BONUS_SKILL 20
// Gives the ability to throw bombs
#define PROBA_BONUS_LAUNCH 15
// Gives the ability to put multiple bombs
#define PROBA_BONUS_MULTI 15
// Gives the ability to go faster
#define PROBA_BONUS_SPEED 70

#define PROBA_DISEASE 15

#define PROBA_LIFE 5

#define PROBA_BONUS_MONSTER      20

//probability to place an egg
#define PROBA_PLACE_EGG 25

/***********************
 * OTHER
 */
//Speed of the growth of the trees
#define GROW_TREE_PER_SEC 0.25
//probability of eating a tree
#define PROBA_PICK_RES 2
//Respawn time of monsters (ms)
#define RESPAWN_TIME_MONSTER 1500
//ability they have ton eat each other
#define MONSTER_CANIBALISM 0
//Time you have to wait to be hit again (ms)
#define TIME_BETWEEN_TWO_HITS 1000
//players can respawn in Queen mod
#define SPAWNMODE 0
#define NB_DISEASE 10
#define DISEASE_MOVE 1
#define DISEASE_BOMBS 2
#define DISEASE_FIRE 3
#define DISEASE_FIRE2 4
#define DISEASE_SPEED 5
#define DISEASE_SPEED2 6
#define DISEASE_INC_BOMB 7
#define DISEASE_DEC_BOMB 8
#define DISEASE_FREEZE 9
#define DISEASE_TIME_BOMB 10
#define DISEASE_DURATION 10000
//Values of win/lost
#define PERDU 2
#define GAGNE 3
#define TIME_AFTER_WIN 2000

#endif /* CONSTANT */
