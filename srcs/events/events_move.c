/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:22:34 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/05 13:40:18 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

#define BOUND 0.25

static t_bool	is_in_block(t_wolf *wolf, float x, float y, float z)
{
	t_block_state	*b_state;

	if (wolf->player.pos.x + x < 0
		|| wolf->player.pos.x + x >= (int)wolf->world.size.x
		|| wolf->player.pos.y + y < 0
		|| wolf->player.pos.y + y >= (int)wolf->world.size.y)
		return (TRUE);
	b_state = wolf->world.data[(int)(wolf->player.pos.y + y)]
			[(int)(wolf->player.pos.x + x)];
	if (b_state && b_state->block->height > z)
		return (TRUE);
	else
		return (FALSE);
}

static t_bool	colide(t_wolf *wolf, float x, float y, float z)
{
	return (
		is_in_block(wolf, x + BOUND, y - BOUND, z)
		|| is_in_block(wolf, x + BOUND, y + BOUND, z)
		|| is_in_block(wolf, x - BOUND, y - BOUND, z)
		|| is_in_block(wolf, x - BOUND, y + BOUND, z));
}

static void		translations(t_wolf *w, const Uint8 *s, t_vec3 *v)
{
	const double	ms = w->stats.delta * 3.;
	t_block_state	*state;

	if (s[SDL_SCANCODE_W] || s[SDL_SCANCODE_S])
	{
		v->y += sinf(w->player.rotation) * (s[SDL_SCANCODE_W] ? 1 : -1) * ms;
		v->x += cosf(w->player.rotation) * (s[SDL_SCANCODE_W] ? 1 : -1) * ms;
	}
	if (s[SDL_SCANCODE_A] || s[SDL_SCANCODE_D])
	{
		v->y += -cosf(w->player.rotation) * (s[SDL_SCANCODE_D] ? 1 : -1) * ms;
		v->x += sinf(w->player.rotation) * (s[SDL_SCANCODE_D] ? 1 : -1) * ms;
	}
	if (s[SDL_SCANCODE_SPACE] || s[SDL_SCANCODE_LSHIFT])
	{
		v->z = (s[SDL_SCANCODE_LSHIFT] ? -1 : 1) * ms;
		state = w->world.data[(int)(w->player.pos.y)][(int)(w->player.pos.x)];
		if (!colide(w, 0, 0, w->player.pos.z - 0.5 + v->z)
			&& (!state || state->block->height < w->player.pos.z + v->z - 0.5))
		{
			w->player.pos.z += v->z;
		}
	}
}

static void		limits(t_wolf *wolf)
{
	if (wolf->player.pos.x < 0)
		wolf->player.pos.x = 0;
	if (wolf->player.pos.y < 0)
		wolf->player.pos.y = 0;
	if (wolf->player.pos.x >= wolf->world.size.x)
		wolf->player.pos.x = wolf->world.size.x;
	if (wolf->player.pos.y >= wolf->world.size.y)
		wolf->player.pos.y = wolf->world.size.y;
	if (wolf->player.pos.z > wolf->world.size.z - 0.25)
		wolf->player.pos.z = wolf->world.size.z - 0.25;
	if (wolf->player.pos.z < -0.5)
		wolf->player.pos.z = -0.5;
}

void			events_move(t_wolf *wolf, const Uint8 *state)
{
	const double	move_speed = wolf->stats.delta * 3.;
	t_vec3			v;

	v = (t_vec3){ 0, 0, 0 };
	if (state[SDL_SCANCODE_J] || state[SDL_SCANCODE_L])
	{
		wolf->player.rotation += 0.3 * (state[SDL_SCANCODE_J] ? 1 : -1)
			* move_speed;
		wolf->player.matrix = ft_mat2_rotation(wolf->player.rotation - M_PI_2);
	}
	translations(wolf, state, &v);
	if (!colide(wolf, 0, v.y, wolf->player.pos.z - 0.5))
		wolf->player.pos.y += v.y;
	if (!colide(wolf, v.x, 0, wolf->player.pos.z - 0.5))
		wolf->player.pos.x += v.x;
	limits(wolf);
}
