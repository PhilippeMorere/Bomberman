/*
 * misc.c
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent R�veill�re
 */

#include "../include/misc.h"
#include "../include/constant.h"

void error(const char *s, ...) {
	va_list ap;
	va_start(ap, s);

	vfprintf(stderr, s, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}

SDL_Surface *load_image(const char *filename) {
	SDL_Surface *img = IMG_Load(filename);
	if (!img) {
		error("IMG_Load: %s\n", IMG_GetError());
	}
	return img;
}
