/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 19:48:57 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/14 17:27:05 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void		hook_events(t_wolf *wolf)
{
	SDL_Event event;

	event = wolf->event;
	if (event.type == SDL_QUIT || (SDL_KEYDOWN == event.type && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode))
		wolf->running = FALSE;
	if (event.type == SDL_KEYDOWN)
	{
		if (SDL_SCANCODE_J == event.key.keysym.scancode || SDL_SCANCODE_L == event.key.keysym.scancode)
			wolf->player.rotation += 0.05 * (SDL_SCANCODE_J == event.key.keysym.scancode ? 1 : -1);
		if (SDL_SCANCODE_W == event.key.keysym.scancode || SDL_SCANCODE_S == event.key.keysym.scancode)
		{
			wolf->player.pos.y += sinf(wolf->player.rotation) * (SDL_SCANCODE_W ==  event.key.keysym.scancode ? 1 : -1) * 0.5;
			wolf->player.pos.x += cosf(wolf->player.rotation) * (SDL_SCANCODE_W == event.key.keysym.scancode ?  1 : -1) * 0.5;
		}
		if (SDL_SCANCODE_A == event.key.keysym.scancode || SDL_SCANCODE_D == event.key.keysym.scancode)
		{
			wolf->player.pos.y += -cosf(wolf->player.rotation) * (SDL_SCANCODE_D ==  event.key.keysym.scancode ? 1 : -1) * 0.5;
			wolf->player.pos.x += sinf(wolf->player.rotation) * (SDL_SCANCODE_D == event.key.keysym.scancode ?  1 : -1) * 0.5;
		}
	}
	SDL_PollEvent(&wolf->event);
}
