/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 12:04:24 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/19 19:52:16 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

#define PLANE ((S_WIDTH / 2) / S_WIDTH)

static void		compute_dir(t_wolf *wolf, t_ray *ray, t_pixel *step)
{
	if (ray->dir.x < 0)
	{
		step->x = -1;
		ray->side_dist.x = (wolf->player.pos.x - (float)ray->hit_pos.x) * ray->delta_dist.x;
	}
	else
	{
		step->x = 1;
		ray->side_dist.x = ((float)ray->hit_pos.x + 1.0 - wolf->player.pos.x) * ray->delta_dist.x;
	}
	if (ray->dir.y < 0)
	{
		step->y = -1;
		ray->side_dist.y = (wolf->player.pos.y - (float)ray->hit_pos.y) * ray->delta_dist.y;
	}
	else
	{
		step->y = 1;
		ray->side_dist.y = ((float)ray->hit_pos.y + 1.0 - wolf->player.pos.y) * ray->delta_dist.y;
	}
}

static t_block_state	*dda(t_wolf *wolf, t_ray *ray, t_pixel *step)
{
	t_block_state		*hit;

	hit = 0;
	while (!hit)
	{
		if (ray->side_dist.x < ray->side_dist.y)
		{
			ray->side_dist.x += ray->delta_dist.x;
			ray->hit_pos.x += step->x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist.y += ray->delta_dist.y;
			ray->hit_pos.y += step->y;
			ray->side = 1;
		}
		hit = wolf->world.data[ray->hit_pos.y][ray->hit_pos.x];
	}
	return (hit);
}

static void	compute_face(t_ray *ray)
{
	if (ray->side == 0 && ray->dir.x < 0)
		ray->face = F_NORTH;
	if (ray->side == 0 && ray->dir.x > 0)
		ray->face = F_SOUTH;
	if (ray->side == 1 && ray->dir.y > 0)
		ray->face = F_WEST;
	if (ray->side == 1 && ray->dir.y < 0)
		ray->face = F_EAST;
}

t_bool			cast_ray(t_wolf *wolf, t_ray *ray, int x)
{
	t_pixel		step;

	ray->dir = ft_mat2_mulv(wolf->player.matrix, ft_vec2_add(
		(t_vec2){0, 1},
		(t_vec2){ PLANE * (2.0f * (float)x / S_WIDTH - 1.0f) * (S_WIDTH / S_HEIGHT), 0}));
	ray->side_dist = (t_vec2){0, 0};
	ray->delta_dist = (t_vec2){ft_absf(1.0f / ray->dir.x), ft_absf(1.0f / ray->dir.y)};
	ray->hit_pos = (t_pixel){.x = wolf->player.pos.x, .y = wolf->player.pos.y};
	compute_dir(wolf, ray, &step);
	ray->hit = dda(wolf, ray, &step);
	if (ray->side == 0)
		ray->dist = (ray->hit_pos.x - wolf->player.pos.x + (1 - step.x) / 2.0) / ray->dir.x;
	else
		ray->dist = (ray->hit_pos.y - wolf->player.pos.y + (1 - step.y) / 2.0) / ray->dir.y;
	if (ray->dist <= 0)
		ray->dist = 0.00001;
	compute_face(ray);
	return (TRUE);
}
