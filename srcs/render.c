/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 19:54:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/04 13:42:00 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void			render_debug(t_wolf *wolf)
{
	const SDL_Color	color = {255, 255, 255, 0};
	SDL_Surface		*text;

	text = TTF_RenderText_Blended(wolf->fonts.helvetica,
		ft_int_to_str(wolf->stats.fps).str, color);
	apply_surface_blended(wolf->img, text, (SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){5, 5, text->w + 5, text->h + 5});
	SDL_FreeSurface(text);
	text = TTF_RenderText_Blended(wolf->fonts.helvetica,
		ft_int_to_str((int)wolf->stats.avg_ms).str, color);
	apply_surface_blended(wolf->img, text, (SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){5, text->h + 10, text->w + 5, text->h + 5});
	SDL_FreeSurface(text);
	text = TTF_RenderText_Blended(wolf->fonts.helvetica,
		ft_int_to_str(wolf->stats.num_rays).str, color);
	apply_surface_blended(wolf->img, text, (SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){5, text->h * 2 + 15, text->w + 5, text->h + 5});
	SDL_FreeSurface(text);
}

t_bool			render_wall(t_wolf *wolf, t_ray *from, t_ray *to, int last_y)
{
	if (from->hit->block->type == B_NORMAL)
		return (render_block_normal_wall(wolf, from, to, last_y));
	if (from->hit->block->type == B_ROUND)
		return (render_block_round_wall(wolf, from));
	return (TRUE);
}

t_bool			render_top(t_wolf *wolf, t_ray *ray, t_block_state *hit, int p)
{
	if (hit->block->type == B_NORMAL)
		return (render_block_normal_top(wolf, ray, hit, p));
	if (hit->block->type == B_ROUND)
		return (render_block_round_top(wolf, ray, hit, p));
	return (TRUE);
}

void			try_portal(t_wolf *wolf, t_ray *from, t_ray *to, int last_y)
{
	if (from->hit == to->hit && from->hit && from->hit->type == B_PORTAL)
	{
		render_floor(wolf, from, to, last_y);
		render_ceil(wolf, from, to);
		ray_use_portal(from);
		if (from != to)
			ray_use_portal(to);
	}
}

void			render_main(t_wolf *wolf)
{
	wolf->stats.num_rays = 0;
	render_binary(wolf, 0, S_WIDTH - 1);
}
