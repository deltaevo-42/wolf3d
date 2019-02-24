/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 12:04:24 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/21 14:17:12 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

#define PLANE ((S_WIDTH / 2) / S_WIDTH)

static void		compute_dir(t_wolf *wolf, t_ray *ray)
{
	if (ray->dir.x < 0)
		ray->side_dist.x = (wolf->player.pos.x - (float)ray->hit_pos.x) * ray->delta_dist.x;
	else
		ray->side_dist.x = ((float)ray->hit_pos.x + 1.0 - wolf->player.pos.x) * ray->delta_dist.x;
	if (ray->dir.y < 0)
		ray->side_dist.y = (wolf->player.pos.y - (float)ray->hit_pos.y) * ray->delta_dist.y;
	else
		ray->side_dist.y = ((float)ray->hit_pos.y + 1.0 - wolf->player.pos.y) * ray->delta_dist.y;
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

t_ray			create_ray(t_wolf *wolf, int x)
{
	t_ray	ray;

	ray.x = x;
	ray.dir = ft_mat2_mulv(wolf->player.matrix, ft_vec2_add(
		(t_vec2){0, 1},
		(t_vec2){ PLANE * (2.0f * (float)x / S_WIDTH - 1.0f) * (S_WIDTH / S_HEIGHT), 0}));
	ray.step = (t_pixel) { ray.dir.x > 0 ? 1 : -1 , ray.dir.y > 0 ? 1 : -1 };
	ray.side_dist = (t_vec2){0, 0};
	ray.delta_dist = (t_vec2){ft_absf(1.0f / ray.dir.x), ft_absf(1.0f / ray.dir.y)};
	ray.hit_pos = (t_pixel){.x = wolf->player.pos.x, .y = wolf->player.pos.y};
	ray.hit = 0;
	return (ray);
}

void			dist_round_block(t_wolf *wolf, t_ray *ray)
{
	t_block_round *block = (t_block_round *)ray->hit->block;
	double p = ray->hit_pos.x + 0.5;
	double q = ray->hit_pos.y + 0.5;

	double r = block->radius;

	if (ray->hit->block->height == 2)
		return ;
	if (ray->dir.x == 0)
	{
		ray->dist -= 1 - (r * 2);
		return;
	}

	double m = ray->dir.y / ray->dir.x;
	double c = wolf->player.pos.y - wolf->player.pos.x * m; 

	double A = (m * m + 1);
	double B = 2 * (m * c - m * q - p);
	double C = (q * q - r * r + p * p - 2 * c * q + c * c);

	double delta = B * B - 4 * A * C;

	if (delta > 0)
	{
		float s_delta = sqrt(delta);
		float x1 = (-B - s_delta) / (2 * A);
		float y1 = m * x1 + c;

		float dx1 = x1 - wolf->player.pos.x;
		float dy1 = y1 - wolf->player.pos.y;

		float dist1 = dx1 * dx1 + dy1 * dy1;

		float x2 = (-B + s_delta) / (2 * A);
		float y2 = m * x2 + c;

		float dx2 = x2 - wolf->player.pos.x;
		float dy2 = y2 - wolf->player.pos.y;

		float dist2 = dx2 * dx2 + dy2 * dy2;

		block->last_hit_x = (dist1 < dist2 ? x1 : x2) - p;
		block->last_hit_y = (dist1 < dist2 ? y1 : y2) - q;

		ray->dist = sqrt(dist1 < dist2 ? dist1 : dist2);
	} else
		ray->hit = 0;
}

void			compute_dist(t_wolf *wolf, t_ray *ray)
{
	if (ray->hit)
	{
		if (ray->hit->block->type == B_ROUND)
			return (dist_round_block(wolf, ray));
	}
	if (ray->side == 0)
		ray->dist = (ray->hit_pos.x - wolf->player.pos.x + (1 - ray->step.x) / 2.0) / ray->dir.x;
	else
		ray->dist = (ray->hit_pos.y - wolf->player.pos.y + (1 - ray->step.y) / 2.0) / ray->dir.y;
}

t_bool			next_ray(t_wolf *wolf, t_ray *ray)
{
	compute_dir(wolf, ray);
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
	if (ray->hit_pos.x < 0 || ray->hit_pos.x >= wolf->world.size.width || ray->hit_pos.y < 0 || ray->hit_pos.y >= wolf->world.size.height)
		return (FALSE);
	ray->hit = wolf->world.data[ray->hit_pos.y][ray->hit_pos.x];
	compute_face(ray);
	compute_dist(wolf, ray);
	if (ray->dist <= 0.1)
		ray->dist = 0.1;
	return (TRUE);
}


t_bool			prev_ray(t_wolf *wolf, t_ray *ray)
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
	if (ray->hit_pos.x < 0 || ray->hit_pos.x >= wolf->world.size.width || ray->hit_pos.y < 0 || ray->hit_pos.y >= wolf->world.size.height)
		return (FALSE);
	ray->hit = wolf->world.data[ray->hit_pos.y][ray->hit_pos.x];
	compute_face(ray);
	compute_dist(wolf, ray);
	return (TRUE);
}