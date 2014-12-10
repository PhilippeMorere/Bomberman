/*
 * game.c
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent R�veill�re
 */

#include <assert.h>
#include <time.h>
#include <math.h>

#include "../include/game.h"
#include "../include/game_time.h"

struct s_blood {
	int x;
	int y;
	int type;
};
struct s_game {
	t_map map;
	t_player player[MAXPLAYER];
	t_bomb bomb[MAXBOMBES];
	t_fire fire[MAXFIRE];
	t_ia ia;

	int id1;
	int id2;
	int queen;
	int spawn[2][2];
	t_player* players;
	t_bomb* bombes;
	t_fire* fires;
	t_blood blood[MAXBLOOD];

	SDL_Surface * number[10];
	SDL_Surface * banner_life;
	SDL_Surface * banner_bomb;
	SDL_Surface * banner_range;
	SDL_Surface * banner_line;
	SDL_Surface * gagne;
	SDL_Surface * perdu;
	SDL_Surface * img_joueur1;
	SDL_Surface * img_joueur2;
	SDL_Surface *img_red_blood;
	SDL_Surface *img_green_blood;
};

static struct s_game the_game;

void save(t_game game, int countermode) {
	int width, height; // get the size of the map
	width = map_get_width(game_get_map(game));
	height = map_get_height(game_get_map(game));

	int number_digit1, number_digit2; // verify the size of variables
	if (width < 10)
		number_digit1 = 1;
	else
		number_digit1 = 2;
	if (height < 10)
		number_digit2 = 1;
	else
		number_digit2 = 2;

	int life1 = player_get_life(the_game.players[game->id1]);
	int bomb1 = player_get_nb_bomb_max(the_game.players[game_get_player1_id(
			game)]);
	int range1 = player_get_range(the_game.players[game->id1]);
	int life2 = 0;
	int bomb2 = 0;
	int range2 = 0;
	if (game->id2 != -1) {
		life2 = player_get_life(the_game.players[game->id2]);
		bomb2 = player_get_nb_bomb_max(the_game.players[game_get_player2_id(
				game)]);
		range2 = player_get_range(the_game.players[game->id2]);
	}

	char temp[(width - 1) + (width + 1) * (height - 1) + number_digit1
			+ number_digit2 + 19];

	sprintf(temp, "%s%i", "", width); // make the first ligne
	sprintf(temp, "%s%s", temp, ":");
	sprintf(temp, "%s%i", temp, height);
	sprintf(temp, "%s%s", temp, ":");
	sprintf(temp, "%s%i", temp, countermode);
	sprintf(temp, "%s%s", temp, ":");
	sprintf(temp, "%s%i", temp, life1);
	sprintf(temp, "%s%s", temp, ":");
	sprintf(temp, "%s%i", temp, bomb1);
	sprintf(temp, "%s%s", temp, ":");
	sprintf(temp, "%s%i", temp, range1);
	sprintf(temp, "%s%s", temp, ":");
	sprintf(temp, "%s%i", temp, life2);
	sprintf(temp, "%s%s", temp, ":");
	sprintf(temp, "%s%i", temp, bomb2);
	sprintf(temp, "%s%s", temp, ":");
	sprintf(temp, "%s%i", temp, range2);
	sprintf(temp, "%s%c", temp, '\n');
	FILE* file = NULL;

	for (int i = 0; i < height; i++) { // complete the variable temp according to the different elements
		for (int j = 0; j < width; j++) {

			switch (map_get_cell_type(game_get_map(game), j, i)) {
			case CELL_EMPTY:
				temp[j + i * (width + 1) + number_digit1 + number_digit2 + 16]
						= '0';
				break;
			case CELL_BONUS:
				switch (map_get_cell_special(game_get_map(game), j, i)) {
				case BONUS_BOMB_NB_DEC:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = 'x';
					break;
				case BONUS_BOMB_NB_INC:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = 'o';
					break;
				case BONUS_BOMB_RANGE_DEC:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = '-';
					break;
				case BONUS_BOMB_RANGE_INC:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = '+';
					break;
				case BONUS_MULTI_BOMB:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = 'M';
					break;
				case BONUS_LAUNCH_BOMB:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = 'l';
					break;
				case BONUS_DISEASE:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = 'd';
					break;
				case BONUS_LIFE:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = 'v';
					break;

				}
				break;
			case CELL_BOMB:
				temp[j + i * (width + 1) + number_digit1 + number_digit2 + 16]
						= '0';
				break;
			case CELL_FLAG:
				temp[j + i * (width + 1) + number_digit1 + number_digit2 + 16]
						= '1';

				break;
			case CELL_PLAYER:
				temp[j + i * (width + 1) + number_digit1 + number_digit2 + 16]
						= '2';

				break;
			case CELL_CASE:
				switch (map_get_cell_special(game_get_map(game), j, i)) {
				case BONUS_BOMB_NB_DEC:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = 'a';
					break;
				case BONUS_BOMB_NB_INC:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = 'c';
					break;
				case BONUS_BOMB_RANGE_DEC:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = 'D';
					break;
				case BONUS_BOMB_RANGE_INC:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = 'e';
					break;
				case BONUS_DISEASE:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = 'f';
					break;
				case BONUS_LAUNCH_BOMB:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = 'g';
					break;
				case BONUS_LIFE:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = 'h';
					break;
				case BONUS_MONSTER:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = 'i';
					break;
				case BONUS_MULTI_BOMB:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = 'j';
					break;
				case BONUS_NOTHING:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = 'k';
					break;
				default:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = '3';
					break;
				}
				break;
			case CELL_SPAWN:
				temp[j + i * (width + 1) + number_digit1 + number_digit2 + 16]
						= '0';
				break;
			case CELL_TREE:
				temp[j + i * (width + 1) + number_digit1 + number_digit2 + 16]
						= '4';

				break;
			case CELL_MONSTER:
				switch (player_get_advanced_type(player_get_player_by_xy(
						game_get_players(game), j, i))) {
				case MONSTER_CLASSIC:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = 'm';
					break;
				case MONSTER_QUEEN:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = 'q';
					break;
				case MONSTER_BUILDER:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = 'b';
					break;
				case MONSTER_RES:
					temp[j + i * (width + 1) + number_digit1 + number_digit2
							+ 16] = 'r';
					break;
				}
				break;
			}
		}
		temp[(i + 1) * (width) + i + number_digit1 + number_digit2 + 16] = '\n';

	}

	temp[(width - 1) + (width + 1) * (height - 1) + number_digit1
			+ number_digit2 + 18] = '\0';
	file = fopen(SRC_MAP_SAVE, "w"); // save the new file
	if (file != NULL) {
		fputs(temp, file);
		fclose(file);
	}

}

t_game game_new(int map, int compteurmode, int compte) {
	srand(time(NULL));
	game_time_init();
	the_game.players = the_game.player;
	the_game.bombes = the_game.bomb;
	the_game.fires = the_game.fire;
	int i, j, x, y;
	//the_game.map = map_load_static();
	if (map == 0) {
		the_game.map = map_load_from_file(SRC_MAP_SAVE);
		int *temp = get_parameter();
		compteurmode = temp[0];
	} else if (map == 1)
		the_game.map = map_load_from_file(SRC_MAP1);
	else if (map == 2)
		the_game.map = map_load_from_file(SRC_MAP2);
	else if (map == 3)
		the_game.map = map_load_from_file(SRC_MAP3);
	else if (map == 4)
		the_game.map = map_load_from_file(SRC_MAP4);
	else if (map == 5)
		the_game.map = map_load_from_file(SRC_MAP5);
	else if (map == 6)
		the_game.map = map_load_from_file(SRC_MAP6);
	else if (map == 7)
		the_game.map = map_load_from_file(SRC_MAP7);
	else if (map == 8)
		the_game.map = map_load_from_file(SRC_MAP8);
	else if (map == 9)
		the_game.map = map_load_from_file(SRC_MAP9);

	//while we find players on the map, add them
	the_game.id1 = -1;
	the_game.id2 = -1;
	the_game.spawn[0][0] = -1;
	the_game.spawn[0][1] = -1;
	the_game.spawn[1][0] = -1;
	the_game.spawn[1][1] = -1;
	the_game.queen = -1;
	the_game.ia = IA_new(map_get_width(the_game.map), map_get_height(
			the_game.map), compte);
	for (j = 0; j < MAXBOMBES; j++) {
		the_game.bomb[j] = NULL;
	}
	for (j = 0; j < MAXPLAYER; j++) {
		the_game.player[j] = NULL;
	}
	for (j = 0; j < MAXBLOOD; j++) {
		the_game.blood[j] = NULL;
	}
	for (j = 0; j < MAXFIRE; j++) {
		the_game.fire[j] = NULL;
	}
	i = 0;
	while (player_on_map(the_game.map) && i < MAXPLAYER) {
		the_game.player[i] = init_player_from_map(the_game.map, NB_START_BOMB);
		if (player_get_type(the_game.player[i]) == PLAYER_STD) {
			if (the_game.id1 == -1) {
				the_game.id1 = i;
				if (compteurmode == MOD_MULTI_QUEEN) {
					the_game.spawn[0][0] = player_get_x(the_game.player[i]);
					the_game.spawn[0][1] = player_get_y(the_game.player[i]);
				}
			} else {
				if (compteurmode == MOD_MULTI_CLASSIC || compteurmode
						== MOD_MULTI_QUEEN) {
					the_game.id2 = i;
					if (compteurmode == MOD_MULTI_QUEEN) {
						the_game.spawn[1][0] = player_get_x(the_game.player[i]);
						the_game.spawn[1][1] = player_get_y(the_game.player[i]);
					}
				} else {
					player_free(the_game.player[i]);
					the_game.player[i] = NULL;
				}
			}
		} else if (player_get_advanced_type(the_game.player[i])
				== MONSTER_QUEEN) {
			if (compteurmode == MOD_MULTI_QUEEN)
				the_game.queen = i;
			else {
				player_free(the_game.player[i]);
				the_game.player[i] = NULL;
			}
		} else if (player_get_advanced_type(the_game.player[i])
				== MONSTER_BUILDER && compteurmode != MOD_MULTI_QUEEN) {
			player_free(the_game.player[i]);
			the_game.player[i] = NULL;
		} else if (player_get_advanced_type(the_game.player[i]) == MONSTER_RES
				&& compteurmode != MOD_MULTI_QUEEN) {
			player_free(the_game.player[i]);
			the_game.player[i] = NULL;
		}
		i++;
	}
	if (map == 0) {
		int *temp = get_parameter();
		if (the_game.id1 != -1) {
			player_set_nb_bomb(the_game.players, the_game.id1, temp[2]);
			player_set_life(the_game.players[the_game.id1], temp[1]);
			player_set_range(the_game.players, the_game.id1, temp[3]);
		}
		if (the_game.id2 != -1) {
			player_set_nb_bomb(the_game.players, the_game.id2, temp[5]);
			player_set_life(the_game.players[the_game.id2], temp[4]);
			player_set_range(the_game.players, the_game.id2, temp[6]);
		}
	}
	//put players back on the map
	for (i = 0; i < MAXPLAYER; i++) {
		if (the_game.player[i] != NULL) {
			x = player_get_x(the_game.player[i]);
			y = player_get_y(the_game.player[i]);
			if (compteurmode == MOD_MULTI_QUEEN && (the_game.id1 == i
					|| the_game.id2 == i) && SPAWNMODE == 1)
				map_set_cell_type(the_game.map, x, y, CELL_SPAWN);
			else if (player_get_type(the_game.player[i]) == PLAYER_STD)
				map_set_cell_type(the_game.map, x, y, CELL_PLAYER);
			else
				map_set_cell_type(the_game.map, x, y, CELL_MONSTER);
		}
	}
	the_game.number[0] = load_image(IMG_BANNER_0);
	the_game.number[1] = load_image(IMG_BANNER_1);
	the_game.number[2] = load_image(IMG_BANNER_2);
	the_game.number[3] = load_image(IMG_BANNER_3);
	the_game.number[4] = load_image(IMG_BANNER_4);
	the_game.number[5] = load_image(IMG_BANNER_5);
	the_game.number[6] = load_image(IMG_BANNER_6);
	the_game.number[7] = load_image(IMG_BANNER_7);
	the_game.number[8] = load_image(IMG_BANNER_8);
	the_game.number[9] = load_image(IMG_BANNER_9);
	the_game.banner_life = load_image(IMG_BANNER_LIFE);
	the_game.banner_bomb = load_image(IMG_BANNER_BOMB);
	the_game.banner_range = load_image(IMG_BANNER_RANGE);
	the_game.banner_line = load_image(IMG_BANNER_LINE);
	the_game.gagne = load_image(IMG_GAGNE);
	the_game.perdu = load_image(IMG_PERDU);
	the_game.img_joueur1 = load_image(IMG_J1);
	the_game.img_joueur2 = load_image(IMG_J2);
	the_game.img_red_blood = load_image(IMG_RED_BLOOD);
	the_game.img_green_blood = load_image(IMG_GREEN_BLOOD);
	return &the_game;
}

void game_free(t_game game) {
	assert(game);
	int i;
	//free players, bombs, fires, bloods and the map
	for (i = 0; i < MAXPLAYER; i++) {
		if (game->player[i] != NULL) {
			player_free(game->player[i]);
			game->player[i] = NULL;
		}
	}
	for (i = 0; i < MAXBOMBES; i++) {
		if (game->bomb[i] != NULL) {
			bomb_remove(game->bombes, i);
		}
	}
	for (i = 0; i < MAXFIRE; i++) {
		if (game->fire[i] != NULL) {
			fire_remove(game->fires, i);
		}
	}
	for (i = 0; i < MAXBLOOD; i++) {
		if (game->blood[i] != NULL) {
			game_free_blood(game, i);
		}
	}
	map_free(game->map);
	game->map = NULL;
	IA_free(game->ia);
	game->ia = NULL;
	for (i = 0; i < 10; i++)
		SDL_FreeSurface(game->number[i]);
	SDL_FreeSurface(game->banner_life);
	SDL_FreeSurface(game->banner_bomb);
	SDL_FreeSurface(game->banner_range);
	SDL_FreeSurface(game->banner_line);
	SDL_FreeSurface(game->gagne);
	SDL_FreeSurface(game->perdu);
	SDL_FreeSurface(game->img_joueur1);
	SDL_FreeSurface(game->img_joueur2);
	SDL_FreeSurface(game->img_red_blood);
	SDL_FreeSurface(game->img_green_blood);
}

t_player game_get_player(t_game game, int id) {
	assert(game);
	return game->player[id];
}

t_player* game_get_players(t_game game) {
	assert(game);
	return game->players;
}

int game_is_player(t_game game, int i) {
	assert(game);
	if (i < 0)
		return 0;
	else if (i >= MAXPLAYER)
		return 0;
	else if (game->player[i] != NULL)
		return 1;
	else
		return 0;
}

int game_player_is_monster(t_game game, int i) {
	assert(game);
	if (player_get_type(game->player[i]) == PLAYER_MONSTER)
		return 1;
	else
		return 0;
}

void game_free_player(t_game game, int i) {
	assert(game);
	//Free player
	int x = player_get_x(game_get_player(game, i));
	int y = player_get_y(game_get_player(game, i));
	map_set_cell_type(game_get_map(game), x, y, CELL_EMPTY);
	if (player_get_type(game_get_player(game, i)) == PLAYER_STD)
		game_set_blood(game, x, y, 0);
	else
		game_set_blood(game, x, y, 1);
	player_free(game->player[i]);
	game->player[i] = NULL;
}

void game_free_blood(t_game game, int i) {
	assert(game);
	assert(game->blood[i]);
	free(game->blood[i]);
	game->blood[i] = NULL;
}

t_map game_get_map(t_game game) {
	assert(game);
	return game->map;
}

t_bomb* game_get_bombes(t_game game) {
	assert(game);
	return game->bombes;
}
void game_move_player(t_game game, int id1) {
	assert(game);
	int x = 0, y = 0;
	t_player player = game_get_player(game, id1);
	if (player_move(game->players, game->map, id1) == 5) {
		x = player_get_x(player);
		y = player_get_y(player);
		switch (player_get_way(player)) {
		case NORTH:
			y--;
			break;
		case SOUTH:
			y++;
			break;
		case WEST:
			x--;
			break;
		case EAST:
			x++;
			break;
		}
		bomb_be_pushed(game-> bombes, bomb_get_id_by_xy(game->bombes, x, y),
				player_get_way(player));
	}
}
int game_get_player1_id(t_game game) {
	assert(game);
	return game->id1;
}
int game_get_player2_id(t_game game) {
	assert(game);
	return game->id2;
}
int game_get_queen_id(t_game game) {
	assert(game);
	return game->queen;
}
void game_set_player1_id(t_game game, int i) {
	assert(game);
	game->id1 = i;
}
void game_set_player2_id(t_game game, int i) {
	assert(game);
	game->id2 = i;
}
void game_set_queen_id(t_game game, int i) {
	assert(game);
	game->queen = i;
}
void game_set_blood(t_game game, int x, int y, int type) {
	assert(game);
	int i = 0;
	t_blood blood = malloc(sizeof(*blood));
	while (i < MAXBLOOD && game->blood[i] != NULL) {
		i++;
	}
	if (i < MAXBLOOD) {
		game->blood[i] = blood;
		game->blood[i]->x = x;
		game->blood[i]->y = y;
		game->blood[i]->type = type;
	}
}

void game_set_bomb(t_game game, int id, int type) {
	assert(game);
	assert(game->player[id]);
	int x, y, ok = 0, i = 1, range = 1, duration = BOMB_LIFETIME, max,
			continuer = 1;
	x = player_get_x(game->player[id]);
	y = player_get_y(game->player[id]);
	t_player player = game->player[id];
	t_map map = game->map;
	max = player_get_nb_bomb(player);
	if (player_get_disease(player) == DISEASE_TIME_BOMB)
		duration = BOMB_LIFETIME / 4;
	//if there is no bomb where the player is and he has bombs left, set the bomb
	if (player_get_nb_bomb(player) > 0 && (map_get_cell_type(map, x, y)
			== CELL_PLAYER || map_get_cell_type(map, x, y) == CELL_MONSTER)) {
		if (player_get_disease(player) == DISEASE_FIRE)
			range = 1;
		else if (player_get_disease(player) == DISEASE_FIRE2)
			range = 9;
		else
			range = player_get_range(player);
		if (player_has_launch(player)) { //launch bomb
			switch (player_get_way(player)) {
			case NORTH:
				if (map_is_inside(map, x, y - LAUNCH_DISTANCE)) {
					if (map_get_cell_type(map, x, y - LAUNCH_DISTANCE)
							== CELL_EMPTY) {
						bomb_set_bomb(game->bombes, x, y - LAUNCH_DISTANCE,
								range, type, id, duration);
						player_dec_nb_bomb(player);
						map_set_cell_type(game->map, x, y - LAUNCH_DISTANCE,
								CELL_BOMB);
						ok = 1;
					}
				}
				break;
			case SOUTH:
				if (map_is_inside(map, x, y + LAUNCH_DISTANCE)) {
					if (map_get_cell_type(map, x, y + LAUNCH_DISTANCE)
							== CELL_EMPTY) {
						bomb_set_bomb(game->bombes, x, y + LAUNCH_DISTANCE,
								range, type, id, duration);
						player_dec_nb_bomb(player);
						map_set_cell_type(game->map, x, y + LAUNCH_DISTANCE,
								CELL_BOMB);
						ok = 1;
					}
				}
				break;
			case EAST:
				if (map_is_inside(map, x + LAUNCH_DISTANCE, y)) {
					if (map_get_cell_type(map, x + LAUNCH_DISTANCE, y)
							== CELL_EMPTY) {
						bomb_set_bomb(game->bombes, x + LAUNCH_DISTANCE, y,
								range, type, id, duration);
						player_dec_nb_bomb(player);
						map_set_cell_type(game->map, x + LAUNCH_DISTANCE, y,
								CELL_BOMB);
						ok = 1;
					}
				}
				break;
			case WEST:
				if (map_is_inside(map, x - LAUNCH_DISTANCE, y)) {
					if (map_get_cell_type(map, x - LAUNCH_DISTANCE, y)
							== CELL_EMPTY) {
						bomb_set_bomb(game->bombes, x - LAUNCH_DISTANCE, y,
								range, type, id, duration);
						player_dec_nb_bomb(player);
						map_set_cell_type(game->map, x - LAUNCH_DISTANCE, y,
								CELL_BOMB);
						ok = 1;
					}
				}
				break;
			}
			if (!ok) {
				bomb_set_bomb(game->bombes, x, y, range, type, id, duration);
				player_dec_nb_bomb(player);
				map_set_cell_type(game->map, x, y, CELL_BOMB);
			}
		} else { //classic bomb
			bomb_set_bomb(game->bombes, x, y, range, type, id, duration);
			player_dec_nb_bomb(player);
			map_set_cell_type(game->map, x, y, CELL_BOMB);
		}
	} else if (player_get_nb_bomb(player) > 0 && map_get_cell_type(map, x, y)
			== CELL_BOMB && player_has_multi(player)) { //multi bomb
		if (player_get_disease(player) == DISEASE_FIRE)
			range = 1;
		else if (player_get_disease(player) == DISEASE_FIRE2)
			range = 9;
		else
			range = player_get_range(player);
		while (i <= max && continuer) {
			switch (player_get_way(player)) {
			case NORTH:
				if (map_is_inside(map, x, y - i)) {
					if (map_get_cell_type(map, x, y - i) == CELL_EMPTY) {
						bomb_set_bomb(game->bombes, x, y - i, range, type, id,
								duration);
						player_dec_nb_bomb(player);
						map_set_cell_type(game->map, x, y - i, CELL_BOMB);
					} else
						continuer = 0;
				}
				break;
			case SOUTH:
				if (map_is_inside(map, x, y + i)) {
					if (map_get_cell_type(map, x, y + i) == CELL_EMPTY) {
						bomb_set_bomb(game->bombes, x, y + i, range, type, id,
								duration);
						player_dec_nb_bomb(player);
						map_set_cell_type(game->map, x, y + i, CELL_BOMB);
					} else
						continuer = 0;
				}
				break;
			case WEST:
				if (map_is_inside(map, x - i, y)) {
					if (map_get_cell_type(map, x - i, y) == CELL_EMPTY) {
						bomb_set_bomb(game->bombes, x - i, y, range, type, id,
								duration);
						player_dec_nb_bomb(player);
						map_set_cell_type(game->map, x - i, y, CELL_BOMB);
					} else
						continuer = 0;
				}
				break;
			case EAST:
				if (map_is_inside(map, x + i, y)) {
					if (map_get_cell_type(map, x + i, y) == CELL_EMPTY) {
						bomb_set_bomb(game->bombes, x + i, y, range, type, id,
								duration);
						player_dec_nb_bomb(player);
						map_set_cell_type(game->map, x + i, y, CELL_BOMB);
					} else
						continuer = 0;
				}
				break;
			}
			i++;
		}
	}
}

int game_free_dead_players(t_game game) {
	assert(game);
	int k = 0;
	int done = 0;
	for (k = 0; k < MAXPLAYER; k++) {
		if (game_is_player(game, k)) {
			if (player_get_life(game_get_player(game, k)) <= 0) {
				if (k == game_get_player1_id(game))
					game_set_player1_id(game, -1);
				else if (k == game_get_player2_id(game))
					game_set_player2_id(game, -1);
				else if (k == game_get_queen_id(game))
					game_set_queen_id(game, -1);
				assert(game_get_player(game,k));
				game_free_player(game, k);
			} else if (map_get_cell_type(game_get_map(game), player_get_x(
					game_get_player(game, k)), player_get_y(game_get_player(
					game, k))) == CELL_FLAG)
				done = GAGNE;
		}
	}
	return (done);
}
int game_test_win_lose(t_game game) {
	int done = 0, id1, id2;
	id1 = game_get_player1_id(game);
	id2 = game_get_player2_id(game);
	if (!game_is_player(game, id1) && !game_is_player(game, id2)) {
		done = PERDU;
	} else if (game_is_player(game, id1) && !game_is_player(game, id2)
			&& player_get_type(game_get_player(game, id1)) == PLAYER_MONSTER) {
		done = PERDU;
	} else if (!game_is_player(game, id1) && game_is_player(game, id2)
			&& player_get_type(game_get_player(game, id2)) == PLAYER_MONSTER) {
		done = PERDU;
	} else if (game_is_player(game, id1) && game_is_player(game, id2)
			&& player_get_type(game_get_player(game, id1)) == PLAYER_MONSTER
			&& player_get_type(game_get_player(game, id2)) == PLAYER_MONSTER) {
		done = PERDU;
	}
	return done;
}
void game_set_mod(t_game game, int mode) {
	assert(game);
	if (mode == MOD_SOLO_CLASSIC) {
		if (game_is_player(game, 1)) {
			while (player_get_life(game_get_player(game, 1)) > 0) // kill the first perso
				player_dec_life(game_get_player(game, 1));
		}
		if (game_is_player(game, 10)) {
			while (player_get_life(game_get_player(game, 10)) > 0) //kill the queen
				player_dec_life(game_get_player(game, 10));
		}
	} else if (mode == MOD_MULTI_CLASSIC) {
		if (game_is_player(game, 10)) {
			while (player_get_life(game_get_player(game, 10)) > 0) //kill the queen
				player_dec_life(game_get_player(game, 10));
		}
	} else if (mode == MOD_MULTI_QUEEN) {
		//nothing
	}
}
void game_do_move(t_game game) {
	assert(game);
	int i;
	for (i = 0; i < MAXPLAYER; i++) {
		if (game_is_player(game, i)) {
			if (player_is_movable(game->player[i])) {
				switch (player_get_next_move(game->player[i])) {
				case NORTH:
					player_set_current_way(game->player[i], NORTH);
					game_move_player(game, i);
					break;
				case SOUTH:
					player_set_current_way(game->player[i], SOUTH);
					game_move_player(game, i);
					break;
				case WEST:
					player_set_current_way(game->player[i], WEST);
					game_move_player(game, i);
					break;
				case EAST:
					player_set_current_way(game->player[i], EAST);
					game_move_player(game, i);
					break;
				case 4:
					game_set_bomb(game, i, BOMB_STD);
					break;
				default:
					break;
				}
				player_set_next_move(game->player[i], -1);
			}
			if (player_get_next_move(game->player[i]) == -1
					&& player_get_disease(game->player[i]) == DISEASE_BOMBS)
				game_set_bomb(game, i, BOMB_STD);
		}
	}
}
void game_smooth_rolling_bombs(t_game game) {
	assert(game);
	int j, direction;
	float small_step = (SIZE_BLOC * ROLL_BOMB_PER_SEC
			* game_time_get_cycles_length());
	small_step /= 1000;
	for (j = 0; j < MAXBOMBES; j++) {
		if (game->bomb[j] != NULL && bomb_is_rolling(game->bombes, j)) {
			if (bomb_get_move_timer(game->bomb[j]) < (int) 1000
					/ ROLL_BOMB_PER_SEC) {
				direction = bomb_get_way(game->bombes, j);
				switch (direction) {
				case NORTH:
					bomb_inc_resty(game->bomb[j], -small_step);
					break;
				case SOUTH:
					bomb_inc_resty(game->bomb[j], small_step);
					break;
				case WEST:
					bomb_inc_restx(game->bomb[j], -small_step);
					break;
				case EAST:
					bomb_inc_restx(game->bomb[j], small_step);
					break;
				}
				bomb_inc_move_timer(game->bomb[j],
						game_time_get_cycles_length());
			} else {
				bomb_set_move_time(game->bomb[j], 0);
				bomb_roll(game->bombes, game->map);
			}
		}
	}
}
void game_smooth_move(t_game game) {
	assert(game);
	int j, direction = 0, speed = 0;
	float small_step = 0;// (SIZE_BLOC * MONSTER_MOVE_PER_SEC
	//* game_time_get_cycles_length()), small_step2 = (SIZE_BLOC
	//* PLAYER_MOVE_PER_SEC * game_time_get_cycles_length());
	//small_step /= 1000;
	//small_step2 /= 1000;
	for (j = 0; j < MAXPLAYER; j++) {
		if (game_is_player(game, j)) {
			if (player_get_disease(game->player[j]) == DISEASE_SPEED) {
				small_step = (SIZE_BLOC * 3 * game_time_get_cycles_length());
				small_step /= 1000;
				speed = 3;
			} else if (player_get_disease(game->player[j]) == DISEASE_SPEED2) {
				small_step = (SIZE_BLOC * 10 * game_time_get_cycles_length());
				small_step /= 1000;
				speed = 10;
			} else {
				small_step = (SIZE_BLOC * player_get_speed(game->player[j])
						* game_time_get_cycles_length());
				small_step /= 1000;
				speed = player_get_speed(game->player[j]);
			}
			if ((player_get_type(game->player[j]) == PLAYER_MONSTER
					&& player_get_move_timer(game->player[j]) >= (int) (1000
							/ speed)) || (player_get_type(game->player[j])
					== PLAYER_STD && player_get_move_timer(game->player[j])
					>= (int) (1000 / speed))) {
				player_set_move_timer(game->player[j], 0);
				//Smooth move finished, player is at the new location
				player_set_movable(game->player[j], 1); //he can move again
				player_set_restx(game->player[j], 0);
				player_set_resty(game->player[j], 0);
				player_inc_posx(game->player[j], player_get_restx(
						game->player[j]) / SIZE_BLOC);
				player_inc_posy(game->player[j], player_get_resty(
						game->player[j]) / SIZE_BLOC);

			} else { //this is doing the smooth move if the player is not movable(because moving)
				if (!player_is_movable(game->player[j])) {
					direction = player_get_way(game->player[j]);
					switch (direction) {
					case NORTH:
						//if (player_get_type(game->player[j]) == PLAYER_STD)
						//	player_inc_resty(game->player[j], -small_step2);
						//else
						player_inc_resty(game->player[j], -small_step);
						break;
					case SOUTH:
						//if (player_get_type(game->player[j]) == PLAYER_STD)
						//	player_inc_resty(game->player[j], small_step2);
						//else
						player_inc_resty(game->player[j], small_step);
						break;
					case WEST:
						//if (player_get_type(game->player[j]) == PLAYER_STD)
						//	player_inc_restx(game->player[j], -small_step2);
						//else
						player_inc_restx(game->player[j], -small_step);
						break;
					case EAST:
						//if (player_get_type(game->player[j]) == PLAYER_STD)
						//	player_inc_restx(game->player[j], small_step2);
						//else
						player_inc_restx(game->player[j], small_step);
						break;
					}
					player_inc_move_timer(game->player[j],
							game_time_get_cycles_length());
				}
			}
		}
	}
}
void game_treatment(t_game game, int counter1) {
	assert(game);
	int i = 0;

	//BOMBS AND FIRE
	//decrement bombs
	bomb_decrement(game->bombes, game_time_get_cycles_length());
	//explode finished bombs
	bomb_explode(game->players, game->fires, game->bombes, game->map);

	//decrement fires
	fire_decrement(game->fires, game_time_get_cycles_length());
	//Hurt people in the fire
	fire_hurt_player(game->players, game->fires, game->map);
	//disease decrement
	player_disease_decrement(game->players, game_time_get_cycles_length());

	//Make the moves of the players
	game_do_move(game);
	//loop for players
	for (i = 0; i < MAXPLAYER; i++) {
		if (game_is_player(game, i)) {
			//Decrement hit time
			player_dec_hit_time(game->player[i], game_time_get_cycles_length());
			//Decrement respawntime
			if (player_get_respawn_time(game->player[i]) > 0)
				player_dec_respawn_time(game->player[i],
						game_time_get_cycles_length());
		}
	}

	//smooth move for players/monsters
	game_smooth_move(game);
	//smooth the rolling bombs
	game_smooth_rolling_bombs(game);

	//grow trees
	if (counter1 > (int) 1000 / GROW_TREE_PER_SEC) {
		map_grow_trees(game->map);
	}
	//Move monsters
	//if (counter1 > (int) 1000 / MONSTER_MOVE_PER_SEC) {
	//IA_upgrade_grid(game->ia, game->map, game->fires);
	IA_do(game->map, game->players, game->bombes, game->fires, game->ia,
			game->queen, game->id1, game->id2);
	//}
}

void game_banner_display(t_game game, SDL_Surface *screen) {
	assert(game);
	assert(screen);

	t_map map = game_get_map(game);

	SDL_Rect place;

	int i;
	int life = 0;
	int bomb_range = 0;
	place.y = (map_get_height(map)) * SIZE_BLOC;
	for (i = 0; i < map_get_width(map); i++) {
		place.x = i * SIZE_BLOC;
		SDL_BlitSurface(game->banner_line, NULL, screen, &place);
	}

	place.y = (map_get_height(map) * SIZE_BLOC) + SIZE_LINE;
	place.x = 0;
	SDL_BlitSurface(game->img_joueur1, NULL, screen, &place);
	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;

	place.x = white_bloc;
	SDL_BlitSurface(game->banner_life, NULL, screen, &place);

	place.x = white_bloc + SIZE_BLOC;
	if (game_is_player(game, game->id1) && player_get_type(game_get_player(
			game, game->id1)) == PLAYER_STD) {
		if (player_get_life(game_get_player(game, game->id1)) >= 0
				&& player_get_life(game_get_player(game, game->id1)) < 10)
			life = player_get_life(game_get_player(game, game->id1));
		else
			life = 0;
	}
	SDL_BlitSurface(game->number[life], NULL, screen, &place);

	place.x = 2 * white_bloc + 2 * SIZE_BLOC;
	SDL_BlitSurface(game->banner_bomb, NULL, screen, &place);
	place.x = 2 * white_bloc + 3 * SIZE_BLOC;

	if (game_is_player(game, game->id1) && player_get_type(game_get_player(
			game, game->id1)) == PLAYER_STD && player_get_nb_bomb(
			game_get_player(game, game->id1)) >= 0 && player_get_nb_bomb(
			game_get_player(game, game->id1)) < 10)
		SDL_BlitSurface(game->number[player_get_nb_bomb(game_get_player(game,
				game->id1))], NULL, screen, &place);
	else
		SDL_BlitSurface(game->number[0], NULL, screen, &place);

	place.x = 3 * white_bloc + 4 * SIZE_BLOC;
	SDL_BlitSurface(game->banner_range, NULL, screen, &place);
	place.x = 3 * white_bloc + 5 * SIZE_BLOC;

	if (game_is_player(game, game->id1) && player_get_type(game_get_player(
			game, game->id1)) == PLAYER_STD && player_get_range(
			game_get_player(game, game->id1)) >= 0 && player_get_range(
			game_get_player(game, game->id1)) < 10) {
		bomb_range = player_get_range(game_get_player(game, game->id1));
		SDL_BlitSurface(game->number[bomb_range], NULL, screen, &place);
	} else
		SDL_BlitSurface(game->number[0], NULL, screen, &place);
}
void game_banner_display2(t_game game, SDL_Surface *screen) {
	assert(game);
	assert(screen);

	t_map map = game_get_map(game);

	SDL_Rect place;

	int i;
	int life = 0;
	int bomb_range = 0;
	place.y = (map_get_height(map)) * SIZE_BLOC + SIZE_BANNER;
	for (i = 0; i < map_get_width(map); i++) {
		place.x = i * SIZE_BLOC;
		SDL_BlitSurface(game->banner_line, NULL, screen, &place);
	}

	place.y = (map_get_height(map) * SIZE_BLOC) + SIZE_LINE + SIZE_BANNER;
	place.x = 0;
	SDL_BlitSurface(game->img_joueur2, NULL, screen, &place);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;

	place.x = white_bloc;
	SDL_BlitSurface(game->banner_life, NULL, screen, &place);
	place.x = white_bloc + SIZE_BLOC;
	if (game_is_player(game, game->id2) && player_get_type(game_get_player(
			game, game->id2)) == PLAYER_STD) {
		if (player_get_life(game_get_player(game, game->id2)) >= 0
				&& player_get_life(game_get_player(game, game->id2)) < 10)
			life = player_get_life(game_get_player(game, game->id2));
		else
			life = 0;
	}
	SDL_BlitSurface(game->number[life], NULL, screen, &place);

	place.x = 2 * white_bloc + 2 * SIZE_BLOC;
	SDL_BlitSurface(game->banner_bomb, NULL, screen, &place);
	place.x = 2 * white_bloc + 3 * SIZE_BLOC;

	if (game_is_player(game, game->id2) && player_get_type(game_get_player(
			game, game->id2)) == PLAYER_STD && player_get_nb_bomb(
			game_get_player(game, game->id2)) >= 0 && player_get_nb_bomb(
			game_get_player(game, game->id2)) < 10)
		SDL_BlitSurface(game->number[player_get_nb_bomb(game_get_player(game,
				game->id2))], NULL, screen, &place);
	else
		SDL_BlitSurface(game->number[0], NULL, screen, &place);

	place.x = 3 * white_bloc + 4 * SIZE_BLOC;
	SDL_BlitSurface(game->banner_range, NULL, screen, &place);
	place.x = 3 * white_bloc + 5 * SIZE_BLOC;

	if (game_is_player(game, game->id2) && player_get_type(game_get_player(
			game, game->id2)) == PLAYER_STD && player_get_range(
			game_get_player(game, game->id2)) >= 0 && player_get_range(
			game_get_player(game, game->id2)) < 10) {
		bomb_range = player_get_range(game_get_player(game, game->id2));
		SDL_BlitSurface(game->number[bomb_range], NULL, screen, &place);
	} else
		SDL_BlitSurface(game->number[0], NULL, screen, &place);
}
void game_blood_display(t_game game, SDL_Surface *screen) {
	assert(game);
	int i;
	for (i = 0; i < MAXBLOOD; i++) {
		if (game->blood[i] != NULL) {
			SDL_Rect place;
			place.x = game->blood[i]->x * SIZE_BLOC;
			place.y = game->blood[i]->y * SIZE_BLOC;
			if (game->blood[i]->type == 0) {
				//red
				SDL_BlitSurface(game->img_red_blood, NULL, screen, &place);
			} else {
				//green
				SDL_BlitSurface(game->img_green_blood, NULL, screen, &place);
			}
		}
	}
}

void game_display(t_game game, SDL_Surface *screen, int mode, int status) {
	assert(game);
	assert(screen);
	int i;
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
	if (mode == MOD_SOLO_CLASSIC) {
		game_banner_display(game, screen);
	} else {
		game_banner_display(game, screen);
		game_banner_display2(game, screen);
	}
	game_blood_display(game, screen);
	map_display(game->map, screen);
	bomb_display(game->map, game->bombes, screen);
	for (i = 0; i < MAXPLAYER; i++) {
		if (game_is_player(game, i)) {
			player_display(game->player[i], screen);
		}
	}
	fire_display(game->map, game->fires, screen);
	game_display_end(game, screen, status); //displays "win" or "lost" or nothing
	SDL_Flip(screen);
}

void game_display_end(t_game game, SDL_Surface *screen, int status) {
	assert(game);
	assert(screen);

	SDL_Rect position;
	position.x = SIZE_BLOC * map_get_width(game_get_map(game)) / 2 - 212;
	position.y = SIZE_BLOC * map_get_height(game_get_map(game)) / 2 - 71;
	if (status == GAGNE) {
		SDL_BlitSurface(game->gagne, NULL, screen, &position);
		//SDL_Flip(screen);
		//SDL_Delay(2000);
	} else if (status == PERDU) {
		SDL_BlitSurface(game->perdu, NULL, screen, &position);
		//SDL_Flip(screen);
		//SDL_Delay(2000);
	}
}
