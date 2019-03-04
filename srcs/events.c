/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 19:48:57 by llelievr          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2019/03/04 17:08:17 by dde-jesu         ###   ########.fr       */
=======
/*   Updated: 2019/03/04 18:20:43 by llelievr         ###   ########.fr       */
>>>>>>> Fix floor/ceiling
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

#define STAIR_MAX 0.5
#define BOUND 0.25

static t_bool	is_in_block(t_wolf *wolf, float x, float y, float z)
{
	t_block_state	*b_state;

	b_state = wolf->world.data[(int)(wolf->player.pos.y + y - BOUND)]
			[(int)(wolf->player.pos.x + x + BOUND)];
	if (b_state && b_state->block->height > z)
		return (TRUE);
	b_state = wolf->world.data[(int)(wolf->player.pos.y + y - BOUND)]
			[(int)(wolf->player.pos.x + x - BOUND)];
	if (b_state && b_state->block->height > z)
		return (TRUE);
	b_state = wolf->world.data[(int)(wolf->player.pos.y + y + BOUND)]
			[(int)(wolf->player.pos.x + x - BOUND)];
	if (b_state && b_state->block->height > z)
		return (TRUE);
	b_state = wolf->world.data[(int)(wolf->player.pos.y + y + BOUND)]
			[(int)(wolf->player.pos.x + x + BOUND)];
	if (b_state && b_state->block->height > z)
		return (TRUE);
	return (FALSE);
}

static void	events_move(t_wolf *wolf, const Uint8 *state)
{
	const double	move_speed = wolf->stats.delta * 3.;
	float			x;
	float			y;
	float			z;
	t_block_state	*b_state;

	x = 0;
	y = 0;
	if (state[SDL_SCANCODE_J] || state[SDL_SCANCODE_L])
	{
		wolf->player.rotation += 0.3 * (state[SDL_SCANCODE_J] ? 1 : -1)
			* move_speed;
		wolf->player.matrix = ft_mat2_rotation(wolf->player.rotation - M_PI_2);
	}
	if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_S])
	{
		y += sinf(wolf->player.rotation) * (state[SDL_SCANCODE_W] ? 1 : -1)
			* move_speed;
		x += cosf(wolf->player.rotation) * (state[SDL_SCANCODE_W] ? 1 : -1)
			* move_speed;
	}
	if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D])
	{
		y += -cosf(wolf->player.rotation) * (state[SDL_SCANCODE_D] ? 1 : -1)
			* move_speed;
		x += sinf(wolf->player.rotation) * (state[SDL_SCANCODE_D] ? 1 : -1)
			* move_speed;
	}
	if (state[SDL_SCANCODE_SPACE] || state[SDL_SCANCODE_LSHIFT])
	{
		z = (state[SDL_SCANCODE_LSHIFT] ? -1 : 1) * move_speed;
		b_state = wolf->world.data[(int)(wolf->player.pos.y)]
			[(int)(wolf->player.pos.x)];
		if (!b_state || b_state->block->height < wolf->player.pos.z + z - 0.5)
		{
			wolf->player.pos.z += z;
		}
	}
	if (!is_in_block(wolf, 0, y, wolf->player.pos.z - 0.5))
		wolf->player.pos.y += y;
	if (!is_in_block(wolf, x, 0, wolf->player.pos.z - 0.5))
		wolf->player.pos.x += x;
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

static void	events_minimap(t_wolf *wolf, const Uint8 *state)
{
	if (state[SDL_SCANCODE_M])
	{
		wolf->minimap_size = (S_WIDTH > S_HEIGHT ? S_HEIGHT : S_WIDTH) * 0.8;
		wolf->minimap_padding_x = S_WIDTH / 2 - wolf->minimap_size / 2;
		wolf->minimap_padding_y = S_HEIGHT / 2 - wolf->minimap_size / 2;
	}
	else
	{
		wolf->minimap_size = 100;
		wolf->minimap_padding_x = 10;
		wolf->minimap_padding_y = 10;
	}
}

static void	events_fov(t_wolf *wolf, const Uint8 *state)
{
	const double	move_speed = wolf->stats.delta * .3;

	if (state[SDL_SCANCODE_KP_PLUS])
		wolf->dist_to_plane += move_speed;
	if (state[SDL_SCANCODE_KP_MINUS] && wolf->dist_to_plane > move_speed)
		wolf->dist_to_plane -= move_speed;
}

static void	events_window(t_wolf *wolf, SDL_Event *event)
{
	const SDL_Scancode	key = event->key.keysym.scancode;


	if (event->type == SDL_QUIT)
		wolf->running = FALSE;
	if (event->type == SDL_KEYDOWN)
	{
		if (key == SDL_SCANCODE_F)
		{
			wolf->fullscreen = !wolf->fullscreen;
			SDL_SetWindowFullscreen(wolf->win,
					wolf->fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
		}
		else if (key == SDL_SCANCODE_P || key == SDL_SCANCODE_O)
		{
			SDL_SetRelativeMouseMode(key == SDL_SCANCODE_O);
			SDL_SetWindowGrab(wolf->win, key == SDL_SCANCODE_O);
		}
	}
}

static void	events_weapon(t_wolf *wolf, SDL_Event *event)
{
	if (event->type == SDL_MOUSEWHEEL
	|| (event->type == SDL_KEYDOWN
			&& event->key.keysym.scancode == SDL_SCANCODE_UP))
	{
		wolf->player.selected_weapon++;
		if (wolf->player.selected_weapon == 4)
			wolf->player.selected_weapon = 0;
		wolf->weapons_texture.index = wolf->player.selected_weapon
			* wolf->weapons_texture.step_count.x;
		wolf->player.shooting = FALSE;
	}
}

void		hook_events(t_wolf *wolf)
{
	const Uint8		*state = SDL_GetKeyboardState(NULL);
	const double	move_speed = wolf->stats.delta * 3.;
	SDL_Event		event;
	int				x_m;

	if (state[SDL_SCANCODE_ESCAPE])
		wolf->running = FALSE;
	events_move(wolf, state);
	events_minimap(wolf, state);
	events_fov(wolf, state);
	while (SDL_PollEvent(&event))
	{
		events_window(wolf, &event);
		events_weapon(wolf, &event);
	}
	SDL_GetRelativeMouseState(&x_m, NULL);
	if (x_m != 0)
	{
		wolf->player.rotation -= 0.01 * x_m * move_speed;
		wolf->player.matrix = ft_mat2_rotation(wolf->player.rotation - M_PI_2);
	}
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)
			&& !wolf->player.shooting)
		wolf->player.shooting = TRUE;
	SDL_PumpEvents();
}
