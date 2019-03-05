/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 12:04:24 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/05 14:05:49 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

static void		compute_dir(t_ray *ray)
{
	if (ray->dir.x < 0)
		ray->side_dist.x = (ray->start.x - (float)ray->hit_pos.x)
			* ray->delta_dist.x;
	else
		ray->side_dist.x = ((float)ray->hit_pos.x + 1.0 - ray->start.x)
			* ray->delta_dist.x;
	if (ray->dir.y < 0)
		ray->side_dist.y = (ray->start.y - (float)ray->hit_pos.y)
			* ray->delta_dist.y;
	else
		ray->side_dist.y = ((float)ray->hit_pos.y + 1.0 - ray->start.y)
			* ray->delta_dist.y;
}

t_ray			create_ray(t_wolf *wolf, int x, t_vec2 start)
{
	t_ray	ray;

	ray.x = x;
	ray.start = start;
	ray.world = &wolf->world;
	ray.dir = ft_mat2_mulv(wolf->player.matrix, ft_vec2_add(
		(t_vec2){0, wolf->dist_to_plane },
		(t_vec2){ PLANE * (2.0f * (float)x / S_WIDTH - 1.0f) * S_RATIO, 0}));
	ray.step = (t_pixel) { ray.dir.x > 0 ? 1 : -1, ray.dir.y > 0 ? 1 : -1, 0 };
	ray.side_dist = (t_vec2){0, 0};
	ray.delta_dist = (t_vec2){fabs(1.0f / ray.dir.x), fabs(1.0f / ray.dir.y)};
	ray.hit_pos = (t_pixel){.x = start.x, .y = start.y};
	ray.hit = ray_in_map(&ray)
		? ray.world->data[ray.hit_pos.y][ray.hit_pos.x] : NULL;
	ray.fhit = ray.hit;
	ray.side = ray.side_dist.x < ray.side_dist.y;
	ray.extra_dist = 0;
	ray_compute_dist(&ray);
	return (ray);
}

t_bool			ray_in_map(t_ray *ray)
{
	return (ray->hit_pos.x >= 0 && ray->hit_pos.x < ray->world->size.x
		&& ray->hit_pos.y >= 0 && ray->hit_pos.y < ray->world->size.y);
}

t_bool			next_ray(t_ray *ray)
{
	compute_dir(ray);
	if (ray->side_dist.x < ray->side_dist.y)
	{
		ray->side_dist.x += ray->delta_dist.x;
		ray->hit_pos.x += ray->step.x;
		ray->side = 0;
	}
	else
	{
		ray->side_dist.y += ray->delta_dist.y;
		ray->hit_pos.y += ray->step.y;
		ray->side = 1;
	}
	ray->hit = NULL;
	if (ray_in_map(ray))
		ray->hit = ray->world->data[ray->hit_pos.y][ray->hit_pos.x];
	ray->fhit = ray->hit;
	ray_compute_dist(ray);
	return (ray_in_map(ray));
}

t_bool			prev_ray(t_ray *ray)
{
	if (ray->side == 0)
	{
		ray->side_dist.x -= ray->delta_dist.x;
		ray->hit_pos.x -= ray->step.x;
		ray->side = 1;
	}
	else
	{
		ray->side_dist.y -= ray->delta_dist.y;
		ray->hit_pos.y -= ray->step.y;
		ray->side = 0;
	}
	ray->hit = NULL;
	if (ray_in_map(ray))
		ray->hit = ray->world->data[ray->hit_pos.y][ray->hit_pos.x];
	ray->fhit = ray->hit;
	ray_compute_dist(ray);
	return (ray_in_map(ray));
}
