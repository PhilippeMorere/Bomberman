/*
 * ia.c
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

struct s_ia {
	int height;
	int width;
	int level;
};
#define CELL1(i,j) ((i) +  ia->width * (j))

t_ia IA_new(int width, int height, int level) {
	assert(width > 0 && height > 0);
	t_ia ia = malloc(sizeof(t_ia));
	if (ia == NULL)
		exit(EXIT_FAILURE);
	ia->height = height;
	ia->width = width;
	ia->level = level;
	return ia;
}
void IA_free(t_ia ia) {
	assert(ia);
	free(ia);
}
void IA_do(t_map map, t_player* players, t_bomb* bombes, t_fire* fires,
		t_ia ia, int queenid, int id1, int id2) {
	assert(map);
	assert(players);
	assert(bombes);
	assert(fires);
	assert(ia);
	int id;
	for (id = 0; id < MAXPLAYER; id++) {
		if (players[id] != NULL && player_is_movable(players[id])) {
			if (player_get_type(player_get_player(players, id))
					== PLAYER_MONSTER && player_get_respawn_time(
					player_get_player(players, id)) <= 0) {
				if (player_get_advanced_type(player_get_player(players, id))
						== CLASSIC) {
					switch (ia->level) {
					case 1:
						IA_1(map, players, bombes, fires, ia, id);
						break;
					case 2:
						IA_2(map, players, bombes, fires, ia, id, id1, id2);
						break;
					case 3:
						IA_3(map, players, bombes, fires, ia, id, id1, id2);
						break;
					default:
						IA_1(map, players, bombes, fires, ia, id);
						break;
					}
				} else if (player_get_advanced_type(player_get_player(players,
						id)) == QUEEN)
					IA_queen(map, players, bombes, fires, ia, id);
				else if (player_get_advanced_type(
						player_get_player(players, id)) == RES)
					IA_res(map, players, bombes, fires, ia, id, queenid);
				else if (player_get_advanced_type(
						player_get_player(players, id)) == BUILDER)
					IA_builder(map, players, bombes, fires, ia, id, queenid);
			}
		}
	}
}
void IA_1(t_map map, t_player* players, t_bomb* bombes, t_fire* fires, t_ia ia,
		int id) {
	assert(map);
	assert(players);
	assert(players[id]);
	assert(bombes);
	assert(fires);
	assert(ia);
	int max = 0;
	do {
		player_set_current_way(player_get_player(players, id), rand() % 4);
		max++;
	} while (!player_move(players, map, id) && max < 10);
	if (max == 10)
		player_set_respawn_time(players[id], RESPAWN_TIME_MONSTER);
}

void IA_2(t_map map, t_player* players, t_bomb* bombes, t_fire* fires, t_ia ia,
		int id, int id1, int id2) {
	// This IA search the player and avoids the fires
	int temp = player_get_x(players[id]);
	int temp1 = player_get_y(players[id]);

	if (id2 != -1) { // player 2 is present or not present ?
		int distance1 = ((player_get_x(players[id1]) - temp) // calculating the distance between the monster and the player one and two
				* (player_get_x(players[id1]) - temp) + (player_get_y(
				players[id1]) - temp1) * (player_get_y(players[id1]) - temp1));
		int distance2 = ((player_get_x(players[id2]) - temp) * (player_get_x(
				players[id2]) - temp) + (player_get_y(players[id2]) - temp1)
				* (player_get_y(players[id2]) - temp1));

		if (distance1 < distance2) { // the monster attacks the player one
			if ((player_get_x(players[id1]) < player_get_x(players[id])) // directions
					&& fire_is_at_xy(fires, player_get_x(players[id]) - 1,
							player_get_y(players[id])) == 0) {

				player_set_current_way(players[id], 2);
				player_move(players, map, id);
			} else if ((player_get_y(players[id1]) < player_get_y(players[id]))
					&& fire_is_at_xy(fires, player_get_x(players[id]),
							player_get_y(players[id]) - 1) == 0) {

				player_set_current_way(players[id], 0);
				player_move(players, map, id);
			} else if ((player_get_x(players[id1]) > player_get_x(players[id]))
					&& fire_is_at_xy(fires, player_get_x(players[id]) + 1,
							player_get_y(players[id])) == 0) {

				player_set_current_way(players[id], 3);
				player_move(players, map, id);
			} else if ((player_get_y(players[id1]) > player_get_y(players[id]))
					&& fire_is_at_xy(fires, player_get_x(players[id]),
							player_get_y(players[id]) + 1) == 0) {

				player_set_current_way(players[id], 1);
				player_move(players, map, id);
			}

			int z = rand() % 2;
			if (z == 0) {
				int i = 0;
				while (player_get_x(players[id]) == temp && player_get_y(
						players[id]) == temp1 && i < 4) {
					switch (i) {
						case 0:
							if (map_is_inside(map, temp, temp1 - 1)
									&& fire_is_at_xy(fires, player_get_x(
											players[id]), player_get_y(players[id])
											- 1) == 0 && (map_get_cell_type(map,
									temp, temp1 - 1) == CELL_EMPTY
									|| map_get_cell_type(map, temp, temp1 - 1)
											== CELL_BONUS)) {
								player_set_current_way(players[id], i);
								player_move(players, map, id);
							}
							break;
						case 1:
							if (map_is_inside(map, temp, temp1 + 1)
									&& fire_is_at_xy(fires, player_get_x(
											players[id]), player_get_y(players[id])
											+ 1) == 0 && (map_get_cell_type(map,
									temp, temp1 + 1) == CELL_EMPTY
									|| map_get_cell_type(map, temp, temp1 + 1)
											== CELL_BONUS)) {
								player_set_current_way(players[id], i);
								player_move(players, map, id);
							}
							break;
						case 2:
							if (map_is_inside(map, temp - 1, temp1)
									&& fire_is_at_xy(fires, player_get_x(
											players[id]) - 1, player_get_y(
											players[id])) == 0
									&& (map_get_cell_type(map, temp - 1, temp1)
											== CELL_EMPTY || map_get_cell_type(map,
											temp - 1, temp1) == CELL_BONUS)) {
								player_set_current_way(players[id], i);
								player_move(players, map, id);
							}
							break;
						case 3:
							if (map_is_inside(map, temp + 1, temp1)
									&& fire_is_at_xy(fires, player_get_x(
											players[id]) + 1, player_get_y(
											players[id])) == 0
									&& (map_get_cell_type(map, temp + 1, temp1)
											== CELL_EMPTY || map_get_cell_type(map,
											temp + 1, temp1) == CELL_BONUS)) {
								player_set_current_way(players[id], i);
								player_move(players, map, id);
							}
							break;
						}
						i++;
				}
			} else {
				int i = 3;

				while (player_get_x(players[id]) == temp && player_get_y(
						players[id]) == temp1 && i >= 0) {

					switch (i) {
							case 0:
								if (map_is_inside(map, temp, temp1 - 1)
										&& fire_is_at_xy(fires, player_get_x(
												players[id]), player_get_y(players[id])
												- 1) == 0 && (map_get_cell_type(map,
										temp, temp1 - 1) == CELL_EMPTY
										|| map_get_cell_type(map, temp, temp1 - 1)
												== CELL_BONUS)) {
									player_set_current_way(players[id], i);
									player_move(players, map, id);
								}
								break;
							case 1:
								if (map_is_inside(map, temp, temp1 + 1)
										&& fire_is_at_xy(fires, player_get_x(
												players[id]), player_get_y(players[id])
												+ 1) == 0 && (map_get_cell_type(map,
										temp, temp1 + 1) == CELL_EMPTY
										|| map_get_cell_type(map, temp, temp1 + 1)
												== CELL_BONUS)) {
									player_set_current_way(players[id], i);
									player_move(players, map, id);
								}
								break;
							case 2:
								if (map_is_inside(map, temp - 1, temp1)
										&& fire_is_at_xy(fires, player_get_x(
												players[id]) - 1, player_get_y(
												players[id])) == 0
										&& (map_get_cell_type(map, temp - 1, temp1)
												== CELL_EMPTY || map_get_cell_type(map,
												temp - 1, temp1) == CELL_BONUS)) {
									player_set_current_way(players[id], i);
									player_move(players, map, id);
								}
								break;
							case 3:
								if (map_is_inside(map, temp + 1, temp1)
										&& fire_is_at_xy(fires, player_get_x(
												players[id]) + 1, player_get_y(
												players[id])) == 0
										&& (map_get_cell_type(map, temp + 1, temp1)
												== CELL_EMPTY || map_get_cell_type(map,
												temp + 1, temp1) == CELL_BONUS)) {
									player_set_current_way(players[id], i);
									player_move(players, map, id);
								}
								break;

							}
							i--;
				}
			}

		} else { // the monster attacks the player two (distance1 > distance 2)
			if ((player_get_x(players[id2]) < player_get_x(players[id])) // directions
					&& fire_is_at_xy(fires, player_get_x(players[id]) - 1,
							player_get_y(players[id])) == 0) {
				player_set_current_way(players[id], 2);
				player_move(players, map, id);
			} else if ((player_get_y(players[id2]) < player_get_y(players[id]))
					&& fire_is_at_xy(fires, player_get_x(players[id]),
							player_get_y(players[id]) - 1) == 0) {

				player_set_current_way(players[id], 0);
				player_move(players, map, id);
				;
			} else if ((player_get_x(players[id2]) > player_get_x(players[id]))
					&& fire_is_at_xy(fires, player_get_x(players[id]) + 1,
							player_get_y(players[id])) == 0) {

				player_set_current_way(players[id], 3);
				player_move(players, map, id);
			} else if ((player_get_y(players[id2]) > player_get_y(players[id]))
					&& fire_is_at_xy(fires, player_get_x(players[id]),
							player_get_y(players[id]) + 1) == 0) {

				player_set_current_way(players[id], 1);
				player_move(players, map, id);
			}

			int z = rand() % 2;
			if (z == 0) {
				int i = 0;
				while (player_get_x(players[id]) == temp && player_get_y(
						players[id]) == temp1 && i < 4) {
					switch (i) {
					case 0:
						if (map_is_inside(map, temp, temp1 - 1)
								&& fire_is_at_xy(fires, player_get_x(
										players[id]), player_get_y(players[id])
										- 1) == 0 && (map_get_cell_type(map,
								temp, temp1 - 1) == CELL_EMPTY
								|| map_get_cell_type(map, temp, temp1 - 1)
										== CELL_BONUS)) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					case 1:
						if (map_is_inside(map, temp, temp1 + 1)
								&& fire_is_at_xy(fires, player_get_x(
										players[id]), player_get_y(players[id])
										+ 1) == 0 && (map_get_cell_type(map,
								temp, temp1 + 1) == CELL_EMPTY
								|| map_get_cell_type(map, temp, temp1 + 1)
										== CELL_BONUS)) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					case 2:
						if (map_is_inside(map, temp - 1, temp1)
								&& fire_is_at_xy(fires, player_get_x(
										players[id]) - 1, player_get_y(
										players[id])) == 0
								&& (map_get_cell_type(map, temp - 1, temp1)
										== CELL_EMPTY || map_get_cell_type(map,
										temp - 1, temp1) == CELL_BONUS)) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					case 3:
						if (map_is_inside(map, temp + 1, temp1)
								&& fire_is_at_xy(fires, player_get_x(
										players[id]) + 1, player_get_y(
										players[id])) == 0
								&& (map_get_cell_type(map, temp + 1, temp1)
										== CELL_EMPTY || map_get_cell_type(map,
										temp + 1, temp1) == CELL_BONUS)) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					}
					i++;
				}
			} else {
				int i = 3;

				while (player_get_x(players[id]) == temp && player_get_y(
						players[id]) == temp1 && i >= 0) {

					switch (i) {
					case 0:
						if (map_is_inside(map, temp, temp1 - 1)
								&& fire_is_at_xy(fires, player_get_x(
										players[id]), player_get_y(players[id])
										- 1) == 0 && (map_get_cell_type(map,
								temp, temp1 - 1) == CELL_EMPTY
								|| map_get_cell_type(map, temp, temp1 - 1)
										== CELL_BONUS)) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					case 1:
						if (map_is_inside(map, temp, temp1 + 1)
								&& fire_is_at_xy(fires, player_get_x(
										players[id]), player_get_y(players[id])
										+ 1) == 0 && (map_get_cell_type(map,
								temp, temp1 + 1) == CELL_EMPTY
								|| map_get_cell_type(map, temp, temp1 + 1)
										== CELL_BONUS)) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					case 2:
						if (map_is_inside(map, temp - 1, temp1)
								&& fire_is_at_xy(fires, player_get_x(
										players[id]) - 1, player_get_y(
										players[id])) == 0
								&& (map_get_cell_type(map, temp - 1, temp1)
										== CELL_EMPTY || map_get_cell_type(map,
										temp - 1, temp1) == CELL_BONUS)) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					case 3:
						if (map_is_inside(map, temp + 1, temp1)
								&& fire_is_at_xy(fires, player_get_x(
										players[id]) + 1, player_get_y(
										players[id])) == 0
								&& (map_get_cell_type(map, temp + 1, temp1)
										== CELL_EMPTY || map_get_cell_type(map,
										temp + 1, temp1) == CELL_BONUS)) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;

					}
					i--;
				}
			}

		}
	} else { // the player one is alone

		if ((player_get_x(players[id1]) < player_get_x(players[id])) // directions
				&& fire_is_at_xy(fires, player_get_x(players[id]) - 1,
						player_get_y(players[id])) == 0) {

			player_set_current_way(players[id], 2);
			player_move(players, map, id);
		} else if ((player_get_y(players[id1]) < player_get_y(players[id]))
				&& fire_is_at_xy(fires, player_get_x(players[id]),
						player_get_y(players[id]) - 1) == 0) {

			player_set_current_way(players[id], 0);
			player_move(players, map, id);
		} else if ((player_get_x(players[id1]) > player_get_x(players[id]))
				&& fire_is_at_xy(fires, player_get_x(players[id]) + 1,
						player_get_y(players[id])) == 0) {

			player_set_current_way(players[id], 3);
			player_move(players, map, id);
		} else if ((player_get_y(players[id1]) > player_get_y(players[id]))
				&& fire_is_at_xy(fires, player_get_x(players[id]),
						player_get_y(players[id]) + 1) == 0) {

			player_set_current_way(players[id], 1);
			player_move(players, map, id);
		}

		int z = rand() % 2;
		if (z == 0) {
			int i = 0;
			while (player_get_x(players[id]) == temp && player_get_y(
					players[id]) == temp1 && i < 4) {
				switch (i) {
				case 0:
					if (map_is_inside(map, temp, temp1 - 1) && fire_is_at_xy(
							fires, player_get_x(players[id]), player_get_y(
									players[id]) - 1) == 0
							&& (map_get_cell_type(map, temp, temp1 - 1)
									== CELL_EMPTY || map_get_cell_type(map,
									temp, temp1 - 1) == CELL_BONUS)) {
						player_set_current_way(players[id], i);
						player_move(players, map, id);
					}
					break;
				case 1:
					if (map_is_inside(map, temp, temp1 + 1) && fire_is_at_xy(
							fires, player_get_x(players[id]), player_get_y(
									players[id]) + 1) == 0
							&& (map_get_cell_type(map, temp, temp1 + 1)
									== CELL_EMPTY || map_get_cell_type(map,
									temp, temp1 + 1) == CELL_BONUS)) {
						player_set_current_way(players[id], i);
						player_move(players, map, id);
					}
					break;
				case 2:
					if (map_is_inside(map, temp - 1, temp1) && fire_is_at_xy(
							fires, player_get_x(players[id]) - 1, player_get_y(
									players[id])) == 0 && (map_get_cell_type(
							map, temp - 1, temp1) == CELL_EMPTY
							|| map_get_cell_type(map, temp - 1, temp1)
									== CELL_BONUS)) {
						player_set_current_way(players[id], i);
						player_move(players, map, id);
					}
					break;
				case 3:
					if (map_is_inside(map, temp + 1, temp1) && fire_is_at_xy(
							fires, player_get_x(players[id]) + 1, player_get_y(
									players[id])) == 0 && (map_get_cell_type(
							map, temp + 1, temp1) == CELL_EMPTY
							|| map_get_cell_type(map, temp + 1, temp1)
									== CELL_BONUS)) {
						player_set_current_way(players[id], i);
						player_move(players, map, id);
					}
					break;
				}
				i++;
			}
		} else {
			int i = 3;

			while (player_get_x(players[id]) == temp && player_get_y(
					players[id]) == temp1 && i >= 0) {
				switch (i) {
				case 0:
					if (map_is_inside(map, temp, temp1 - 1) && fire_is_at_xy(
							fires, player_get_x(players[id]), player_get_y(
									players[id]) - 1) == 0
							&& (map_get_cell_type(map, temp, temp1 - 1)
									== CELL_EMPTY || map_get_cell_type(map,
									temp, temp1 - 1) == CELL_BONUS)) {
						player_set_current_way(players[id], i);
						player_move(players, map, id);
					}
					break;
				case 1:
					if (map_is_inside(map, temp, temp1 + 1) && fire_is_at_xy(
							fires, player_get_x(players[id]), player_get_y(
									players[id]) + 1) == 0
							&& (map_get_cell_type(map, temp, temp1 + 1)
									== CELL_EMPTY || map_get_cell_type(map,
									temp, temp1 + 1) == CELL_BONUS)) {
						player_set_current_way(players[id], i);
						player_move(players, map, id);
					}
					break;
				case 2:
					if (map_is_inside(map, temp - 1, temp1) && fire_is_at_xy(
							fires, player_get_x(players[id]) - 1, player_get_y(
									players[id])) == 0 && (map_get_cell_type(
							map, temp - 1, temp1) == CELL_EMPTY
							|| map_get_cell_type(map, temp - 1, temp1)
									== CELL_BONUS)) {
						player_set_current_way(players[id], i);
						player_move(players, map, id);
					}
					break;
				case 3:
					if (map_is_inside(map, temp + 1, temp1) && fire_is_at_xy(
							fires, player_get_x(players[id]) + 1, player_get_y(
									players[id])) == 0 && (map_get_cell_type(
							map, temp + 1, temp1) == CELL_EMPTY
							|| map_get_cell_type(map, temp + 1, temp1)
									== CELL_BONUS)) {
						player_set_current_way(players[id], i);
						player_move(players, map, id);
					}
					break;

				}
				i--;
			}
		}
	}
}

int IArecursive(t_map map, int id, int i, int j, int dir) {
	assert(map);
	if (i + 1 < map_get_width(map) && i + 1 > 0 && j + 1

	< map_get_height(map) && j + 1 > 0) {
		if (map_get_cell_type(map, i, j) == CELL_PLAYER) {
			return 1;
		}
		if (map_get_cell_type(map, i, j) == CELL_EMPTY || map_get_cell_type(
				map, i, j) == CELL_BONUS || map_get_cell_type(map, i, j)
				== CELL_MONSTER) {
			if (dir == 3) {
				if (i + 1 < map_get_width(map) && i + 1 > 0)
					return IArecursive(map, id, i + 1, j, 3);
				else
					return 0;
			}
			if (dir == 0) {
				if (j - 1 < map_get_height(map) && j - 1 > 0)
					return IArecursive(map, id, i, j - 1, 0);
				else
					return 0;
			}
			if (dir == 1) {
				if (j + 1 < map_get_height(map) && j + 1 > 0)
					return IArecursive(map, id, i, j + 1, 1);
				else
					return 0;
			}
			if (dir == 2) {
				if (i - 1 < map_get_width(map) && i - 1 > 0)
					return IArecursive(map, id, i - 1, j, 2);
				else
					return 0;
			}
		} else if (map_get_cell_type(map, i, j) == CELL_BOMB) {
			return 2;

		} else {
			return 0;
		}
	}
	return 0;
}
void IA_2second(t_map map, t_player* players, t_bomb* bombes, t_fire* fires,
		t_ia ia, int id, int id1, int id2) {
	int temp = player_get_x(players[id]);
	int temp1 = player_get_y(players[id]);

	if (id2 != -1) { // player 2 is present or not present ?
		int distance1 = ((player_get_x(players[id1]) - temp) // calculating the distance between the monster and the player one and two
				* (player_get_x(players[id1]) - temp) + (player_get_y(
				players[id1]) - temp1) * (player_get_y(players[id1]) - temp1));
		int distance2 = ((player_get_x(players[id2]) - temp) * (player_get_x(
				players[id2]) - temp) + (player_get_y(players[id2]) - temp1)
				* (player_get_y(players[id2]) - temp1));

		if (distance1 < distance2) { // the monster attacks the player one
			if ((player_get_x(players[id1]) < player_get_x(players[id])) // directions
					&& fire_is_at_xy(fires, player_get_x(players[id]) - 1,
							player_get_y(players[id])) == 0
					&& bomb_time_to_explode_at_xy(map, bombes, temp - 1, temp1)
							== -1) {

				player_set_current_way(players[id], 2);
				player_move(players, map, id);
			} else if ((player_get_y(players[id1]) < player_get_y(players[id]))
					&& fire_is_at_xy(fires, player_get_x(players[id]),
							player_get_y(players[id]) - 1) == 0
					&& bomb_time_to_explode_at_xy(map, bombes, temp, temp1 - 1)
							== -1) {

				player_set_current_way(players[id], 0);
				player_move(players, map, id);
			} else if ((player_get_x(players[id1]) > player_get_x(players[id]))
					&& fire_is_at_xy(fires, player_get_x(players[id]) + 1,
							player_get_y(players[id])) == 0
					&& bomb_time_to_explode_at_xy(map, bombes, temp + 1, temp1)
							== -1) {

				player_set_current_way(players[id], 3);
				player_move(players, map, id);
			} else if ((player_get_y(players[id1]) > player_get_y(players[id]))
					&& fire_is_at_xy(fires, player_get_x(players[id]),
							player_get_y(players[id]) + 1) == 0
					&& bomb_time_to_explode_at_xy(map, bombes, temp, temp1 + 1)
							== -1) {

				player_set_current_way(players[id], 1);
				player_move(players, map, id);
			}

			int z = rand() % 2;
			if (z == 0) {
				int i = 0;
				while (player_get_x(players[id]) == temp && player_get_y(
						players[id]) == temp1 && i < 4) {
					switch (i) {
					case 0:
						if (fire_is_at_xy(fires, player_get_x(players[id]),
								player_get_y(players[id]) - 1) == 0
								&& bomb_time_to_explode_at_xy(map, bombes,
										temp, temp1 - 1) == -1) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					case 1:
						if (fire_is_at_xy(fires, player_get_x(players[id]),
								player_get_y(players[id]) + 1) == 0
								&& bomb_time_to_explode_at_xy(map, bombes,
										temp, temp1 + 1) == -1) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					case 2:
						if (fire_is_at_xy(fires, player_get_x(players[id]) - 1,
								player_get_y(players[id])) == 0
								&& bomb_time_to_explode_at_xy(map, bombes, temp
										- 1, temp1) == -1) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					case 3:
						if (fire_is_at_xy(fires, player_get_x(players[id]) + 1,
								player_get_y(players[id])) == 0
								&& bomb_time_to_explode_at_xy(map, bombes, temp
										+ 1, temp1) == -1) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					}
					i++;
				}
			} else {
				int i = 3;

				while (player_get_x(players[id]) == temp && player_get_y(
						players[id]) == temp1 && i >= 0) {

					switch (i) {
					case 0:
						if (fire_is_at_xy(fires, player_get_x(players[id]),
								player_get_y(players[id]) - 1) == 0
								&& bomb_time_to_explode_at_xy(map, bombes,
										temp, temp1 - 1) == -1) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					case 1:
						if (fire_is_at_xy(fires, player_get_x(players[id]),
								player_get_y(players[id]) + 1) == 0
								&& bomb_time_to_explode_at_xy(map, bombes,
										temp, temp1 + 1) == -1) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					case 2:
						if (fire_is_at_xy(fires, player_get_x(players[id]) - 1,
								player_get_y(players[id])) == 0
								&& bomb_time_to_explode_at_xy(map, bombes, temp
										- 1, temp1) == -1) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					case 3:
						if (fire_is_at_xy(fires, player_get_x(players[id]) + 1,
								player_get_y(players[id])) == 0
								&& bomb_time_to_explode_at_xy(map, bombes, temp
										+ 1, temp1) == -1) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					}
					i--;
				}
			}

		} else { // the monster attacks the player two (distance1 > distance 2)
			if ((player_get_x(players[id2]) < player_get_x(players[id])) // directions
					&& fire_is_at_xy(fires, player_get_x(players[id]) - 1,
							player_get_y(players[id])) == 0
					&& bomb_time_to_explode_at_xy(map, bombes, temp - 1, temp1)
							== -1) {
				player_set_current_way(players[id], 2);
				player_move(players, map, id);
			} else if ((player_get_y(players[id2]) < player_get_y(players[id]))
					&& fire_is_at_xy(fires, player_get_x(players[id]),
							player_get_y(players[id]) - 1) == 0
					&& bomb_time_to_explode_at_xy(map, bombes, temp, temp1 - 1)
							== -1) {

				player_set_current_way(players[id], 0);
				player_move(players, map, id);
				;
			} else if ((player_get_x(players[id2]) > player_get_x(players[id]))
					&& fire_is_at_xy(fires, player_get_x(players[id]) + 1,
							player_get_y(players[id])) == 0
					&& bomb_time_to_explode_at_xy(map, bombes, temp + 1, temp1)
							== -1) {

				player_set_current_way(players[id], 3);
				player_move(players, map, id);
			} else if ((player_get_y(players[id2]) > player_get_y(players[id]))
					&& fire_is_at_xy(fires, player_get_x(players[id]),
							player_get_y(players[id]) + 1) == 0
					&& bomb_time_to_explode_at_xy(map, bombes, temp, temp1 + 1)
							== -1) {

				player_set_current_way(players[id], 1);
				player_move(players, map, id);
			}

			int z = rand() % 2;
			if (z == 0) {
				int i = 0;
				while (player_get_x(players[id]) == temp && player_get_y(
						players[id]) == temp1 && i < 4) {
					switch (i) {
					case 0:
						if (fire_is_at_xy(fires, player_get_x(players[id]),
								player_get_y(players[id]) - 1) == 0
								&& bomb_time_to_explode_at_xy(map, bombes,
										temp, temp1 - 1) == -1) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					case 1:
						if (fire_is_at_xy(fires, player_get_x(players[id]),
								player_get_y(players[id]) + 1) == 0
								&& bomb_time_to_explode_at_xy(map, bombes,
										temp, temp1 + 1) == -1) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					case 2:
						if (fire_is_at_xy(fires, player_get_x(players[id]) - 1,
								player_get_y(players[id])) == 0
								&& bomb_time_to_explode_at_xy(map, bombes, temp
										- 1, temp1) == -1) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					case 3:
						if (fire_is_at_xy(fires, player_get_x(players[id]) + 1,
								player_get_y(players[id])) == 0
								&& bomb_time_to_explode_at_xy(map, bombes, temp
										+ 1, temp1) == -1) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					}
					i++;
				}
			} else {
				int i = 3;

				while (player_get_x(players[id]) == temp && player_get_y(
						players[id]) == temp1 && i >= 0) {

					switch (i) {
					case 0:
						if (fire_is_at_xy(fires, player_get_x(players[id]),
								player_get_y(players[id]) - 1) == 0
								&& bomb_time_to_explode_at_xy(map, bombes,
										temp, temp1 - 1) == -1) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					case 1:
						if (fire_is_at_xy(fires, player_get_x(players[id]),
								player_get_y(players[id]) + 1) == 0
								&& bomb_time_to_explode_at_xy(map, bombes,
										temp, temp1 + 1) == -1) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					case 2:
						if (fire_is_at_xy(fires, player_get_x(players[id]) - 1,
								player_get_y(players[id])) == 0
								&& bomb_time_to_explode_at_xy(map, bombes, temp
										- 1, temp1) == -1) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					case 3:
						if (fire_is_at_xy(fires, player_get_x(players[id]) + 1,
								player_get_y(players[id])) == 0
								&& bomb_time_to_explode_at_xy(map, bombes, temp
										+ 1, temp1) == -1) {
							player_set_current_way(players[id], i);
							player_move(players, map, id);
						}
						break;
					}
					i--;
				}
			}

		}
	} else { // the player one is alone

		if ((player_get_x(players[id1]) < player_get_x(players[id])) // directions
				&& fire_is_at_xy(fires, player_get_x(players[id]) - 1,
						player_get_y(players[id])) == 0
				&& bomb_time_to_explode_at_xy(map, bombes, temp - 1, temp1)
						== -1) {

			player_set_current_way(players[id], 2);
			player_move(players, map, id);
		} else if ((player_get_y(players[id1]) < player_get_y(players[id]))
				&& fire_is_at_xy(fires, player_get_x(players[id]),
						player_get_y(players[id]) - 1) == 0
				&& bomb_time_to_explode_at_xy(map, bombes, temp, temp1 - 1)
						== -1) {

			player_set_current_way(players[id], 0);
			player_move(players, map, id);
		} else if ((player_get_x(players[id1]) > player_get_x(players[id]))
				&& fire_is_at_xy(fires, player_get_x(players[id]) + 1,
						player_get_y(players[id])) == 0
				&& bomb_time_to_explode_at_xy(map, bombes, temp + 1, temp1)
						== -1) {

			player_set_current_way(players[id], 3);
			player_move(players, map, id);
		} else if ((player_get_y(players[id1]) > player_get_y(players[id]))
				&& fire_is_at_xy(fires, player_get_x(players[id]),
						player_get_y(players[id]) + 1) == 0
				&& bomb_time_to_explode_at_xy(map, bombes, temp, temp1 + 1)
						== -1) {

			player_set_current_way(players[id], 1);
			player_move(players, map, id);
		}

		int z = rand() % 2;
		if (z == 0) {
			int i = 0;
			while (player_get_x(players[id]) == temp && player_get_y(
					players[id]) == temp1 && i < 4) {
				switch (i) {
				case 0:
					if (fire_is_at_xy(fires, player_get_x(players[id]),
							player_get_y(players[id]) - 1) == 0
							&& bomb_time_to_explode_at_xy(map, bombes, temp,
									temp1 - 1) == -1) {
						player_set_current_way(players[id], i);
						player_move(players, map, id);
					}
					break;
				case 1:
					if (fire_is_at_xy(fires, player_get_x(players[id]),
							player_get_y(players[id]) + 1) == 0
							&& bomb_time_to_explode_at_xy(map, bombes, temp,
									temp1 + 1) == -1) {
						player_set_current_way(players[id], i);
						player_move(players, map, id);
					}
					break;
				case 2:
					if (fire_is_at_xy(fires, player_get_x(players[id]) - 1,
							player_get_y(players[id])) == 0
							&& bomb_time_to_explode_at_xy(map, bombes,
									temp - 1, temp1) == -1) {
						player_set_current_way(players[id], i);
						player_move(players, map, id);
					}
					break;
				case 3:
					if (fire_is_at_xy(fires, player_get_x(players[id]) + 1,
							player_get_y(players[id])) == 0
							&& bomb_time_to_explode_at_xy(map, bombes,
									temp + 1, temp1) == -1) {
						player_set_current_way(players[id], i);
						player_move(players, map, id);
					}
					break;
				}
				i++;
			}
		} else {
			int i = 3;

			while (player_get_x(players[id]) == temp && player_get_y(
					players[id]) == temp1 && i >= 0) {
				switch (i) {
				case 0:
					if (fire_is_at_xy(fires, player_get_x(players[id]),
							player_get_y(players[id]) - 1) == 0
							&& bomb_time_to_explode_at_xy(map, bombes, temp,
									temp1 - 1) == -1) {
						player_set_current_way(players[id], i);
						player_move(players, map, id);
					}
					break;
				case 1:
					if (fire_is_at_xy(fires, player_get_x(players[id]),
							player_get_y(players[id]) + 1) == 0
							&& bomb_time_to_explode_at_xy(map, bombes, temp,
									temp1 + 1) == -1) {
						player_set_current_way(players[id], i);
						player_move(players, map, id);
					}
					break;
				case 2:
					if (fire_is_at_xy(fires, player_get_x(players[id]) - 1,
							player_get_y(players[id])) == 0
							&& bomb_time_to_explode_at_xy(map, bombes,
									temp - 1, temp1) == -1) {
						player_set_current_way(players[id], i);
						player_move(players, map, id);
					}
					break;
				case 3:
					if (fire_is_at_xy(fires, player_get_x(players[id]) + 1,
							player_get_y(players[id])) == 0
							&& bomb_time_to_explode_at_xy(map, bombes,
									temp + 1, temp1) == -1) {
						player_set_current_way(players[id], i);
						player_move(players, map, id);
					}
					break;
				}
				i--;
			}
		}
	}
}

void IA_3(t_map map, t_player* players, t_bomb* bombes, t_fire* fires, t_ia ia,
		int id, int id1, int id2) {
	int flag = 0;
	int i = player_get_x(players[id]);
	int j = player_get_y(players[id]);
	int direction = -1; // -1 if not player 1 if player
	int temp = IArecursive(map, id, i + 1, j, 3); // return 2 if there is a bomb return 1 if player else return 0
	int z = rand() % 2;
	if (z == 0 && temp == 2 && map_is_inside(map,i,j) && map_is_inside(map,i,j-1)
			&& bomb_time_to_explode_at_xy(map, bombes, i, j) != -1
			&& fire_is_at_xy(fires, player_get_x(players[id]) - 1,
					player_get_y(players[id])) == 0
			&& bomb_time_to_explode_at_xy(map, bombes, i, j - 1) == -1) {
		player_set_current_way(players[id], 0);
		player_move(players, map, id);
		flag = 1;
	} else if (z == 1 && temp == 2 && map_is_inside(map,i,j) && map_is_inside(map,i,j+1)
			&& bomb_time_to_explode_at_xy(map, bombes, i, j) != -1
			&& fire_is_at_xy(fires, player_get_x(players[id]) + 1,
					player_get_y(players[id])) == 0
			&& bomb_time_to_explode_at_xy(map, bombes, i, j + 1) == -1) {
		player_set_current_way(players[id], 1);
		player_move(players, map, id);
		flag = 1;
	}

	if (temp == 1) // looking if the player is before the bomb
		direction = 1;
	temp = IArecursive(map, id, i, j - 1, 0);
	z = rand() % 2;
	if (z == 0 && temp == 2 && map_is_inside(map,i,j) && map_is_inside(map,i-1,j)
			&& bomb_time_to_explode_at_xy(map, bombes, i, j) != -1
			&& fire_is_at_xy(fires, player_get_x(players[id]), player_get_y(
					players[id]) - 1) == 0 && bomb_time_to_explode_at_xy(map,
			bombes, i - 1, j) == -1) {
		player_set_current_way(players[id], 2);
		player_move(players, map, id);
		flag = 1;
	} else if (z == 1 && temp == 2 && map_is_inside(map,i,j) && map_is_inside(map,i+1,j)
			&& bomb_time_to_explode_at_xy(map, bombes, i, j) != -1
			&& fire_is_at_xy(fires, player_get_x(players[id]), player_get_y(
					players[id]) + 1) == 0 && bomb_time_to_explode_at_xy(map,
			bombes, i + 1, j) == -1) {
		player_set_current_way(players[id], 3);
		player_move(players, map, id);
		flag = 1;
	}

	if (temp == 1)// looking if the payer is before the bomb
		direction = 1;
	temp = IArecursive(map, id, i, j + 1, 1);
	z = rand() % 2;

	if (z == 0 && temp == 2 && map_is_inside(map,i+1,j) && map_is_inside(map,i,j)
			&& bomb_time_to_explode_at_xy(map, bombes, i, j) != -1
			&& fire_is_at_xy(fires, player_get_x(players[id]), player_get_y(
					players[id]) + 1) == 0 && bomb_time_to_explode_at_xy(map,
			bombes, i + 1, j) == -1) {
		player_set_current_way(players[id], 3);
		player_move(players, map, id);
		flag = 1;
	} else if (z == 1 && temp == 2 && map_is_inside(map,i,j) && map_is_inside(map,i-1,j)
			&& bomb_time_to_explode_at_xy(map, bombes, i, j) != -1
			&& fire_is_at_xy(fires, player_get_x(players[id]), player_get_y(
					players[id]) - 1) == 0 && bomb_time_to_explode_at_xy(map,
			bombes, i - 1, j) == -1) {
		player_set_current_way(players[id], 2);
		player_move(players, map, id);
		flag = 1;
	}

	if (temp == 1)// looking if the payer is before the bomb
		direction = 1;
	temp = IArecursive(map, id, i - 1, j, 2);
	z = rand() % 2;
	if (z == 0 && temp == 2 && map_is_inside(map,i,j) && map_is_inside(map,i,j+1)
			&& bomb_time_to_explode_at_xy(map, bombes, i, j) != -1
			&& fire_is_at_xy(fires, player_get_x(players[id]) + 1,
					player_get_y(players[id])) == 0
			&& bomb_time_to_explode_at_xy(map, bombes, i, j + 1) == -1) {
		player_set_current_way(players[id], 1);
		player_move(players, map, id);
		flag = 1;
	} else if (z == 1 && temp == 2 && map_is_inside(map,i,j) && map_is_inside(map,i,j-1)
			&& bomb_time_to_explode_at_xy(map, bombes, i, j) != -1
			&& fire_is_at_xy(fires, player_get_x(players[id]) - 1,
					player_get_y(players[id])) == 0
			&& bomb_time_to_explode_at_xy(map, bombes, i, j - 1) == -1) {
		player_set_current_way(players[id], 0);
		player_move(players, map, id);
		flag = 1;
	}

	if (temp == 1)// looking if the payer is before the bomb
		direction = 1;

	if (flag == 0 && i == player_get_x(players[id]) && j == player_get_y(
			players[id]) && ((i - 1 >= 0 && i - 1 < map_get_width(map)
			&& (map_get_cell_type(map, i - 1, j) == CELL_EMPTY
					|| map_get_cell_type(map, i - 1, j) == CELL_BONUS)) || (j
			- 1 >= 0 && j - 1 < map_get_height(map) && (map_get_cell_type(map,
			i, j - 1) == CELL_EMPTY || map_get_cell_type(map, i, j - 1)
			== CELL_BONUS)) || (i + 1 >= 0 && i + 1 < map_get_width(map)
			&& map_get_cell_type(map, i + 1, j) == CELL_EMPTY) || (j + 1 >= 0
			&& j + 1 < map_get_height(map) && (map_get_cell_type(map, i, j + 1)
			== CELL_EMPTY || map_get_cell_type(map, i, j + 1) == CELL_BONUS)))) {

		IA_2second(map, players, bombes, fires, ia, id, id1, id2); // use the previous IA (mod attack)


	} else if (flag == 1 && i == player_get_x(players[id]) && j // (mod defend)
			== player_get_y(players[id])) {
		int z = rand() % 2;
		if (z == 0) {
			int u = 0;
			while (player_get_x(players[id]) == i && player_get_y(players[id])
					== j && u < 4) {
				switch (u) {
				case 0:
					if (bomb_time_to_explode_at_xy(map, bombes, i, j - 1) != -1
							&& fire_is_at_xy(fires, player_get_x(players[id]),
									player_get_y(players[id]) - 1) == 0) {
						player_set_current_way(players[id], u);
						player_move(players, map, id);
					}
					break;
				case 1:
					if (bomb_time_to_explode_at_xy(map, bombes, i, j + 1) != -1
							&& fire_is_at_xy(fires, player_get_x(players[id]),
									player_get_y(players[id]) + 1) == 0) {
						player_set_current_way(players[id], u);
						player_move(players, map, id);
					}
					break;
				case 2:
					if (bomb_time_to_explode_at_xy(map, bombes, i - 1, j) != -1
							&& fire_is_at_xy(fires, player_get_x(players[id])
									- 1, player_get_y(players[id])) == 0) {
						player_set_current_way(players[id], u);
						player_move(players, map, id);
					}
					break;
				case 3:
					if (bomb_time_to_explode_at_xy(map, bombes, i + 1, j) != -1
							&& fire_is_at_xy(fires, player_get_x(players[id])
									+ 1, player_get_y(players[id])) == 0) {
						player_set_current_way(players[id], u);
						player_move(players, map, id);
					}
					break;
				}
				u++;

			}
		} else if (flag == 1 && i == player_get_x(players[id]) && j // (mod defend)
				== player_get_y(players[id])) {
			int u = 3;
			while (player_get_x(players[id]) == i && player_get_y(players[id])
					== j && u >= 0) {
				switch (u) {
				case 0:
					if (bomb_time_to_explode_at_xy(map, bombes, i, j - 1) != -1
							&& fire_is_at_xy(fires, player_get_x(players[id]),
									player_get_y(players[id]) - 1) == 0) {
						player_set_current_way(players[id], u);
						player_move(players, map, id);
					}
					break;
				case 1:
					if (bomb_time_to_explode_at_xy(map, bombes, i, j + 1) != -1
							&& fire_is_at_xy(fires, player_get_x(players[id]),
									player_get_y(players[id]) + 1) == 0) {
						player_set_current_way(players[id], u);
						player_move(players, map, id);
					}
					break;
				case 2:
					if (bomb_time_to_explode_at_xy(map, bombes, i - 1, j) != -1
							&& fire_is_at_xy(fires, player_get_x(players[id])
									- 1, player_get_y(players[id])) == 0) {
						player_set_current_way(players[id], u);
						player_move(players, map, id);
					}
					break;
				case 3:
					if (bomb_time_to_explode_at_xy(map, bombes, i + 1, j) != -1
							&& fire_is_at_xy(fires, player_get_x(players[id])
									+ 1, player_get_y(players[id])) == 0) {
						player_set_current_way(players[id], u);
						player_move(players, map, id);
					}
					break;
				}
				u--;
			}
		}

		if (i == player_get_x(players[id]) && j == player_get_y(players[id])) // player blocked
		{

			if (j + 1 >= 0 && j + 1 < map_get_height(map)
					&& (map_get_cell_type(map, i, j + 1) == CELL_EMPTY
							|| map_get_cell_type(map, i, j + 1) == CELL_BONUS)
					&& bomb_time_to_explode_at_xy(map, bombes, i, j + 1) == -1
					&& fire_is_at_xy(fires, player_get_x(players[id]),
							player_get_y(players[id]) + 1) == 0) {

				IA_2second(map, players, bombes, fires, ia, id, id1, id2);
			} else if (j - 1 >= 0 && j - 1 < map_get_height(map)
					&& (map_get_cell_type(map, i, j - 1) == CELL_EMPTY
							|| map_get_cell_type(map, i, j - 1) == CELL_BONUS)
					&& bomb_time_to_explode_at_xy(map, bombes, i, j - 1) == -1
					&& fire_is_at_xy(fires, player_get_x(players[id]),
							player_get_y(players[id]) - 1) == 0) {

				IA_2second(map, players, bombes, fires, ia, id, id1, id2);
			} else if (i + 1 >= 0 && i + 1 < map_get_width(map)
					&& (map_get_cell_type(map, i + 1, j) == CELL_EMPTY
							|| map_get_cell_type(map, i + 1, j) == CELL_BONUS)
					&& bomb_time_to_explode_at_xy(map, bombes, i + 1, j) == -1
					&& fire_is_at_xy(fires, player_get_x(players[id]) + 1,
							player_get_y(players[id])) == 0) {
				IA_2second(map, players, bombes, fires, ia, id, id1, id2);

			} else if (i - 1 >= 0 && i - 1 < map_get_width(map)
					&& (map_get_cell_type(map, i - 1, j) == CELL_EMPTY
							|| map_get_cell_type(map, i - 1, j) == CELL_BONUS)
					&& bomb_time_to_explode_at_xy(map, bombes, i - 1, j) == -1
					&& fire_is_at_xy(fires, player_get_x(players[id]) - 1,
							player_get_y(players[id])) == 0) {
				IA_2second(map, players, bombes, fires, ia, id, id1, id2);

			}
		}
	}
}

void IA_queen(t_map map, t_player* players, t_bomb* bombes, t_fire* fires,
		t_ia ia, int id) {
	assert(map);
	assert(players);
	assert(players[id]);
	assert(bombes);
	assert(fires);
	assert(ia);
	int egg_type = -1, ok = 0;
	//PONTE
	t_player player = player_get_player(players, id);
	int x = player_get_x(player);
	int y = player_get_y(player);
	if (rand() % 100 + 1 <= PROBA_PLACE_EGG) {
		int continuer = 0;
		if (map_is_inside(map, x, y - 1) && map_get_cell_type(map, x, y - 1)
				== CELL_EMPTY) {
			continuer = 1;
			player_set_current_way(player, NORTH);
		} else if (map_is_inside(map, x, y + 1) && map_get_cell_type(map, x, y
				+ 1) == CELL_EMPTY) {
			continuer = 1;
			player_set_current_way(player, SOUTH);
		} else if (map_is_inside(map, x + 1, y) && map_get_cell_type(map,
				x + 1, y) == CELL_EMPTY) {
			continuer = 1;
			player_set_current_way(player, EAST);
		} else if (map_is_inside(map, x - 1, y) && map_get_cell_type(map,
				x - 1, y) == CELL_EMPTY) {
			continuer = 1;
			player_set_current_way(player, WEST);
		}
		if (continuer) {
			int random = rand() % 3;
			if (player_get_ressources(player) <= 2 * PRICE_MONSTER_BUILDER) {
				random = 2;
			}
			if (random == 0 && player_count(players, MONSTER_CLASSIC)
					<= MAX_CLASSIC && player_get_ressources(player)
					>= PRICE_MONSTER_CLASSIC) {
				egg_type = EGG_CLASSIC;
				player_dec_ressources(player, PRICE_MONSTER_CLASSIC);
			} else if (random == 1 && player_count(players, MONSTER_BUILDER)
					<= MAX_BUILDER && player_get_ressources(player)
					>= PRICE_MONSTER_BUILDER) {
				egg_type = EGG_BUILDER;
				player_dec_ressources(player, PRICE_MONSTER_BUILDER);
			} else if (random == 2 && player_count(players, MONSTER_RES)
					<= MAX_RES && player_get_ressources(player)
					>= PRICE_MONSTER_RES) {
				egg_type = EGG_RES;

				player_dec_ressources(player, PRICE_MONSTER_RES);
			}
			if (egg_type != -1) {
				if (player_has_launch(player)) { //launch bomb
					switch (player_get_way(player)) {
					case NORTH:
						if (map_is_inside(map, x, y - LAUNCH_DISTANCE)) {
							if (map_get_cell_type(map, x, y - LAUNCH_DISTANCE)
									== CELL_EMPTY) {
								bomb_set_bomb(bombes, x, y - LAUNCH_DISTANCE,
										player_get_range(player), egg_type, id,
										BOMB_LIFETIME);
								player_dec_nb_bomb(player);
								map_set_cell_type(map, x, y - LAUNCH_DISTANCE,
										CELL_BOMB);
								ok = 1;
							}
						}
						break;
					case SOUTH:
						if (map_is_inside(map, x, y + LAUNCH_DISTANCE)) {
							if (map_get_cell_type(map, x, y + LAUNCH_DISTANCE)
									== CELL_EMPTY) {
								bomb_set_bomb(bombes, x, y + LAUNCH_DISTANCE,
										player_get_range(player), egg_type, id,
										BOMB_LIFETIME);
								player_dec_nb_bomb(player);
								map_set_cell_type(map, x, y + LAUNCH_DISTANCE,
										CELL_BOMB);
								ok = 1;
							}
						}
						break;
					case EAST:
						if (map_is_inside(map, x + LAUNCH_DISTANCE, y)) {
							if (map_get_cell_type(map, x + LAUNCH_DISTANCE, y)
									== CELL_EMPTY) {
								bomb_set_bomb(bombes, x + LAUNCH_DISTANCE, y,
										player_get_range(player), egg_type, id,
										BOMB_LIFETIME);
								player_dec_nb_bomb(player);
								map_set_cell_type(map, x + LAUNCH_DISTANCE, y,
										CELL_BOMB);
								ok = 1;
							}
						}
						break;
					case WEST:
						if (map_is_inside(map, x - LAUNCH_DISTANCE, y)) {
							if (map_get_cell_type(map, x - LAUNCH_DISTANCE, y)
									== CELL_EMPTY) {
								bomb_set_bomb(bombes, x - LAUNCH_DISTANCE, y,
										player_get_range(player), egg_type, id,
										BOMB_LIFETIME);
								player_dec_nb_bomb(player);
								map_set_cell_type(map, x - LAUNCH_DISTANCE, y,
										CELL_BOMB);
								ok = 1;
							}
						}
						break;
					}
				}
			}
		}
	}
}
void IA_res(t_map map, t_player* players, t_bomb* bombes, t_fire* fires,
		t_ia ia, int id, int queenid) {
	assert(map);
	assert(players);
	assert(players[id]);
	assert(bombes);
	assert(fires);
	assert(ia);
	t_player player = player_get_player(players, id);
	t_player queen;
	if (queenid != -1)
		queen = player_get_player(players, queenid);
	int x = player_get_x(player);
	int y = player_get_y(player);
	if (queenid != -1) {
		if (map_is_inside(map, x, y + 1) && map_get_cell_type(map, x, y + 1)
				== CELL_TREE && map_get_cell_special(map, x, y + 1) < 7) {
			player_collect_ressources(players, map, x, y + 1, queenid);
		} else if (map_is_inside(map, x, y - 1) && map_get_cell_type(map, x, y
				- 1) == CELL_TREE && map_get_cell_special(map, x, y - 1) < 7) {
			player_collect_ressources(players, map, x, y - 1, queenid);
		} else if (map_is_inside(map, x + 1, y) && map_get_cell_type(map,
				x + 1, y) == CELL_TREE && map_get_cell_special(map, x + 1, y)
				< 7) {
			player_collect_ressources(players, map, x + 1, y, queenid);
		} else if (map_is_inside(map, x - 1, y) && map_get_cell_type(map,
				x - 1, y) == CELL_TREE && map_get_cell_special(map, x - 1, y)
				< 7) {
			player_collect_ressources(players, map, x - 1, y, queenid);
		} else {
			IA_1(map, players, bombes, fires, ia, id);
		}
	} else {
		IA_1(map, players, bombes, fires, ia, id);
	}
}
void IA_builder(t_map map, t_player* players, t_bomb* bombes, t_fire* fires,
		t_ia ia, int id, int queenid) {
	assert(map);
	assert(players);
	assert(players[id]);
	assert(bombes);
	assert(fires);
	assert(ia);
	t_player player = player_get_player(players, id);
	t_player queen;
	if (queenid != -1)
		queen = player_get_player(players, queenid);
	int x = player_get_x(player);
	int y = player_get_y(player);
	if (queenid != -1) {
		int ressources = player_get_ressources(queen);
		int random = rand() % 100 + 1;
		if (ressources > COST_RES_CASE && random <= PROBA_SPAWN_CASE) {
			if (map_is_inside(map, x, y + 1) && map_is_inside(map, x, y + 2)
					&& map_get_cell_type(map, x, y + 1) == CELL_EMPTY
					&& map_get_cell_type(map, x, y + 2) == CELL_CASE
					&& ((map_is_inside(map, x + 1, y + 1) && map_get_cell_type(
							map, x + 1, y + 1) == CELL_CASE) || (map_is_inside(
							map, x - 1, y + 1) && map_get_cell_type(map, x - 1,
							y + 1) == CELL_CASE))) {
				player_put_a_case(map, x, y + 1);
				player_dec_ressources(queen, COST_RES_CASE);
			} else if (map_is_inside(map, x, y - 1) && map_is_inside(map, x, y
					- 2) && map_get_cell_type(map, x, y - 1) == CELL_EMPTY
					&& map_get_cell_type(map, x, y - 2) == CELL_CASE
					&& ((map_is_inside(map, x + 1, y - 1) && map_get_cell_type(
							map, x + 1, y - 1) == CELL_CASE) || (map_is_inside(
							map, x - 1, y - 1) && map_get_cell_type(map, x - 1,
							y - 1) == CELL_CASE))) {
				player_put_a_case(map, x, y - 1);
				player_dec_ressources(queen, COST_RES_CASE);
			} else if (map_is_inside(map, x + 1, y) && map_is_inside(map,
					x + 2, y) && map_get_cell_type(map, x + 1, y) == CELL_EMPTY
					&& map_get_cell_type(map, x + 2, y) == CELL_CASE
					&& ((map_is_inside(map, x + 1, y - 1) && map_get_cell_type(
							map, x + 1, y - 1) == CELL_CASE) || (map_is_inside(
							map, x + 1, y + 1) && map_get_cell_type(map, x + 1,
							y + 1) == CELL_CASE))) {
				player_put_a_case(map, x + 1, y);
				player_dec_ressources(queen, COST_RES_CASE);
			} else if (map_is_inside(map, x - 1, y) && map_is_inside(map,
					x - 2, y) && map_get_cell_type(map, x - 1, y) == CELL_EMPTY
					&& map_get_cell_type(map, x - 2, y) == CELL_CASE
					&& ((map_is_inside(map, x - 1, y + 1) && map_get_cell_type(
							map, x - 1, y + 1) == CELL_CASE) || (map_is_inside(
							map, x - 1, y - 1) && map_get_cell_type(map, x - 1,
							y - 1) == CELL_CASE))) {
				player_put_a_case(map, x - 1, y);
				player_dec_ressources(queen, COST_RES_CASE);
			} else {
				IA_1(map, players, bombes, fires, ia, id);
			}
		} else {
			IA_1(map, players, bombes, fires, ia, id);
		}
	} else {
		IA_1(map, players, bombes, fires, ia, id);
	}
}
