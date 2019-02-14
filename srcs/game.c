/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 17:51:33 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/14 17:27:22 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

static void		update_fps(t_wolf *wolf, clock_t t1, int *frames)
{
	(*frames)++;
	wolf->stats.delta += clock() - t1;
	if (ticks_to_ms(wolf->stats.delta) > 1000.0)
	{
		wolf->stats.fps = (double)*frames / 2 + wolf->stats.fps / 2;
		*frames = 0;
		wolf->stats.delta -= CLOCKS_PER_SEC;
		wolf->stats.avg_ms = 1000 / (wolf->stats.fps == 0 ? 0.001 : wolf->stats.fps);
	}
}

void			game_loop(t_wolf *wolf)
{
	clock_t		t1;
	int			frames;

	frames = 0;
	wolf->stats.fps = 0;
	wolf->stats.delta = 0;
	wolf->player.pos = (t_vec2){ 12, 12 };
	while (wolf->running)
	{
		t1 = clock();
		SDL_SetRenderDrawColor(wolf->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(wolf->renderer);
		SDL_SetRenderTarget(wolf->renderer, wolf->screen);
		render_main(wolf);
		update_fps(wolf, t1, &frames);
		SDL_UpdateTexture(wolf->screen, NULL, wolf->pixels, S_WIDTH * 4);
		SDL_SetRenderTarget(wolf->renderer, NULL);
		SDL_RenderCopy(wolf->renderer, wolf->screen, NULL, NULL);
		render_debug(wolf);
		SDL_RenderPresent(wolf->renderer);
		hook_events(wolf);
	}
}
