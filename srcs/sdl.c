/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 17:15:55 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/27 22:08:39 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void		sdl_init(t_wolf *wolf)
{
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	wolf->win = SDL_CreateWindow("|| Wolf 3D ||", 0, 0, 1000, 1000, SDL_WINDOW_SHOWN);
	wolf->renderer = SDL_CreateRenderer(wolf->win, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetLogicalSize(wolf->renderer, S_WIDTH, S_HEIGHT);
	wolf->screen = SDL_CreateTexture(wolf->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, S_WIDTH, S_HEIGHT);
	
	if (!(wolf->img = (t_img *)malloc(sizeof(t_img))))
		return ; //TODO: malloc fail;
	wolf->img->width = S_WIDTH;
	wolf->img->height = S_HEIGHT;
	wolf->img->size = IMG_MAX_I;
	if (!(wolf->img->pixels = malloc(wolf->img->size * 4)))
		return ; //TODO: malloc fail;
	wolf->fonts.helvetica = TTF_OpenFont("assets/fonts/helvetica.ttf", 22);
	wolf->running = TRUE;
}

void		sdl_quit(t_wolf *wolf)
{
	SDL_DestroyTexture(wolf->screen);
	SDL_DestroyRenderer(wolf->renderer);
	SDL_DestroyWindow(wolf->win);
	TTF_CloseFont(wolf->fonts.helvetica);
	TTF_Quit();
	SDL_Quit();
	free(wolf->img->pixels);
	free(wolf->img);
}
