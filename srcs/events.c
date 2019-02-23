/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 19:48:57 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/21 00:23:40 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void		hook_events(t_wolf *wolf)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	double move_speed = wolf->stats.delta * 3.;
	SDL_Event event;

	event = wolf->event;
	if (event.type == SDL_QUIT || state[SDL_SCANCODE_ESCAPE])
		wolf->running = FALSE;
	if (state[SDL_SCANCODE_J] || state[SDL_SCANCODE_L])
	{
		wolf->player.rotation += 0.3 * (state[SDL_SCANCODE_J] ? 1 : -1) * move_speed;
		wolf->player.matrix = ft_mat2_rotation(wolf->player.rotation - M_PI_2);
	}
	if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_S])
	{
		wolf->player.pos.y += sinf(wolf->player.rotation) * (state[SDL_SCANCODE_W] ? 1 : -1) * move_speed;
		wolf->player.pos.x += cosf(wolf->player.rotation) * (state[SDL_SCANCODE_W] ? 1 : -1) * move_speed;
	}
	if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D])
	{
		wolf->player.pos.y += -cosf(wolf->player.rotation) * (state[SDL_SCANCODE_D] ? 1 : -1)  * move_speed;
		wolf->player.pos.x += sinf(wolf->player.rotation) * (state[SDL_SCANCODE_D] ?  1 : -1)  * move_speed;
	}
	if (state[SDL_SCANCODE_SPACE] || state[SDL_SCANCODE_LSHIFT])
	{
		wolf->player.pos.z += (state[SDL_SCANCODE_LSHIFT] ? -1 : 1) * move_speed;
		if (wolf->player.pos.z > wolf->world.height - 0.5)
			wolf->player.pos.z = wolf->world.height - 0.5;
		if (wolf->player.pos.z < 0)
			wolf->player.pos.z = 0;
	}
	if (state[SDL_SCANCODE_M])
	{
		wolf->minimap_size = S_WIDTH * 0.8;
		wolf->minimap_padding = S_WIDTH / 2 - wolf->minimap_size / 2;
	}
	else
	{
		wolf->minimap_size = 100;
		wolf->minimap_padding = 10;
	}
	SDL_PollEvent(&wolf->event);
}
