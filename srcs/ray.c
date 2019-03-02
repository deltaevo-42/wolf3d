/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 12:04:24 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/02 17:44:14 by llelievr         ###   ########.fr       */
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

static void		compute_face(t_ray *ray)
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

static void		solve_round_polynom(t_ray *r, t_polynom polynom,
		t_circle circle, t_line line)
{
	double	delta;
	float	dist1;
	float	dist2;
	t_vec2	p1;
	t_vec2	p2;

	delta = polynom.b * polynom.b - 4 * polynom.a * polynom.c;
	if (delta < 0)
		return ((void)(r->hit = NULL));
	delta = sqrt(delta);
	p1.x = (-polynom.b - delta) / (2 * polynom.a);
	p1.y = line.m * p1.x + line.c;
	p2.x = (-polynom.b + delta) / (2 * polynom.a);
	p2.y = line.m * p2.x + line.c;
	dist1 = r->side == 0 ? r->extra_dist + (p1.x - r->start.x) / r->dir.x
						: r->extra_dist + (p1.y - r->start.y) / r->dir.y;
	dist2 = r->side == 0 ? r->extra_dist + (p2.x - r->start.x) / r->dir.x
						: r->extra_dist + (p2.y - r->start.y) / r->dir.y;
	r->circle_last_hit_x = (dist1 < dist2 ? p1.x : p2.x) - circle.p;
	r->circle_last_hit_y = (dist1 < dist2 ? p1.y : p2.y) - circle.q;
	r->circle_last_out_dist = dist1 < dist2 ? dist2 : dist1;
	r->dist = dist1 < dist2 ? dist1 : dist2;
}

static void		dist_round_block(t_ray *ray)
{
	t_circle			circle;
	t_line				line;
	t_polynom			polynom;

	circle = (t_circle) {
		.p = ray->hit_pos.x + 0.5, .q = ray->hit_pos.y + 0.5,
		.r = ((t_block_round *)ray->hit->block)->radius
	};
	if (ray->dir.x == 0)
	{
		ray->dist -= 1 - (circle.r * 2);
		return ;
	}
	line = (t_line) {
		.m = ray->dir.y / ray->dir.x,
		.c = ray->start.y - ray->start.x * (ray->dir.y) / ray->dir.x
	};
	polynom = (t_polynom) {
		.a = (line.m * line.m + 1),
		.b = 2 * (line.m * line.c - line.m * circle.q - circle.p),
		.c = (circle.q * circle.q - circle.r * circle.r + circle.p * circle.p
			- 2 * line.c * circle.q + line.c * line.c)
	};
	solve_round_polynom(ray, polynom, circle, line);
}

void			ray_use_portal(t_ray *ray)
{
	double wall_x;
	double unused;

	if (ray->extra_dist > 100)
		return ;
	ray->extra_dist += ray->dist;
	ray->hit_pos = ray->hit->portal_to;
	if (ray->side == 0)
		wall_x = ray->start.y + ray->sdist * ray->dir.y;
	else
		wall_x = ray->start.x + ray->sdist * ray->dir.x;
	wall_x = modf(wall_x, &unused);
	ray->start = (t_vec2) { ray->hit_pos.x + (ray->side == 1 ? wall_x : 0),
							ray->hit_pos.y + (ray->side == 0 ? wall_x : 0) };
	ray->hit = 0;
}

static void		compute_dist(t_ray *ray)
{
	if (ray->side == 0)
		ray->dist = ray->extra_dist + (ray->hit_pos.x - ray->start.x
						+ (1 - ray->step.x) / 2.0) / ray->dir.x;
	else
		ray->dist = ray->extra_dist + (ray->hit_pos.y - ray->start.y
						+ (1 - ray->step.y) / 2.0) / ray->dir.y;
	if (ray->dist <= 0.1)
		ray->dist = 0.1;
	ray->sdist = ray->dist;
	if (ray->hit && ray->hit->block->type == B_ROUND)
		dist_round_block(ray);
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
	ray.hit = ray.world->data[ray.hit_pos.y][ray.hit_pos.x];
	ray.fhit = ray.hit;
	ray.side = ray.side_dist.x < ray.side_dist.y;
	ray.extra_dist = 0;
	compute_face(&ray);
	compute_dist(&ray);
	return (ray);
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
	ray->fhit = NULL;
	if (ray->hit_pos.x < 0 || ray->hit_pos.x >= ray->world->size.x
		|| ray->hit_pos.y < 0 || ray->hit_pos.y >= ray->world->size.y)
		return (FALSE);
	ray->hit = ray->world->data[ray->hit_pos.y][ray->hit_pos.x];
	ray->fhit = ray->hit;
	compute_face(ray);
	compute_dist(ray);
	return (TRUE);
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
	ray->fhit = NULL;
	if (ray->hit_pos.x < 0 || ray->hit_pos.x >= ray->world->size.x
		|| ray->hit_pos.y < 0 || ray->hit_pos.y >= ray->world->size.y)
		return (FALSE);
	ray->hit = ray->world->data[ray->hit_pos.y][ray->hit_pos.x];
	ray->fhit = ray->hit;
	compute_face(ray);
	compute_dist(ray);
	return (TRUE);
}
