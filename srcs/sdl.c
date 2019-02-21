/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 17:15:55 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/20 22:48:55 by llelievr         ###   ########.fr       */
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
	//SDL_Set
	wolf->screen = SDL_CreateTexture(wolf->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, S_WIDTH, S_HEIGHT);
	if (!(wolf->pixels = (unsigned char *)malloc(S_WIDTH * S_HEIGHT * 4)))
		return ; //TODO malloc fail;
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
}
