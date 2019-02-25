/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 19:48:57 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/25 00:22:09 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

#define STAIR_MAX 0.5

void		hook_events(t_wolf *wolf)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	double move_speed = wolf->stats.delta * 3.;
	SDL_Event event;
	float	x;
	float	y;
	t_block_state	*b_state;

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
		y = sinf(wolf->player.rotation) * (state[SDL_SCANCODE_W] ? 1 : -1) * move_speed;
		x = cosf(wolf->player.rotation) * (state[SDL_SCANCODE_W] ? 1 : -1) * move_speed;

		b_state = wolf->world.data[(int)(wolf->player.pos.y + y)][(int)(wolf->player.pos.x + x)];

		if (!b_state || (b_state->block->height <= wolf->player.pos.z + STAIR_MAX && b_state->block->height < wolf->world.size.z - 0.5))
		{
			wolf->player.pos.y += y;
			wolf->player.pos.x += x;
			if (b_state && wolf->player.pos.z < b_state->block->height + 0.5)
				wolf->player.pos.z = b_state->block->height + 0.5;
		}
	}
	if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D])
	{
		y += -cosf(wolf->player.rotation) * (state[SDL_SCANCODE_D] ? 1 : -1)  * move_speed;
		x += sinf(wolf->player.rotation) * (state[SDL_SCANCODE_D] ?  1 : -1)  * move_speed;

		b_state = wolf->world.data[(int)(wolf->player.pos.y + y)][(int)(wolf->player.pos.x + x)];

		if (!b_state || (b_state->block->height <= wolf->player.pos.z + STAIR_MAX && b_state->block->height < wolf->world.size.z - 0.5))
		{
			wolf->player.pos.y += y;
			wolf->player.pos.x += x;
			if (b_state && wolf->player.pos.z < b_state->block->height + 0.5)
				wolf->player.pos.z = b_state->block->height + 0.5;
		}
	}
	if (state[SDL_SCANCODE_SPACE] || state[SDL_SCANCODE_LSHIFT])
	{
		float z = (state[SDL_SCANCODE_LSHIFT] ? -1 : 1) * move_speed;

		b_state = wolf->world.data[(int)(wolf->player.pos.y)][(int)(wolf->player.pos.x)];

		if (!b_state || b_state->block->height < wolf->player.pos.z - 0.5)
		{
			wolf->player.pos.z += z;
		}
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
	/*if (wolf->player.pos.x < 0)
		wolf->player.pos.x = 0;
	if (wolf->player.pos.y < 0)
		wolf->player.pos.y = 0;*/
	if (wolf->player.pos.z > wolf->world.size.z + 0.5)
		wolf->player.pos.z = wolf->world.size.z + 0.5;
	if (wolf->player.pos.z < -0.5)
		wolf->player.pos.z = -0.5;
	SDL_PollEvent(&wolf->event);
}
