/*
 * main.c
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent R�veill�re
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <SDL.h>
#include <SDL_image.h>
#include "../include/map.h"
#include "../include/game.h"
#include "../include/game_time.h"
int countermod;

// Keyboard management
typedef struct {
	char key[SDLK_LAST];
	int mousex, mousey;
	int mousexrel, mouseyrel;
	char mousebuttons[8];
	char quit;
} Input;

void UpdateEvents(Input* in) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			in->key[event.key.keysym.sym] = 1;
			break;
		case SDL_KEYUP:
			in->key[event.key.keysym.sym] = 0;
			break;
		case SDL_MOUSEMOTION:
			in->mousex = event.motion.x;
			in->mousey = event.motion.y;
			in->mousexrel = event.motion.xrel;
			in->mouseyrel = event.motion.yrel;
			break;
		case SDL_MOUSEBUTTONDOWN:
			in->mousebuttons[event.button.button] = 1;
			break;
		case SDL_MOUSEBUTTONUP:
			in->mousebuttons[event.button.button] = 0;
			break;
		case SDL_QUIT:
			in->quit = 1;
			break;
		default:
			break;
		}
	}
}
int pause_menu(t_game game) {
	assert(game);
	//afficher menu de pause
	int again = 1;
	assert(game);
	SDL_Event event;
	while (again) {
		SDL_WaitEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
			again = 0;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				again = 0;
				break;
			case SDLK_F1:
				save(game, countermod);
				again = 1;
				break;
			case SDLK_SPACE:
				return 0;
				break;
			default:
				again = 1;
				break;
			}
			break;
		default:
			again = 1;
			break;
		}
	}
	return 1;
}

// Keyboard management
int input_update(t_game game, Input* in, SDL_Rect positionbreak,
		SDL_Surface *menubreak, SDL_Surface *screen) {
	assert(game);
	assert(screen);
	assert(menubreak);
	UpdateEvents(in);
	int id1, id2, i, move1, move2;
	t_player player = NULL;
	t_player player1 = NULL;
	move1 = 0;
	move2 = 0;
	id1 = game_get_player1_id(game);
	id2 = game_get_player2_id(game);
	if (id1 != -1) {
		player = game_get_player(game, id1);
		if (player_get_move_timer(player) == 0 || player_get_move_timer(player)
				>= (int) (500 / player_get_speed(player)))
			move1 = 1;

	}
	if (id2 != -1) {
		player1 = game_get_player(game, id2);
		if (player_get_move_timer(player1) == 0 || player_get_move_timer(
				player1) >= (int) (500 / player_get_speed(player1)))
			move2 = 1;
	}

	//events
	if (in->quit)
		return 1;
	if (in->key[SDLK_ESCAPE]) {
		SDL_BlitSurface(menubreak, NULL, screen, &positionbreak);
		SDL_Flip(screen);
		for (i = 0; i < SDLK_LAST; i++)
			in->key[i] = 0;

		if (pause_menu(game) == 0)
			return 1;
	}
	if (in->key[SDLK_UP]) {
		if (move1 && player != NULL && player_get_type(player) == PLAYER_STD
				&& player_get_next_move(player) != 4) {
			if (player_get_disease(player) == DISEASE_MOVE)
				player_set_next_move(player, SOUTH);
			else if (player_get_disease(player) != DISEASE_FREEZE)
				player_set_next_move(player, NORTH);
		}
	} else if (in->key[SDLK_DOWN]) {
		if (move1 && player != NULL && player_get_type(player) == PLAYER_STD
				&& player_get_next_move(player) != 4) {
			if (player_get_disease(player) == DISEASE_MOVE)
				player_set_next_move(player, NORTH);
			else if (player_get_disease(player) != DISEASE_FREEZE)
				player_set_next_move(player, SOUTH);
		}
	} else if (in->key[SDLK_RIGHT]) {
		if (move1 && player != NULL && player_get_type(player) == PLAYER_STD
				&& player_get_next_move(player) != 4) {
			if (player_get_disease(player) == DISEASE_MOVE)
				player_set_next_move(player, WEST);
			else if (player_get_disease(player) != DISEASE_FREEZE)
				player_set_next_move(player, EAST);
		}
	} else if (in->key[SDLK_LEFT]) {
		if (move1 && player != NULL && player_get_type(player) == PLAYER_STD
				&& player_get_next_move(player) != 4) {
			if (player_get_disease(player) == DISEASE_MOVE)
				player_set_next_move(player, EAST);
			else if (player_get_disease(player) != DISEASE_FREEZE)
				player_set_next_move(player, WEST);
		}
	}
	if (in->key[SDLK_SPACE]) {
		if (player != NULL && player_get_type(player) == PLAYER_STD) {
			player_set_next_move(player, 4);
			in->key[SDLK_SPACE] = 0;
		}
	}
	if (in->key[SDLK_z]) {
		if (move2 && player1 != NULL && player_get_type(player1) == PLAYER_STD
				&& player_get_next_move(player1) != 4) {
			if (player_get_disease(player1) == DISEASE_MOVE)
				player_set_next_move(player1, SOUTH);
			else if (player_get_disease(player1) != DISEASE_FREEZE)
				player_set_next_move(player1, NORTH);
		}
	} else if (in->key[SDLK_s]) {
		if (move2 && player1 != NULL && player_get_type(player1) == PLAYER_STD
				&& player_get_next_move(player1) != 4) {
			if (player_get_disease(player1) == DISEASE_MOVE)
				player_set_next_move(player1, NORTH);
			else if (player_get_disease(player1) != DISEASE_FREEZE)
				player_set_next_move(player1, SOUTH);
		}
	} else if (in->key[SDLK_d]) {
		if (move2 && player1 != NULL && player_get_type(player1) == PLAYER_STD
				&& player_get_next_move(player1) != 4) {
			if (player_get_disease(player1) == DISEASE_MOVE)
				player_set_next_move(player1, WEST);
			else if (player_get_disease(player1) != DISEASE_FREEZE)
				player_set_next_move(player1, EAST);
		}
	} else if (in->key[SDLK_q]) {
		if (move2 && player1 != NULL && player_get_type(player1) == PLAYER_STD
				&& player_get_next_move(player1) != 4) {
			if (player_get_disease(player1) == DISEASE_MOVE)
				player_set_next_move(player1, EAST);
			else if (player_get_disease(player1) != DISEASE_FREEZE)
				player_set_next_move(player1, WEST);
		}
	}
	if (in->key[SDLK_LCTRL]) {
		if (player1 != NULL && player_get_type(player1) == PLAYER_STD) {
			player_set_next_move(player1, 4);
			in->key[SDLK_LCTRL] = 0;
		}
	}
	return 0;
}
/*
 int input_update(t_game game, SDL_Rect positionbreak, SDL_Surface *menubreak,
 SDL_Surface *screen) {
 assert(game);
 assert(screen);
 assert(menubreak);
 SDL_Event event;
 int id1, id2;
 t_player player = NULL;
 t_player player1 = NULL;
 id1 = game_get_player1_id(game);
 id2 = game_get_player2_id(game);
 if (id1 != -1)
 player = game_get_player(game, id1);
 if (id2 != -1)
 player1 = game_get_player(game, id2);

 while (SDL_PollEvent(&event)) {
 switch (event.type) {
 case SDL_QUIT:
 return 1;
 case SDL_KEYDOWN:
 switch (event.key.keysym.sym) {
 case SDLK_ESCAPE:
 SDL_BlitSurface(menubreak, NULL, screen, &positionbreak);
 SDL_Flip(screen);
 if (pause_menu(game) == 0) {
 return 1;
 }
 break;
 case SDLK_UP:
 if (player != NULL && player_get_type(player) == PLAYER_STD) {
 if (player_get_disease(player) == DISEASE_MOVE)
 player_set_next_move(player, SOUTH);
 else if (player_get_disease(player) != DISEASE_FREEZE)
 player_set_next_move(player, NORTH);
 }
 break;
 case SDLK_DOWN:
 if (player != NULL && player_get_type(player) == PLAYER_STD) {
 if (player_get_disease(player) == DISEASE_MOVE)
 player_set_next_move(player, NORTH);
 else if (player_get_disease(player) != DISEASE_FREEZE)
 player_set_next_move(player, SOUTH);
 }
 break;
 case SDLK_RIGHT:
 if (player != NULL && player_get_type(player) == PLAYER_STD) {
 if (player_get_disease(player) == DISEASE_MOVE)
 player_set_next_move(player, WEST);
 else if (player_get_disease(player) != DISEASE_FREEZE)
 player_set_next_move(player, EAST);
 }
 break;
 case SDLK_LEFT:
 if (player != NULL && player_get_type(player) == PLAYER_STD) {
 if (player_get_disease(player) == DISEASE_MOVE)
 player_set_next_move(player, EAST);
 else if (player_get_disease(player) != DISEASE_FREEZE)
 player_set_next_move(player, WEST);
 }
 break;
 case SDLK_SPACE:
 if (player != NULL && player_get_type(player) == PLAYER_STD) {
 player_set_next_move(player, 4);
 }
 break;
 case SDLK_z:
 if (player1 != NULL && player_get_type(player1) == PLAYER_STD
 && player_is_movable(player1)) {
 if (player_get_disease(player1) == DISEASE_MOVE)
 player_set_next_move(player1, SOUTH);
 else if (player_get_disease(player1) != DISEASE_FREEZE)
 player_set_next_move(player1, NORTH);
 }
 break;
 case SDLK_s:
 if (player1 != NULL && player_get_type(player1) == PLAYER_STD) {
 if (player_get_disease(player1) == DISEASE_MOVE)
 player_set_next_move(player1, NORTH);
 else if (player_get_disease(player1) != DISEASE_FREEZE)
 player_set_next_move(player1, SOUTH);
 }
 break;
 case SDLK_d:
 if (player1 != NULL && player_get_type(player1) == PLAYER_STD) {
 if (player_get_disease(player1) == DISEASE_MOVE)
 player_set_next_move(player1, WEST);
 else if (player_get_disease(player1) != DISEASE_FREEZE)
 player_set_next_move(player1, EAST);
 }
 break;
 case SDLK_q:
 if (player1 != NULL && player_get_type(player1) == PLAYER_STD) {
 if (player_get_disease(player1) == DISEASE_MOVE)
 player_set_next_move(player1, EAST);
 else if (player_get_disease(player1) != DISEASE_FREEZE)
 player_set_next_move(player1, WEST);
 }
 break;
 case SDLK_LCTRL:
 if (player1 != NULL && player_get_type(player1) == PLAYER_STD) {
 player_set_next_move(player1, 4);
 }
 break;
 default:
 ;
 }

 break;
 }
 }
 return 0;
 }
 */
int main(int argc, char *argv[]) {
	int i, again, countermap = 1, count = 1;
	countermod = 0;
	t_game game;
	again = 1;
	Input in;
	SDL_Surface * screen;

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		error("Can't init SDL:  %s\n", SDL_GetError());
	}

	int dimx = 650; // size of main menu
	int dimy = 650;
	SDL_WM_SetCaption(WINDOW_NAME, NULL);

	screen = SDL_SetVideoMode(dimx, dimy, WINDOW_BPP, SDL_HWSURFACE);
	if (screen == NULL) {
		error("Can't set video mode: %s\n", SDL_GetError());
	}

	SDL_EnableKeyRepeat(10, 100); //10 100
	SDL_Event event;
	SDL_Rect positionMenu;
	SDL_Rect positioncounter;
	SDL_Rect positionmap;
	SDL_Rect positionbreak;
	SDL_Rect positionmod;
	SDL_Surface *menu = NULL;
	SDL_Surface *numbers[10];
	SDL_Surface *mod[3];
	SDL_Surface *menubreak = NULL;

	// Load Pictures
	//menu
	menu = IMG_Load("sprite/menu1.png");
	//mod
	mod[0] = load_image("sprite/mode1.png");
	mod[1] = load_image("sprite/mode2.png");
	mod[2] = load_image("sprite/mode3.png");
	//numbers
	numbers[0] = load_image("sprite/banner_0.jpg");
	numbers[1] = load_image("sprite/banner_1.jpg");
	numbers[2] = load_image("sprite/banner_2.jpg");
	numbers[3] = load_image("sprite/banner_3.jpg");
	numbers[4] = load_image("sprite/banner_4.jpg");
	numbers[5] = load_image("sprite/banner_5.jpg");
	numbers[6] = load_image("sprite/banner_6.jpg");
	numbers[7] = load_image("sprite/banner_7.jpg");
	numbers[8] = load_image("sprite/banner_8.jpg");
	numbers[9] = load_image("sprite/banner_9.jpg");
	//menubreak
	menubreak = load_image("sprite/pause.png");

	//positions
	//mod
	positionmod.x = dimx / 2 + 26;
	positionmod.y = dimy / 2 + 85;
	//menu
	positionMenu.x = dimx / 2 - 225;
	positionMenu.y = dimy / 2 - 185;
	//counter
	positioncounter.x = dimx / 2 + 66;
	positioncounter.y = dimy / 2 + 33;
	//map
	positionmap.x = dimx / 2 + 66;
	positionmap.y = dimy / 2 + 145;
	while (again) {
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
		// building of main menu
		SDL_BlitSurface(menu, NULL, screen, &positionMenu);
		SDL_BlitSurface(numbers[count], NULL, screen, &positioncounter);
		SDL_BlitSurface(mod[countermod], NULL, screen, &positionmod);
		SDL_BlitSurface(numbers[countermap], NULL, screen, &positionmap);
		SDL_Flip(screen);
		SDL_WaitEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
			again = 0;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_F1: // choice of difficult level
				switch (count) {
				case 1:
					count = 2;
					break;
				case 2:
					count = 3;
					break;
				case 3:
					count = 1;
					break;
				}
				break;
			case SDLK_F2: // choice of mode
				switch (countermod) {
				case 0:
					countermod = 1;
					break;
				case 1:
					countermod = 2;
					break;
				case 2:
					countermod = 0;
					break;
				}

				break;
			case SDLK_F3: // choice of map
				switch (countermap) {
				case 0:
					countermap = 1;
					break;
				case 1:
					countermap = 2;
					break;
				case 2:
					countermap = 3;
					break;
				case 3:
					countermap = 4;
					break;
				case 4:
					countermap = 5;
					break;
				case 5:
					countermap = 6;
					break;
				case 6:
					countermap = 7;
					break;
				case 7:
					countermap = 0;
					break;
				}
				break;
			case SDLK_UP:
				//initialization
				memset(&in, 0, sizeof(in));
				game = game_new(countermap, countermod, count);
				if (countermap == 0) {
					int *temp = get_parameter();
					countermod = temp[0];
				}
				game_set_mod(game, countermod);
				if (countermod == MOD_SOLO_CLASSIC) {
					screen = SDL_SetVideoMode(SIZE_BLOC * map_get_width(
							game_get_map(game)), SIZE_BLOC * map_get_height(
							game_get_map(game)) + SIZE_BANNER + SIZE_LINE,
							WINDOW_BPP, SDL_HWSURFACE);
				} else {
					screen = SDL_SetVideoMode(SIZE_BLOC * map_get_width(
							game_get_map(game)), SIZE_BLOC * map_get_height(
							game_get_map(game)) + 2 * SIZE_BANNER + SIZE_LINE,
							WINDOW_BPP, SDL_HWSURFACE);
				}
				positionbreak.x = SIZE_BLOC * map_get_width(game_get_map(game))
						/ 2 - 150;
				positionbreak.y = SIZE_BLOC
						* map_get_height(game_get_map(game)) / 2 - 100;
				int done = 0, status = 0, timer_win = 0, counter2 = 0;
				while (!done) {
					game_time_update();
					int k;
					for (k = 0; k < game_time_get_cycles_to_calculate(); k++) {
						//treatments
						game_treatment(game, counter2);
						//timers
						if (counter2 > (int) 1000 / GROW_TREE_PER_SEC)
							counter2 = 0;
						else
							counter2 += game_time_get_cycles_length();
					}
					if (status == 0)
						done = input_update(game, &in, positionbreak,
								menubreak, screen); //keyboard control
					if (done == 0) {
						game_display(game, screen, countermod, status); //display
						done = game_free_dead_players(game); //remove monsters with 0 hp
					}
					if (done == 0) {
						done = game_test_win_lose(game); //win or lost?
					}
					if ((done != 0 && done != 1 && timer_win == 0) //if its a win or a loss, display it
							|| (timer_win > 0 && timer_win < TIME_AFTER_WIN)) { //and the game continues
						if (done != 0) { //during TIME_AFTER_WIN
							status = done;
							done = 0;
						}
						timer_win += game_time_get_cycles_length();
						if (timer_win >= TIME_AFTER_WIN) {
							done = 1;
						}
					}
				}
				game_free(game);

				//back to the main page
				screen
						= SDL_SetVideoMode(dimx, dimy, WINDOW_BPP,
								SDL_HWSURFACE);

				if (screen == NULL) {
					error("Can't set video mode: %s\n", SDL_GetError());
				}
				assert(screen);
				countermod = 0;
				count = 1;
				countermap = 1;
				break;
			case SDLK_SPACE:
				again = 0;
				break;
			default:
				;
				break;
			}
			break;
		default:
			;
			break;
		}
	}
	//free every surface
	SDL_FreeSurface(menubreak);
	SDL_FreeSurface(menu);
	for (i = 0; i < 3; i++) {
		SDL_FreeSurface(mod[i]);
	}
	for (i = 0; i < 10; i++) {
		SDL_FreeSurface(numbers[i]);
	}
	SDL_FreeSurface(screen);
	SDL_Quit();
	return EXIT_SUCCESS;
}
