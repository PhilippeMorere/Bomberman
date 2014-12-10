/*
 * bomb.c
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

struct s_bomb {
	int time_left;
	int range;
	enum e_type_bomb type;
	int owner;
	int x;
	int y;
	float restx, resty;
	int rolling;
	int time_rolling;
	enum e_way way;
};

int bomb_get_time_left(t_bomb* bombes, int id) {
	assert(bombes);
	assert(bombes[id]);
	return bombes[id]->time_left;
}
int bomb_get_x(t_bomb* bombes, int id) {
	assert(bombes);
	assert(bombes[id]);
	return bombes[id]->x;
}
int bomb_get_y(t_bomb* bombes, int id) {
	assert(bombes);
	assert(bombes[id]);
	return bombes[id]->y;
}
int bomb_get_type(t_bomb* bombes, int id) {
	assert(bombes);
	assert(bombes[id]);
	return bombes[id]->type;
}
int bomb_get_way(t_bomb* bombes, int id) {
	assert(bombes);
	assert(bombes[id]);
	return bombes[id]->way;
}
int bomb_is_rolling(t_bomb* bombes, int id) {
	assert(bombes);
	assert(bombes[id]);
	return bombes[id]->rolling;
}
int bomb_get_range(t_bomb* bombes, int id) {
	assert(bombes);
	assert(bombes[id]);
	return bombes[id]->range;
}
int bomb_get_owner(t_bomb* bombes, int id) {
	assert(bombes);
	assert(bombes[id]);
	return bombes[id]->owner;
}
int bomb_get_id_by_xy(t_bomb* bombes, int x, int y) {
	assert(bombes);
	int i;
	for (i = 0; i < MAXBOMBES; i++) {
		if (bombes[i] != NULL && bombes[i]->x == x && bombes[i]->y == y)
			return i;
	}
	return -1;
}

void bomb_set_bomb(t_bomb* bombes, int x, int y, int range, int type,
		int owner, int duration) {
	assert(bombes);
	int i = 0;
	while (bombes[i] != NULL && i < MAXBOMBES) {
		i++;
	}
	if (i < MAXBOMBES) {
		t_bomb bomb = malloc(sizeof(*bomb));
		if (!bomb)
			error("Memory error");
		bomb->time_left = duration;
		bomb->rolling = 0;
		bomb->way = NORTH;
		bomb->type = type;
		bomb->range = range;
		bomb->x = x;
		bomb->y = y;
		bomb->restx = 0.0;
		bomb->resty = 0.0;
		bomb->owner = owner;
		bomb->time_rolling = 0;
		bombes[i] = bomb;
	}
}
void bomb_set_time_left(t_bomb* bombes, int id, int time) {
	assert(bombes);
	assert(bombes[id]);
	bombes[id]->time_left = time;
}
void bomb_set_x(t_bomb* bombes, int id, int x) {
	assert(bombes);
	assert(bombes[id]);
	bombes[id]->x = x;
}
void bomb_set_y(t_bomb* bombes, int id, int y) {
	assert(bombes);
	assert(bombes[id]);
	bombes[id]->y = y;
}
void bomb_set_way(t_bomb* bombes, int id, int way) {
	assert(bombes);
	assert(bombes[id]);
	bombes[id]->way = way;
}
void bomb_set_rolling(t_bomb* bombes, int id, int flag) {
	assert(bombes);
	assert(bombes[id]);
	bombes[id]->rolling = flag;
}
void bomb_set_range(t_bomb* bombes, int id, int range) {
	assert(bombes);
	assert(bombes[id]);
	bombes[id]->range = range;
}
void bomb_set_type(t_bomb* bombes, int id, int type) {
	assert(bombes);
	assert(bombes[id]);
	bombes[id]->type = type;
}
void bomb_set_explode_by_xy(t_bomb* bombes, int x, int y) {
	assert(bombes);
	int i = 0, trouve = 0;
	while (!trouve && i < MAXBOMBES) {
		if (bombes[i] != NULL) {
			if (bombes[i]->x == x && bombes[i]->y == y) {
				bombes[i]->time_left = 0;
				trouve = 1;
			}
		}
		i++;
	}
}

void bomb_remove(t_bomb* bombes, int id) {
	assert(bombes);
	assert(bombes[id]);
	free(bombes[id]);
	bombes[id] = NULL;
}
void bomb_remove_by_xy(t_bomb* bombes, int x, int y) {
	assert(bombes);
	int i = 0, trouve = 0;
	while (!trouve && i < MAXBOMBES) {
		if (bombes[i] != NULL && bombes[i]->x == x && bombes[i]->y == y)
			bomb_remove(bombes, i);
		i++;
	}

}
void bomb_dec_time_left(t_bomb* bombes, int id, int time) {
	assert(bombes);
	assert(bombes[id]);
	if (bombes[id]->time_left > 0) {
		if (bombes[id]->time_left >= time)
			bombes[id]->time_left -= time;
		else
			bombes[id]->time_left = 0;
	}
}
void bomb_decrement(t_bomb* bombes, int time) {
	assert(bombes);
	int i;
	for (i = 0; i < MAXBOMBES; i++) {
		if (bombes[i] != NULL) {
			bomb_dec_time_left(bombes, i, time);
		}
	}
}
void bomb_be_pushed(t_bomb* bombes, int id, int direction) {
	assert(bombes);
	assert(bombes[id]);
	if (bombes[id]->rolling == 0) {
		bombes[id]->way = direction;
		bombes[id]->rolling = 1;
		bombes[id]->time_rolling = 0;
	}

}
void bomb_inc_move_timer(t_bomb bomb, int time) {
	assert(bomb);
	bomb->time_rolling += time;
}
extern float bomb_get_move_timer(t_bomb bomb) {
	assert(bomb);
	return bomb->time_rolling;
}
void bomb_set_move_time(t_bomb bomb, float time) {
	assert(bomb);
	bomb->time_rolling = time;
}
extern void bomb_inc_restx(t_bomb bomb, float x) {
	assert(bomb);
	bomb->restx += x;
}
extern void bomb_inc_resty(t_bomb bomb, float y) {
	assert(bomb);
	bomb->resty += y;
}

int bomb_is_set(t_bomb* bombes, int id) {
	assert(bombes);
	if (bombes[id] != NULL)
		return 1;
	else
		return 0;
}
int bomb_is_at_xy(t_bomb* bombes, int x, int y) {
	assert(bombes);
	int i = 0;
	while (i < MAXBOMBES) {
		if (bombes[i] != NULL && bombes[i]->x == x && bombes[i]->y == y)
			return 1;
		i++;
	}
	return 0;
}
int bomb_time_to_explode_at_xy(t_map map, t_bomb* bombes, int x, int y) {
	assert(bombes);
	assert(map);
	int i, j, x1, y1, range, dist = 0, again = 0;
	for (i = 0; i < MAXBOMBES; i++) {
		if (bombes[i] != NULL) {
			again = 0;
			range = bombes[i]->range;
			x1 = bombes[i]->x;
			y1 = bombes[i]->y;
			if (x1 == x && (y <= y1 + range && y >= y1 - range)) {
				again = 1;
				dist = y - y1;
				if (y1 <= y) {
					j = 0;
					while (j <= dist && again) {
						if (!map_is_inside(map, x, y1 + j)
								|| !(map_get_cell_type(map, x, y1 + j)
										== CELL_EMPTY || map_get_cell_type(map,
										x, y1 + j) == CELL_BOMB))
							again = 0;
						j++;
					}
				} else {
					j = 0;
					while (j >= dist && again) {
						if (!map_is_inside(map, x, y1 + j)
								|| !(map_get_cell_type(map, x, y1 + j)
										== CELL_EMPTY || map_get_cell_type(map,
										x, y1 + j) == CELL_BOMB))
							again = 0;
						j--;
					}
				}
			} else if (y1 == y && (x <= x1 + range && x >= x1 - range)) {
				again = 1;
				dist = x - x1;
				if (x1 <= x) {
					j = 0;
					while (j <= dist && again) {
						if (!map_is_inside(map, x1 + j, y)
								|| !(map_get_cell_type(map, x1 + j, y)
										== CELL_EMPTY || map_get_cell_type(map,
										x1 + j, y) == CELL_BOMB))
							again = 0;
						j++;
					}
				} else {
					j = 0;
					while (j >= dist && again) {
						if (!map_is_inside(map, x1 + j, y)
								|| !(map_get_cell_type(map, x1 + j, y)
										== CELL_EMPTY || map_get_cell_type(map,
										x1 + j, y) == CELL_BOMB))
							again = 0;
						j--;
					}
				}
			}
			if (again)
				return bombes[i]->time_left;
		}
	}
	return -1;
}

void bomb_display(t_map map, t_bomb* bombes, SDL_Surface *screen) {
	assert(bombes);
	assert(screen);
	assert(map);
	int i;
	SDL_Rect place;
	for (i = 0; i < MAXBOMBES; i++) {
		if (bombes[i] != NULL) {
			place.x = bombes[i]->x * SIZE_BLOC + bombes[i]->restx;
			place.y = bombes[i]->y * SIZE_BLOC + bombes[i]->resty;
			if (bombes[i]->type == BOMB_STD) {
				if (bombes[i]->time_left > 3000)
					SDL_BlitSurface(map_get_imgbomb4(map), NULL, screen, &place);
				else if (bombes[i]->time_left > 2000)
					SDL_BlitSurface(map_get_imgbomb3(map), NULL, screen, &place);
				else if (bombes[i]->time_left > 1000)
					SDL_BlitSurface(map_get_imgbomb2(map), NULL, screen, &place);
				else if (bombes[i]->time_left > 0)
					SDL_BlitSurface(map_get_imgbomb1(map), NULL, screen, &place);
			} else {
				SDL_BlitSurface(map_get_imgegg(map), NULL, screen, &place);
			}
		}
	}
}

void bomb_destroy_case(t_player* players, t_map map, int x, int y) {
	assert(players);
	assert(map && map_is_inside(map,x,y));
	int bonus;
	bonus = map_get_cell_special(map, x, y);
	switch (bonus) {
	case BONUS_NOTHING:
		map_set_cell_type(map, x, y, CELL_EMPTY);
		break;
	case BONUS_BOMB_NB_DEC:
		map_set_cell_type(map, x, y, CELL_BONUS);
		map_set_cell_special(map, x, y, BONUS_BOMB_NB_DEC);
		break;
	case BONUS_BOMB_NB_INC:
		map_set_cell_type(map, x, y, CELL_BONUS);
		map_set_cell_special(map, x, y, BONUS_BOMB_NB_INC);
		break;
	case BONUS_BOMB_RANGE_INC:
		map_set_cell_type(map, x, y, CELL_BONUS);
		map_set_cell_special(map, x, y, BONUS_BOMB_RANGE_INC);
		break;
	case BONUS_BOMB_RANGE_DEC:
		map_set_cell_type(map, x, y, CELL_BONUS);
		map_set_cell_special(map, x, y, BONUS_BOMB_RANGE_DEC);
		break;
	case BONUS_LAUNCH_BOMB:
		map_set_cell_type(map, x, y, CELL_BONUS);
		map_set_cell_special(map, x, y, BONUS_LAUNCH_BOMB);
		break;
	case BONUS_MULTI_BOMB:
		map_set_cell_type(map, x, y, CELL_BONUS);
		map_set_cell_special(map, x, y, BONUS_MULTI_BOMB);
		break;
	case BONUS_LIFE:
		map_set_cell_type(map, x, y, CELL_BONUS);
		map_set_cell_special(map, x, y, BONUS_LIFE);
		break;
	case BONUS_SPEED:
		map_set_cell_type(map, x, y, CELL_BONUS);
		map_set_cell_special(map, x, y, BONUS_SPEED);
		break;
	case BONUS_DISEASE:
		map_set_cell_type(map, x, y, CELL_BONUS);
		map_set_cell_special(map, x, y, BONUS_DISEASE);
		break;
	case BONUS_MONSTER:
		player_new_monster(players, x, y, CLASSIC);
		map_set_cell_type(map, x, y, CELL_MONSTER);
		break;
	}
}
int bomb_explode_aux2(t_player* players, t_fire* fires, int id, t_map map,
		t_bomb* bombes, int x, int y) {
	assert(players);
	assert(map);
	int continuer = 1;
	if (map_is_inside(map, x, y)) {
		switch (map_get_cell_type(map, x, y)) {
		case CELL_TREE:
			continuer = 0;
			break;
		case CELL_CASE:
			continuer = 0;
			bomb_destroy_case(players, map, x, y);
			break;
		case CELL_EMPTY:
			fire_set_fire(fires, map, x, y);
			continuer = 1;
			break;
		case CELL_PLAYER:
			fire_set_fire(fires, map, x, y);
			continuer = 0;
			break;
		case CELL_MONSTER:
			fire_set_fire(fires, map, x, y);
			continuer = 0;
			break;
		case CELL_BONUS:
			map_set_cell_type(map, x, y, CELL_EMPTY);
			fire_set_fire(fires, map, x, y);
			continuer = 0;
			break;
		case CELL_BOMB:
			if (bomb_get_type(bombes, bomb_get_id_by_xy(bombes, x, y))
					== BOMB_STD) {
				bomb_set_explode_by_xy(bombes, x, y);
			} else {
				bomb_set_time_left(bombes, bomb_get_id_by_xy(bombes, x, y), -1);
			}
			/*if (player_is_player_at_xy(players, x, y) == PLAYER_STD)
			 map_set_cell_type(map, x, y, CELL_PLAYER);
			 else if (player_is_player_at_xy(players, x, y) == PLAYER_MONSTER)
			 map_set_cell_type(map, x, y, CELL_MONSTER);*/
			continuer = 0;
			break;
		case CELL_FLAG:
			continuer = 0;
			break;
		default:
			continuer = 0;
			fire_set_fire(fires, map, x, y);
			break;
		}
	} else
		continuer = 0;
	return continuer;
}
int bomb_explode_aux(t_player* players, t_fire* fires, t_bomb* bombes,
		t_map map, int id, int x, int y) {
	assert(players);
	assert(map && map_is_inside(map,x,y));
	assert(bombes);
	//faire tout péter dans la portée de la bombe i
	map_set_cell_type(map, x, y, CELL_EMPTY);
	fire_set_fire(fires, map, x, y);
	if (player_is_player_at_xy(players, x, y) == PLAYER_STD)
		map_set_cell_type(map, x, y, CELL_PLAYER);
	else if (player_is_player_at_xy(players, x, y) == PLAYER_MONSTER)
		map_set_cell_type(map, x, y, CELL_MONSTER);
	int i = 1;
	int continuer = 1;
	int range = bomb_get_range(bombes, id);
	while (i <= range && continuer) {
		continuer
				= bomb_explode_aux2(players, fires, id, map, bombes, x + i, y); //droite
		i++;
	}
	continuer = 1;
	i = 1;
	while (i <= range && continuer) {
		continuer
				= bomb_explode_aux2(players, fires, id, map, bombes, x - i, y); //gauche
		i++;
	}
	continuer = 1;
	i = 1;
	while (i <= range && continuer) {
		continuer
				= bomb_explode_aux2(players, fires, id, map, bombes, x, y + i); //bas
		i++;
	}
	continuer = 1;
	i = 1;
	while (i <= range && continuer) {
		continuer
				= bomb_explode_aux2(players, fires, id, map, bombes, x, y - i); //haut
		i++;
	}
	return 1;
}

void bomb_explode(t_player* players, t_fire* fires, t_bomb* bombes, t_map map) {
	assert(players);
	assert(map);
	assert(bombes);
	int i;
	for (i = 0; i < MAXBOMBES; i++) {
		if (bomb_is_set(bombes, i)) {
			int x = bomb_get_x(bombes, i);
			int y = bomb_get_y(bombes, i);
			if (bomb_get_time_left(bombes, i) <= 0
					|| fire_is_at_xy(fires, x, y)) {
				if (bomb_get_type(bombes, i) != BOMB_STD) { // Spawn a monster
					if (bomb_get_time_left(bombes, i) == -1) {
						map_set_cell_type(map, x, y, CELL_EMPTY);
					} else {
						switch (bomb_get_type(bombes, i)) {
						case EGG_CLASSIC:
							player_new_monster(players, x, y, MONSTER_CLASSIC);
							break;
						case EGG_RES:
							player_new_monster(players, x, y, MONSTER_RES);
							break;
						case EGG_BUILDER:
							player_new_monster(players, x, y, MONSTER_BUILDER);
							break;
						case EGG_QUEEN:
							player_new_monster(players, x, y, MONSTER_QUEEN);
							break;
						default:
							player_new_monster(players, x, y, MONSTER_CLASSIC);
							break;
						}
						map_set_cell_type(map, x, y, CELL_MONSTER);
					}
				} else { //explode the bomb
					bomb_explode_aux(players, fires, bombes, map, i, x, y);
				}
				if (player_exists(players, bombes[i]->owner)) //give back a bomb
					player_inc_nb_bomb(players, bombes[i]->owner);
				bomb_remove(bombes, i); //and free the bomb
			}
		}
	}
}

void bomb_roll(t_bomb* bombes, t_map map) {
	assert(bombes);
	assert(map);
	int i = 0, x = 0, y = 0;
	for (i = 0; i < MAXBOMBES; i++) {
		if (bombes[i] != NULL) {
			if (bombes[i]->rolling) {
				x = bombes[i]->x;
				y = bombes[i]->y;
				bombes[i]->restx = 0;
				bombes[i]->resty = 0;
				switch (bombes[i]->way) {
				case NORTH:
					if (map_is_inside(map, x, y - 1) && (map_get_cell_type(map,
							x, y - 1) == CELL_EMPTY || map_get_cell_type(map,
							x, y - 1) == CELL_BONUS)) {
						bombes[i]->y--;
						map_set_cell_type(map, x, y, CELL_EMPTY);
						map_set_cell_type(map, x, y - 1, CELL_BOMB);
					} else
						bombes[i]->rolling = 0;
					if (!(map_is_inside(map, x, y - 2) && (map_get_cell_type(
							map, x, y - 2) == CELL_EMPTY || map_get_cell_type(
							map, x, y - 2) == CELL_BONUS)))
						bombes[i]->rolling = 0;
					break;
				case SOUTH:
					if (map_is_inside(map, x, y + 1) && (map_get_cell_type(map,
							x, y + 1) == CELL_EMPTY || map_get_cell_type(map,
							x, y + 1) == CELL_BONUS)) {
						bombes[i]->y++;
						map_set_cell_type(map, x, y, CELL_EMPTY);
						map_set_cell_type(map, x, y + 1, CELL_BOMB);
					} else
						bombes[i]->rolling = 0;
					if (!(map_is_inside(map, x, y + 2) && (map_get_cell_type(
							map, x, y + 2) == CELL_EMPTY || map_get_cell_type(
							map, x, y + 2) == CELL_BONUS)))
						bombes[i]->rolling = 0;
					break;
				case WEST:
					if (map_is_inside(map, x - 1, y) && (map_get_cell_type(map,
							x - 1, y) == CELL_EMPTY || map_get_cell_type(map, x
							- 1, y) == CELL_BONUS)) {
						bombes[i]->x--;
						map_set_cell_type(map, x, y, CELL_EMPTY);
						map_set_cell_type(map, x - 1, y, CELL_BOMB);
					} else
						bombes[i]->rolling = 0;
					if (!(map_is_inside(map, x - 2, y) && (map_get_cell_type(
							map, x - 2, y) == CELL_EMPTY || map_get_cell_type(
							map, x - 2, y) == CELL_BONUS)))
						bombes[i]->rolling = 0;
					break;
				case EAST:
					if (map_is_inside(map, x + 1, y) && (map_get_cell_type(map,
							x + 1, y) == CELL_EMPTY || map_get_cell_type(map, x
							+ 1, y) == CELL_BONUS)) {
						bombes[i]->x++;
						map_set_cell_type(map, x, y, CELL_EMPTY);
						map_set_cell_type(map, x + 1, y, CELL_BOMB);
					} else
						bombes[i]->rolling = 0;
					if (!(map_is_inside(map, x + 2, y) && (map_get_cell_type(
							map, x + 2, y) == CELL_EMPTY || map_get_cell_type(
							map, x + 2, y) == CELL_BONUS)))
						bombes[i]->rolling = 0;
					break;
				}
			}
		}
	}
}
