/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 17:51:33 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/04 14:31:33 by llelievr         ###   ########.fr       */
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
		wolf->stats.avg_ms = 1000
			/ (wolf->stats.fps == 0 ? 0.001 : wolf->stats.fps);
	}
}

void			update_texture(t_texture_animated *t)
{
	if (SDL_GetTicks() - t->last_seen > t->delay)
	{
		t->last_seen = SDL_GetTicks();
		t->index++;
		if (t->index == t->step_count.x * t->step_count.y)
			t->index = 0;
	}
}

void			update_textures(t_wolf *wolf)
{
	int		i;

	i = 0;
	while (i < wolf->textures_count)
	{
		if (wolf->textures[i]->type == T_ANIMATED)
			update_texture((t_texture_animated *)wolf->textures[i]);
		i++;
	}
	update_texture(&wolf->heads_texture);
}

#define WEAPONS "assets/textures/weapons.png"
#define HEADS "assets/textures/heads.png"

t_bool			init_defaults(t_wolf *wolf)
{
	SDL_Surface	*img;

	wolf->player.pos = wolf->world.spawn_pos;
	wolf->player.rotation = wolf->world.spawn_rotation * DEG_TO_RAD;
	wolf->player.matrix = ft_mat2_rotation(wolf->player.rotation - M_PI_2);
	if (!(wolf->head_overlay = IMG_Load("assets/textures/head_overlay.png"))
		|| !(img = IMG_Load("assets/textures/crosshair.png"))
		|| !setup_animated_texture(&wolf->weapons_texture, WEAPONS)
		|| !setup_animated_texture(&wolf->heads_texture, HEADS))
		return (FALSE);
	wolf->crosshair = SDL_ConvertSurfaceFormat(img,
		SDL_PIXELFORMAT_ARGB8888, 0);
	SDL_FreeSurface(img);
	wolf->weapons_texture.index = wolf->player.selected_weapon
		* wolf->weapons_texture.step_count.x;
	return (TRUE);
}

void			game_loop(t_wolf *wolf)
{
	while (wolf->running)
	{
		ft_memset(wolf->img->pixels, 0, wolf->img->size * 4);
		render_main(wolf);
		render_debug(wolf);
		render_hud(wolf);
		render_minimap(wolf);
		SDL_UpdateTexture(wolf->screen, NULL, wolf->img->pixels,
				wolf->img->width * 4);
		SDL_RenderCopy(wolf->renderer, wolf->screen, NULL, NULL);
		SDL_RenderPresent(wolf->renderer);
		hook_events(wolf);
		update_fps(wolf);
		update_textures(wolf);
	}
}
