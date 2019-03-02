/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 19:54:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/02 15:04:41 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include <assert.h>
#include <limits.h>

void			render_debug(t_wolf *wolf)
{
	SDL_Surface		*text;

	text = TTF_RenderText_Blended(wolf->fonts.helvetica, ft_int_to_str(wolf->stats.fps).str, (SDL_Color){255, 255, 255, 0});
	apply_surface_blended(wolf->img, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){5, 5, text->w + 5, text->h + 5});
	SDL_FreeSurface(text);
	text = TTF_RenderText_Blended(wolf->fonts.helvetica, ft_int_to_str((int)wolf->stats.avg_ms).str, (SDL_Color){255, 255, 255, 0});
	apply_surface_blended(wolf->img, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){5, text->h + 10, text->w + 5, text->h + 5});
	SDL_FreeSurface(text);
	text = TTF_RenderText_Blended(wolf->fonts.helvetica, ft_int_to_str(wolf->stats.num_rays).str, (SDL_Color){255, 255, 255, 0});
	apply_surface_blended(wolf->img, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){5, text->h * 2 + 15, text->w + 5, text->h + 5});
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

void			cast_ray(t_wolf *wolf, t_ray *ray, int last_y)
{
	while (1)
	{
		if (ray->hit && ray->hit->type == B_PORTAL)
		{
			render_floor(wolf, ray, ray, last_y);
			render_ceil(wolf, ray, ray);
			ray_use_portal(ray);
		}
		if (ray->hit)
		{
			t_block_state *hit = ray->hit;
			float h = S_HEIGHT / ray->dist;
			int p = S_HEIGHT_2 + h * (wolf->player.pos.z + 1) * 0.5 - h * hit->block->height;
			render_wall(wolf, ray, ray, last_y);
			if (p < last_y)
				render_floor(wolf, ray, ray, last_y);
			if (p < last_y)
				last_y = p;
			if (ray->hit->block->height == wolf->world.size.z)
				break ;
			if (p <= 0 && p + h > S_HEIGHT)
				break;
			next_ray(ray);
			render_top(wolf, ray, hit, p);
			continue ;
		}
		if (!next_ray(ray))
			break ;
	}
	wolf->last_rays[wolf->stats.num_rays++] = *ray;
	render_ceil(wolf, ray, ray);
}

t_bool			double_cast_ray(t_wolf *wolf, int x1, int x2)
{
	t_ray	first = create_ray(wolf, x1, (t_vec2) { wolf->player.pos.x, wolf->player.pos.y });
	t_ray	second = create_ray(wolf, x2, (t_vec2) { wolf->player.pos.x, wolf->player.pos.y });
	int		last_y = INT_MAX;

	while (1)
	{
		t_bool	next_first = next_ray(&first);
		t_bool	next_second = next_ray(&second);

		if (!next_first && !next_second)
			break ;
		if (next_first != next_second)
		{
			cast_ray(wolf, &first, last_y);
			cast_ray(wolf, &second, last_y);
			return (FALSE);
		}
		if ((first.fhit && first.fhit->block->type != B_NORMAL)
			|| (second.fhit && second.fhit->block->type != B_NORMAL))
		{
			cast_ray(wolf, &first, last_y);
			cast_ray(wolf, &second, last_y);
			return (FALSE);
		}
		if (first.fhit == second.fhit && first.fhit && first.fhit->type == B_PORTAL)
		{
			render_floor(wolf, &first, &second, last_y);
			render_ceil(wolf, &first, &second);
			ray_use_portal(&first);
			ray_use_portal(&second);
		}
		if (first.hit || second.hit)
		{
			if (first.hit != second.hit || first.face != second.face)
			{
				cast_ray(wolf, &first, last_y);
				cast_ray(wolf, &second, last_y);
				return (FALSE);
			}
			t_block_state *hit = first.hit;
			float hf = S_HEIGHT / first.dist;
			int pf = S_HEIGHT_2 + hf * (wolf->player.pos.z + 1) * 0.5 - hf * hit->block->height;
			float hs = S_HEIGHT / second.dist;
			int ps = S_HEIGHT_2 + hs * (wolf->player.pos.z + 1) * 0.5 - hs * hit->block->height;
			float h = pf > ps ? hf : hs;
			int p = pf > ps ? pf : ps;
			render_wall(wolf, &first, &second, last_y);
			if (p < last_y)
				render_floor(wolf, &first, &second, last_y);
			if (p < last_y)
				last_y = p;
			if (first.hit->block->height == wolf->world.size.z)
				break ;
			if (p <= 0 && p + h > S_HEIGHT)
				break;
			next_ray(&first);
			next_ray(&second);
			render_top(wolf, &first, hit, pf);
			prev_ray(&first);
			prev_ray(&second);
		}
	}
	render_ceil(wolf, &first, &second);
	wolf->last_rays[wolf->stats.num_rays++] = first;
	wolf->last_rays[wolf->stats.num_rays++] = second;
	return (TRUE);
}

void			render_binary(t_wolf *wolf, int x1, int x2)
{
	if (x2 < x1)
		return ;
	if (x1 == x2)
	{
		t_ray ray = create_ray(wolf, x1, (t_vec2) { wolf->player.pos.x, wolf->player.pos.y });
		cast_ray(wolf, &ray, INT_MAX);
	} else
	{
		if (double_cast_ray(wolf, x1, x2))
			return ;
		if (x2 - x1 < 2)
			return ;
		int mid = (x1 + x2) / 2;
		render_binary(wolf, x1 + 1, mid - 1);
		render_binary(wolf, mid, x2 - 1);
	}
}	

void			render_main(t_wolf *wolf)
{
	wolf->stats.num_rays = 0;
	render_binary(wolf, 0, S_WIDTH - 1);
}