/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_binary.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/04 13:32:20 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/03/04 13:41:49 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include <limits.h>

static int		get_hit_bottom(t_wolf *wolf, t_ray *ray)
{
	const float	h = S_HEIGHT / ray->dist;

	return (S_HEIGHT_2 + h * (wolf->player.pos.z + 1) * 0.5
						- h * ray->hit->block->height);
}

static void		cast_ray(t_wolf *wolf, t_ray *ray, int last_y)
{
	int				p;
	t_block_state	*hit;

	while (1)
	{
		try_portal(wolf, ray, ray, last_y);
		if ((hit = ray->hit))
		{
			p = get_hit_bottom(wolf, ray);
			render_wall(wolf, ray, ray, last_y);
			if (p < last_y)
				render_floor(wolf, ray, ray, last_y);
			last_y = p < last_y ? p : last_y;
			if (ray->hit->block->height == wolf->world.size.z
				|| (p <= 0 && p + (S_HEIGHT / ray->dist) > S_HEIGHT))
				break ;
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

static t_bool	render_double_ray(t_wolf *wolf, t_ray *from,
	t_ray *to, int *last_y)
{
	t_block_state *const	hit = from->hit;
	const int				pf = get_hit_bottom(wolf, from);
	const int				pt = get_hit_bottom(wolf, to);
	const int				p = pf > pt ? pf : pt;
	const float				h = S_HEIGHT / fmin(from->dist, to->dist);

	render_wall(wolf, from, to, *last_y);
	if (p < *last_y)
		render_floor(wolf, from, to, *last_y);
	if (p < *last_y)
		*last_y = p;
	if (from->hit->block->height == wolf->world.size.z)
		return (TRUE);
	if (p <= 0 && p + h > S_HEIGHT)
		return (TRUE);
	next_ray(from);
	next_ray(to);
	render_top(wolf, from, hit, pf);
	prev_ray(from);
	prev_ray(to);
	return (FALSE);
}

static t_bool	double_cast_ray(t_wolf *wolf, int x1, int x2, int last_y)
{
	t_ray			first;
	t_ray			second;
	const t_vec2	start = (t_vec2) { wolf->player.pos.x, wolf->player.pos.y };

	first = create_ray(wolf, x1, start);
	second = create_ray(wolf, x2, start);
	while (1)
	{
		if ((next_ray(&first) ^ next_ray(&second))
			|| (first.fhit && first.fhit->block->type != B_NORMAL)
			|| (second.fhit && second.fhit->block->type != B_NORMAL)
			|| first.hit != second.hit || first.face != second.face)
		{
			cast_ray(wolf, &first, last_y);
			cast_ray(wolf, &second, last_y);
			return (FALSE);
		}
		try_portal(wolf, &first, &second, last_y);
		if (first.hit && render_double_ray(wolf, &first, &second, &last_y))
			break ;
	}
	wolf->last_rays[wolf->stats.num_rays++] = first;
	wolf->last_rays[wolf->stats.num_rays++] = second;
	render_ceil(wolf, &first, &second);
	return (TRUE);
}

void			render_binary(t_wolf *wolf, int x1, int x2)
{
	t_ray	ray;
	int		mid;

	if (x2 < x1)
		return ;
	if (x1 == x2)
	{
		ray = create_ray(wolf, x1,
			(t_vec2) { wolf->player.pos.x, wolf->player.pos.y });
		cast_ray(wolf, &ray, INT_MAX);
	}
	else
	{
		if (double_cast_ray(wolf, x1, x2, INT_MAX))
			return ;
		if (x2 - x1 < 2)
			return ;
		mid = (x1 + x2) / 2;
		render_binary(wolf, x1 + 1, mid - 1);
		render_binary(wolf, mid, x2 - 1);
	}
}