/*
 * player.c
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

struct s_player {
	int x, y;
	float restx, resty;
	SDL_Surface * directed_img[4];
	enum e_way current_way;
	enum e_player type;
	enum e_type type_advanced;
	int next_move;
	int respawn;
	int movable;
	int move_timer;
	int speed;
	int life;
	int last_hit;
	int power;
	int nb_bomb;
	int nb_bomb_total;
	int ressources;
	int ability_launch;
	int ability_multi;
	int ability_foot;
	int disease;
	int time_disease;
};

t_player init_player_from_map(t_map map, int bomb_number) {
	int i, j, x = 0, y = 0, type = 0, type_advanced = 0, trouve = 0;
	t_player player;
	i = -1;
	while (i < map_get_width(map) - 1 && !trouve) {
		i++;
		j = -1;
		while (j < map_get_width(map) - 1 && !trouve) {
			j++;
			if (map_get_cell_type(map, i, j) == CELL_PLAYER) {
				x = i;
				y = j;
				type = PLAYER_STD;
				type_advanced = 0;
				trouve = 1;
				map_set_cell_type(map, i, j, CELL_EMPTY);
			}
		}
	}
	if (!trouve) {
		i = -1;
		while (i < map_get_width(map) - 1 && !trouve) {
			i++;
			j = -1;
			while (j < map_get_width(map) - 1 && !trouve) {
				j++;
				if (map_get_cell_type(map, i, j) == CELL_MONSTER) {
					x = i;
					y = j;
					type = PLAYER_MONSTER;
					switch (map_get_cell_special(map, i, j)) {
					case MONSTER_CLASSIC:
						type_advanced = CLASSIC;
						break;
					case MONSTER_QUEEN:
						type_advanced = QUEEN;
						break;
					case MONSTER_BUILDER:
						type_advanced = BUILDER;
						break;
					case MONSTER_RES:
						type_advanced = RES;
						break;
					}

					trouve = 1;
					map_set_cell_type(map, i, j, CELL_EMPTY);
				}
			}
		}
	}
	if (trouve) {
		player = player_init(bomb_number, type, type_advanced);
		player->x = x;
		player->y = y;
		return player;
	} else
		return NULL;
}

t_player player_init(int bomb_number, int type, int type_advanced) {
	t_player player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->current_way = SOUTH;
	player->nb_bomb = bomb_number;
	player->nb_bomb_total = player->nb_bomb;
	player->ability_launch = 0;
	player->ability_multi = 0;
	player->ability_foot = 0;
	player->last_hit = 0;
	player->movable = 1;
	player->move_timer = 0;
	player->restx = 0.0;
	player->resty = 0.0;
	player->next_move = -1;
	player->disease = 0;
	player->time_disease = 0;
	player->power = BOMB_RANGE_DEFAULT;
	if (type == PLAYER_STD) { //init a player
		player->type = PLAYER_STD;
		player->directed_img[WEST] = load_image(IMG_PLAYER_LEFT);
		player->directed_img[EAST] = load_image(IMG_PLAYER_RIGHT);
		player->directed_img[NORTH] = load_image(IMG_PLAYER_UP);
		player->directed_img[SOUTH] = load_image(IMG_PLAYER_DOWN);
		player->life = NB_START_LIFE;
		player->respawn = 0;
		player->type_advanced = 0;
		player->ressources = 0;
		player->speed = PLAYER_MOVE_PER_SEC;
	} else { //init a monster
		if (type_advanced == CLASSIC) {
			player->directed_img[WEST] = load_image(IMG_MONSTER_LEFT);
			player->directed_img[EAST] = load_image(IMG_MONSTER_RIGHT);
			player->directed_img[NORTH] = load_image(IMG_MONSTER_UP);
			player->directed_img[SOUTH] = load_image(IMG_MONSTER_DOWN);
			player->life = 1;
			player->ressources = 0;
		} else if (type_advanced == QUEEN) {
			player->directed_img[WEST] = load_image(IMG_MONSTER_QUEEN_LEFT);
			player->directed_img[EAST] = load_image(IMG_MONSTER_QUEEN_RIGHT);
			player->directed_img[NORTH] = load_image(IMG_MONSTER_QUEEN_UP);
			player->directed_img[SOUTH] = load_image(IMG_MONSTER_QUEEN_DOWN);
			player->life = 3;
			player->ressources = NB_START_RESSOURCES;
			player->ability_launch = 1;
		} else if (type_advanced == BUILDER) {
			player->directed_img[WEST] = load_image(IMG_MONSTER_BUILDER_LEFT);
			player->directed_img[EAST] = load_image(IMG_MONSTER_BUILDER_RIGHT);
			player->directed_img[NORTH] = load_image(IMG_MONSTER_BUILDER_UP);
			player->directed_img[SOUTH] = load_image(IMG_MONSTER_BUILDER_DOWN);
			player->life = 1;
			player->ressources = 0;
		} else if (type_advanced == RES) {
			player->directed_img[WEST] = load_image(IMG_MONSTER_RES_LEFT);
			player->directed_img[EAST] = load_image(IMG_MONSTER_RES_RIGHT);
			player->directed_img[NORTH] = load_image(IMG_MONSTER_RES_UP);
			player->directed_img[SOUTH] = load_image(IMG_MONSTER_RES_DOWN);
			player->life = 1;
			player->ressources = 0;
		}
		player->type = PLAYER_MONSTER;
		player->respawn = RESPAWN_TIME_MONSTER;
		player->type_advanced = type_advanced;
		player->speed = MONSTER_MOVE_PER_SEC;
	}
	return player;
}

void player_free(t_player player) {
	assert(player);
	int i;
	for (i = 0; i < 4; i++)
		assert(player->directed_img[i]);
	for (i = 0; i < 4; i++) {
		SDL_FreeSurface(player->directed_img[i]);
	}
	free(player);
}

int player_get_x(t_player player) {
	assert(player);
	return player->x;
}

int player_get_y(t_player player) {
	assert(player);
	return player->y;
}

void player_set_x(t_player player, int x) {
	assert(player);
	player->x = x;
}
void player_set_y(t_player player, int y) {
	assert(player);
	player->y = y;
}

int player_get_respawn_time(t_player player) {
	assert(player);
	return player->respawn;
}
void player_set_respawn_time(t_player player, int time) {
	assert(player);
	player->respawn = time;
}

void player_inc_posx(t_player player, int x) {
	assert(player);
	player->x += x;
}
void player_inc_posy(t_player player, int y) {
	assert(player);
	player->y += y;
}

void player_dec_respawn_time(t_player player, int time) {
	assert(player);
	player->respawn -= time;
}

int player_get_type(t_player player) {
	assert(player);
	return player->type;
}
int player_get_move_timer(t_player player) {
	assert(player);
	return player->move_timer;
}
int player_get_advanced_type(t_player player) {
	assert(player);
	return player->type_advanced;
}
int player_get_way(t_player player) {
	assert(player);
	return player->current_way;
}
int player_get_life(t_player player) {
	assert(player);
	return player->life;
}
void player_set_life(t_player player, int life) {
	assert(player);
	player->life = life;
}

int player_get_range(t_player player) {
	assert(player);
	return player->power;
}

void player_set_current_way(t_player player, enum e_way way) {
	assert(player);
	player->current_way = way;
}

void player_set_movable(t_player player, int flag) {
	assert(player);
	if (flag)
		player->movable = 1;
	else
		player->movable = 0;
}

void player_set_restx(t_player player, float x) {
	assert(player);
	player->restx = x;
}
void player_set_resty(t_player player, float y) {
	assert(player);
	player->resty = y;
}
void player_set_nb_bomb(t_player* players, int id, int nb) {
	assert(players);
	players[id]->nb_bomb_total = nb;
	players[id]->nb_bomb = nb;
}
void player_set_range(t_player* players, int id, int nb) {
	assert(players);
	players[id]->power = nb;
}
void player_set_next_move(t_player player, int type) {
	assert(player);
	player->next_move = type;
}
void player_set_move_timer(t_player player, int value) {
	assert(player);
	player->move_timer = value;
}
void player_set_speed(t_player player, int speed) {
	assert(player);
	player->speed = speed;
	if (player->speed > MAXSPEED)
		player->speed = MAXSPEED;
}
int player_get_nb_bomb(t_player player) {
	assert(player);
	return player->nb_bomb;
}
int player_get_nb_bomb_max(t_player player) {
	assert(player);
	return player->nb_bomb_total;
}

int player_get_ressources(t_player player) {
	assert(player);
	return player->ressources;
}
int player_get_speed(t_player player) {
	assert(player);
	return player->speed;
}
float player_get_restx(t_player player) {
	assert(player);
	return player->restx;
}
float player_get_resty(t_player player) {
	assert(player);
	return player->resty;
}

void player_inc_nb_bomb(t_player* players, int id) {
	assert(players);
	assert(players[id]);
	if (players[id]->nb_bomb < players[id]->nb_bomb_total)
		players[id]->nb_bomb += 1;
}

void player_dec_nb_bomb(t_player player) {
	assert(player);
	player->nb_bomb -= 1;
}

void player_inc_nb_bomb_total(t_player player) {
	assert(player);
	if (player->nb_bomb_total < NB_MAX_BOMB) {
		player->nb_bomb_total += 1;
		player->nb_bomb += 1;
	}
}

void player_dec_nb_bomb_total(t_player player) {
	assert(player);
	if (player->nb_bomb_total > NB_START_BOMB) {
		player->nb_bomb_total -= 1;
		if (player->nb_bomb > 0)
			player->nb_bomb -= 1;
	}
}

void player_dec_range_bomb(t_player player) {
	assert(player);
	if (player->power > BOMB_RANGE_DEFAULT)
		player->power--;
}

void player_inc_range_bomb(t_player player) {
	assert(player);
	if (player->power < BOMB_RANGE_MAX)
		player->power++;
}

void player_dec_life(t_player player) {
	assert(player);
	if (player->life > 0 && player->last_hit <= 0) {
		player->life--;
		player->last_hit = TIME_BETWEEN_TWO_HITS;
	}
}
void player_inc_life(t_player player) {
	assert(player);
	if (player->life < MAXLIFE) {
		player->life++;
	}
}

void player_inc_restx(t_player player, float x) {
	assert(player);
	player->restx += x;
}
void player_inc_resty(t_player player, float y) {
	assert(player);
	player->resty += y;
}

void player_dec_hit_time(t_player player, int time) {
	assert(player);
	if (player->last_hit > 0)
		player->last_hit -= time;
}

void player_dec_ressources(t_player player, int res) {
	assert(player);
	player->ressources -= res;
}
void player_inc_move_timer(t_player player, int value) {
	assert(player);
	player->move_timer += value;
}
void player_disease_decrement(t_player* players, int time) {
	assert(players);
	int i = 0;
	for (i = 0; i < MAXPLAYER; i++) {
		if (players[i] != NULL) {
			if (players[i]->disease != 0 && players[i]->time_disease >= 0) {
				players[i]->time_disease -= time;
			} else {
				players[i]->disease = 0;
				players[i]->time_disease = 0;
			}
		}
	}
}
void player_from_map(t_player player, t_map map) {
	assert(player);
	assert(map);
	int i = 0, j = 0, trouve = 0;
	while (i < map_get_width(map) && !trouve) {
		j = 0;
		while (j < map_get_height(map) && !trouve) {
			if (map_get_cell_type(map, i, j) == CELL_PLAYER) {
				player->x = i;
				player->y = j;
				trouve = 1;
				map_set_cell_type(map, i, j, CELL_EMPTY);
			}
			j++;
		}
		i++;
	}
}
int player_on_map(t_map map) {
	assert(map);
	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
		for (j = 0; j < map_get_height(map); j++) {
			if (map_get_cell_type(map, j, i) == CELL_PLAYER
					|| map_get_cell_type(map, j, i) == CELL_MONSTER)
				return 1;
		}
	}
	return 0;
}

int player_is_at_xy(t_player* players, int x, int y) {
	assert(players);
	int i = 0, trouve = 0;
	while (!trouve && i < MAXPLAYER) {
		if (players[i] != NULL) {
			if (player_get_x(players[i]) == x && player_get_y(players[i]) == y)
				trouve = 1;
		}
		i++;
	}
	return trouve;
}
int player_is_movable(t_player player) {
	assert(player);
	return player->movable;
}
int player_exists(t_player* players, int id) {
	assert(players);
	if (id < 0)
		return 0;
	else if (id >= MAXPLAYER)
		return 0;
	else if (players[id] == NULL)
		return 0;
	else
		return 1;
}
int player_has_launch(t_player player) {
	assert(player);
	if (player->ability_launch)
		return 1;
	else
		return 0;
}
int player_has_multi(t_player player) {
	assert(player);
	if (player->ability_multi)
		return 1;
	else
		return 0;
}
int player_has_foot(t_player player) {
	assert(player);
	if (player->ability_foot)
		return 1;
	else
		return 0;
}
int case_is_pushable(t_map map, int x, int y) {
	assert(map);
	if (map_is_inside(map, x, y) && (map_get_cell_type(map, x, y) == CELL_EMPTY
			|| map_get_cell_type(map, x, y) == CELL_BONUS)) {
		return 1;
	} else {
		return 0;
	}
}

t_player player_get_player(t_player* players, int id) {
	assert(players);
	t_player player = NULL;
	if (id >= 0 && id < MAXPLAYER) {
		player = (players)[id];
		assert(player);
		return player;
	}
	return player;
}
int player_get_next_move(t_player player) {
	assert(player);
	return player->next_move;
}
t_player player_get_player_by_xy(t_player* players, int x1, int y1) {
	assert(players);
	int i = 0, trouve = 0;
	t_player player = NULL;
	while (i < MAXPLAYER && !trouve) {
		if ((players)[i] != NULL) {
			player = player_get_player(players, i);
			if (player_get_x(player) == x1 && player_get_y(player) == y1) {
				trouve = 1;
			}
		}
		i++;
	}
	return player;
}
int player_get_disease(t_player player) {
	assert(player);
	return player->disease;
}
int player_is_player_at_xy(t_player* players, int x, int y) {
	assert(players);
	int i = 0;
	t_player player = NULL;
	for (i = 0; i < MAXPLAYER; i++) {
		if (players[i] != NULL) {
			player = player_get_player(players, i);
			if (player_get_x(player) == x && player_get_y(player) == y) {
				return player_get_type(player);
			}
		}
	}
	return -1;
}

int player_count(t_player* players, int type) {
	assert(players);
	int i, compteur = 0;
	for (i = 0; i < MAXPLAYER; i++) {
		if (players[i] != NULL && players[i]->type_advanced == type)
			compteur++;
	}
	return compteur;
}

int player_move_aux(t_player* players, int id, t_map map, int x1, int y1,
		int x2, int y2) {
	assert(players);
	assert(map);
	t_player player = player_get_player(players, id);
	if (!map_is_inside(map, x1, y1))
		return 0;

	switch (map_get_cell_type(map, x1, y1)) {
	case CELL_TREE:
		return 0;
		break;

	case CELL_CASE:
		//only the player can push cases
		if (player_get_type(player) == PLAYER_STD) {
			if (map_is_inside(map, x2, y2) && case_is_pushable(map, x2, y2)) {
				map_set_cell_type(map, x2, y2, CELL_CASE);//pousser la caisse
				map_set_cell_special(map, x2, y2, map_get_cell_special(map, x1,
						y1));
				return 1;
			} else {
				return 0;
			}
		} else
			return 0;
		break;
	case CELL_BONUS:
		if (player_get_type(player) == PLAYER_STD)
			player_apply_bonus(player, map_get_cell_special(map, x1, y1));
		map_set_cell_special(map, x1, y1, 0);
		return 1;
		break;
	case CELL_BOMB:
		if (player_get_type(player) == PLAYER_STD && player->ability_foot
				&& case_is_pushable(map, x2, y2)) {
			return 5;
		}
		return 0;
		break;
	case CELL_SPAWN:
		if (player_get_type(player) == PLAYER_STD) {
			player_set_life(player, NB_START_LIFE);
			return 1;
		} else
			return 0;
		break;
	case CELL_FLAG:
		if (player_get_type(player) == PLAYER_STD)
			return 1;
		else
			return 0;
		break;
	case CELL_MONSTER:
		if (player_get_type(player) == PLAYER_MONSTER && MONSTER_CANIBALISM)
			player_dec_life(player_get_player_by_xy(players, x1, y1));
		return 0;
		break;

	case CELL_PLAYER:
		if (player_get_type(player) == PLAYER_MONSTER)
			player_dec_life(player_get_player_by_xy(players, x1, y1));
		return 0;
		break;

	default:
		return 1;
		break;
	}

	// Player has moved
	return 1;
}

int player_move(t_player* players, t_map map, int id) {
	assert(players);
	assert(map);
	assert(players[id]);
	t_player player = player_get_player(players, id);
	int x = player->x;
	int y = player->y;
	int result;
	int move = 0;
	if (player->movable) {
		switch (player->current_way) {
		case NORTH:
			if ((result = player_move_aux(players, id, map, x, y - 1, x, y - 2))
					== 1) {
				player->y--;
				move = 1;
				player->movable = 0;
				player->resty = SIZE_BLOC;
			} else if (result == 5) {
				return 5;
			}
			break;
		case SOUTH:
			if ((result = player_move_aux(players, id, map, x, y + 1, x, y + 2))
					== 1) {
				player->y++;
				move = 1;
				player->movable = 0;
				player->resty = -SIZE_BLOC;
			} else if (result == 5) {
				return 5;
			}
			break;
		case WEST:
			if ((result = player_move_aux(players, id, map, x - 1, y, x - 2, y))
					== 1) {
				player->x--;
				move = 1;
				player->movable = 0;
				player->restx = SIZE_BLOC;
				;
			} else if (result == 5) {
				return 5;
			}
			break;
		case EAST:
			if ((result = player_move_aux(players, id, map, x + 1, y, x + 2, y))
					== 1) {
				player->x++;
				move = 1;
				player->movable = 0;
				player->restx = -SIZE_BLOC;
			} else if (result == 5) {
				return 5;
			}
			break;
		}
	}
	if (move && map_get_cell_type(map, x, y) == CELL_SPAWN) {
		map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
		map_set_cell_type(map, x, y, CELL_SPAWN);
	} else if (move && map_get_cell_type(map, player->x, player->y)
			== CELL_SPAWN) {
		map_set_cell_type(map, player->x, player->y, CELL_SPAWN);
		map_set_cell_type(map, x, y, CELL_EMPTY);
	} else if (move && map_get_cell_type(map, x, y) == CELL_BOMB) {
		if (player->type == PLAYER_STD)
			map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
		else
			map_set_cell_type(map, player->x, player->y, CELL_MONSTER);
	} else if (move && map_get_cell_type(map, player->x, player->y)
			== CELL_FLAG)
		;
	else if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
		if (player->type == PLAYER_STD)
			map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
		else
			map_set_cell_type(map, player->x, player->y, CELL_MONSTER);
	}
	return move;
}

void player_display(t_player player, SDL_Surface *screen) {
	assert(player);
	assert(player->directed_img[player->current_way]);
	assert(screen);

	//player
	SDL_Rect place;
	place.x = player->x * SIZE_BLOC + player->restx;
	place.y = player->y * SIZE_BLOC + player->resty;
	SDL_BlitSurface(player->directed_img[player->current_way], NULL, screen,
			&place);
}

void player_apply_bonus(t_player player, t_bonus_type bonus) {
	assert(player);
	int random = 0, i = 0;
	switch (bonus) {
	case BONUS_BOMB_RANGE_INC:
		player_inc_range_bomb(player);
		break;
	case BONUS_BOMB_RANGE_DEC:
		player_dec_range_bomb(player);
		break;
	case BONUS_BOMB_NB_INC:
		player_inc_nb_bomb_total(player);
		break;
	case BONUS_BOMB_NB_DEC:
		player_dec_nb_bomb_total(player);
		break;
	case BONUS_LAUNCH_BOMB:
		player->ability_foot = 1;
		break;
	case BONUS_MULTI_BOMB:
		player->ability_multi = 1;
		break;
	case BONUS_LIFE:
		player_inc_life(player);
		break;
	case BONUS_SPEED:
		player_set_speed(player, player_get_speed(player) + AMOUNT_SPEED_BONUS);
		break;
	case BONUS_DISEASE:
		random = rand() % NB_DISEASE + 1;
		if (random == DISEASE_INC_BOMB) {
			for (i = 0; i < 5; i++)
				player_inc_nb_bomb_total(player);
		} else if (random == DISEASE_DEC_BOMB) {
			for (i = 0; i < 5; i++)
				player_dec_nb_bomb_total(player);
		} else {

			player->disease = random;
			player->time_disease = DISEASE_DURATION;
			break;
			default:;
			break;
		}
	}
}

void player_new_monster(t_player* players, int x, int y, t_monster_type type) {
	assert(players);
	int i = 0;
	while (players[i] != NULL)
		i++;
	players[i] = player_init(NB_START_BOMB, PLAYER_MONSTER, type);
	player_set_x(players[i], x);
	player_set_y(players[i], y);
}

void player_collect_ressources(t_player* players, t_map map, int x, int y,
		int queen_id) {
	assert(players);
	assert(map);
	int random;
	random = rand() % 100 + 1;
	if (random <= PROBA_PICK_RES) {
		player_dec_ressources(players[queen_id], -NB_RES_PICK);
		int type = map_get_cell_special(map, x, y);
		if (type != 7)
			map_set_cell_special(map, x, y, type + 1);
	}
}

void player_put_a_case(t_map map, int x, int y) {
	assert(map);
	map_set_cell_type(map, x, y, CELL_CASE);
	map_set_cell_special(map, x, y, map_bonus_proba());
}

