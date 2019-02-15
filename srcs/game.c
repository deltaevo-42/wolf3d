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

static void		update_fps(t_wolf *wolf)
{
	static double		t1 = 0;
	static double		t2 = 0;
	static double		t3 = 0;

	t2 = t1;
	t1 = SDL_GetTicks();
	wolf->stats.delta = (t1 - t2) / 1000.;
	if (t3 + 100 < t1)
	{
		t3 = t1;
		wolf->stats.fps = 1 / wolf->stats.delta;
		wolf->stats.avg_ms = 1000 / (wolf->stats.fps == 0 ? 0.001 : wolf->stats.fps);
	}
}

void			game_loop(t_wolf *wolf)
{
	wolf->stats.fps = 0;
	wolf->stats.delta = 0;
	wolf->player.rotation = 0;
	wolf->player.pos = (t_vec2){ 12, 12 };
	wolf->player.matrix = ft_mat2_rotation(wolf->player.rotation - M_PI_2);
	wolf->texture = IMG_Load("assets/textures/walls.png");
	// printf("SDL_Init failed: %s\n", SDL_GetError());
	while (wolf->running)
	{
		
		SDL_SetRenderDrawColor(wolf->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(wolf->renderer);
		SDL_SetRenderTarget(wolf->renderer, wolf->screen);
		render_main(wolf);
		update_fps(wolf);
		SDL_UpdateTexture(wolf->screen, NULL, wolf->pixels, S_WIDTH * 4);
		SDL_SetRenderTarget(wolf->renderer, NULL);
		SDL_RenderCopy(wolf->renderer, wolf->screen, NULL, NULL);
		render_debug(wolf);
		SDL_RenderPresent(wolf->renderer);
		hook_events(wolf);
	}
}
