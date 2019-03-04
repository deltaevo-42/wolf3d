/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 17:15:55 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/04 15:53:50 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

t_bool		sdl_init(t_wolf *wolf)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0
		|| IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG
		|| TTF_Init() < 0
		|| !(wolf->win = SDL_CreateWindow("|| Wolf 3D ||",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
			SDL_WINDOW_SHOWN))
		|| !(wolf->renderer = SDL_CreateRenderer(wolf->win, -1,
			SDL_RENDERER_ACCELERATED)))
		return (sdl_quit(wolf));
	SDL_RenderSetLogicalSize(wolf->renderer, S_WIDTH, S_HEIGHT);
	if (!(wolf->screen = SDL_CreateTexture(wolf->renderer,
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
		S_WIDTH, S_HEIGHT)))
		return (sdl_quit(wolf));
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_SetWindowGrab(wolf->win, SDL_TRUE);
	if (!(wolf->img = (t_img *)malloc(sizeof(t_img))))
		return (sdl_quit(wolf));
	*wolf->img = (t_img) { .width = S_WIDTH, .height = S_HEIGHT,
							.size = IMG_MAX_I, .pixels = NULL };
	if (!(wolf->fonts.helvetica = TTF_OpenFont("assets/fonts/helvetica.ttf",
		25)) || !(wolf->img->pixels = malloc(wolf->img->size * 4)))
		return (sdl_quit(wolf));
	return (wolf->running = TRUE);
}

t_bool		sdl_quit(t_wolf *wolf)
{
	if (wolf->screen)
		SDL_DestroyTexture(wolf->screen);
	if (wolf->renderer)
		SDL_DestroyRenderer(wolf->renderer);
	if (wolf->win)
		SDL_DestroyWindow(wolf->win);
	if (wolf->fonts.helvetica)
		TTF_CloseFont(wolf->fonts.helvetica);
	TTF_Quit();
	SDL_Quit();
	if (wolf->img)
		free(wolf->img->pixels);
	free(wolf->img);
	return (FALSE);
}
