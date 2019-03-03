/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/02 14:37:28 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/03/03 15:54:47 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "floor.h"

static t_vec2	get_floor_wall(t_ray *ray)
{
	float	wall_x;
	t_vec2	floor_wall;

	if (ray->side == 0)
		wall_x = ray->start.y + (ray->sdist - ray->extra_dist) * ray->dir.y;
	else
		wall_x = ray->start.x + (ray->sdist - ray->extra_dist) * ray->dir.x;
	wall_x -= floor((wall_x));
	if (ray->side == 0 && ray->dir.x > 0)
		floor_wall = (t_vec2) {ray->hit_pos.x, ray->hit_pos.y + wall_x};
	else if (ray->side == 0 && ray->dir.x < 0)
		floor_wall = (t_vec2) {ray->hit_pos.x + 1.0, ray->hit_pos.y + wall_x};
	else if (ray->side == 1 && ray->dir.y > 0)
		floor_wall = (t_vec2) {ray->hit_pos.x + wall_x, ray->hit_pos.y};
	else
		floor_wall = (t_vec2) {ray->hit_pos.x + wall_x, ray->hit_pos.y + 1.0};
	return (floor_wall);
}

#define FLOOR 0
#define TEX 1

static void		render_floor_x(t_wall_info from, t_wall_info to,
		SDL_Surface *tex, t_img *img)
{
	const int	x_d = to.r->x == from.r->x ? 1 : to.r->x - from.r->x;
	t_vec2		from_p[2];
	t_vec2		to_p[2];
	t_vec3		inc;
	t_pixel		i;

	from_p[FLOOR] = (t_vec2) {
		from.weight * from.floor_wall.x + (1. - from.weight) * from.r->start.x,
		from.weight * from.floor_wall.y + (1. - from.weight) * from.r->start.y};
	from_p[TEX] = (t_vec2) {from_p[FLOOR].x * tex->w, from_p[FLOOR].y * tex->h};
	to_p[FLOOR] = (t_vec2) {
		to.weight * to.floor_wall.x + (1. - to.weight) * to.r->start.x,
		to.weight * to.floor_wall.y + (1. - to.weight) * to.r->start.y};
	to_p[TEX] = (t_vec2) {to_p[FLOOR].x * tex->w, to_p[FLOOR].y * tex->h};
	inc = (t_vec3) {(to_p[TEX].x - from_p[TEX].x) / x_d,
		(to_p[TEX].y - from_p[TEX].y) / x_d, (float)(to.y - from.y) / x_d};
	i.x = from.r->x - 1;
	while (++i.x <= to.r->x)
		if ((i.y = from.y + inc.z * (i.x - from.r->x)) > 0
			&& i.y < (int)S_HEIGHT
			&& img->pixels[i.y * (int)S_WIDTH + i.x] == 0)
			img->pixels[i.y * (int)S_WIDTH + i.x] = getpixel(tex,
				(int)fabs(from_p[TEX].x + inc.x * (i.x - from.r->x)) % tex->w,
				(int)fabs(from_p[TEX].y + inc.y * (i.x - from.r->x)) % tex->h);
}

static int		get_wall_screen_pos(t_wolf *wolf, t_ray *ray,
		enum e_wall_pos pos)
{
	const float	height = S_HEIGHT / ray->dist;
	const int	world_height = height * wolf->world.size.z;
	const int	offset = height * (wolf->player.pos.z + 1) / 2. - height;

	if (pos == WALL_BOTTOM)
		return (S_HEIGHT_2 + world_height / 2 + offset + 1);
	else if (pos == WALL_TOP)
		return (S_HEIGHT_2 - world_height / 2 + offset - 1);
	else
		return (S_HEIGHT_2);
}

void			render_floor(t_wolf *wolf, t_ray *from_ray, t_ray *to_ray,
		int last_y)
{
	t_wall_info from;
	t_wall_info to;
	float		from_dist_z;
	float		to_dist_z;

	from = (t_wall_info) {
		.r = from_ray, .floor_wall = get_floor_wall(from_ray),
		.y = get_wall_screen_pos(wolf, from_ray, WALL_BOTTOM)
	};
	to = (t_wall_info) {
		.r = to_ray, .floor_wall = get_floor_wall(to_ray),
		.y = get_wall_screen_pos(wolf, to_ray, WALL_BOTTOM)
	};
	from_dist_z = (from_ray->sdist - from_ray->extra_dist)
				/ (wolf->player.pos.z + 1);
	to_dist_z = (to_ray->sdist - to_ray->extra_dist) / (wolf->player.pos.z + 1);
	last_y = last_y > S_HEIGHT ? S_HEIGHT : last_y;
	while (from.y < last_y || to.y < last_y)
	{
		from.weight = S_HEIGHT / ((2 * from.y - S_HEIGHT) * from_dist_z);
		to.weight = S_HEIGHT / ((2 * to.y - S_HEIGHT) * to_dist_z);
		render_floor_x(from, to, wolf->world.floor->surface, wolf->img);
		from.y++;
		to.y++;
	}
}

void			render_ceil(t_wolf *wolf, t_ray *from_ray, t_ray *to_ray)
{
	t_wall_info from;
	t_wall_info to;
	float		from_dist_z;
	float		to_dist_z;

	from = (t_wall_info) {
		.r = from_ray, .floor_wall = get_floor_wall(from_ray),
		.y = get_wall_screen_pos(wolf, from_ray, WALL_TOP)
	};
	to = (t_wall_info) {
		.r = to_ray, .floor_wall = get_floor_wall(to_ray),
		.y = get_wall_screen_pos(wolf, to_ray, WALL_TOP)
	};
	from_dist_z = (from_ray->sdist - from_ray->extra_dist)
			/ -(wolf->world.size.z - wolf->player.pos.z + 1);
	to_dist_z = (to_ray->sdist - to_ray->extra_dist)
			/ -(wolf->world.size.z - wolf->player.pos.z + 1);
	while (from.y > 0 || to.y > 0)
	{
		from.weight = S_HEIGHT / ((2 * from.y - S_HEIGHT) * from_dist_z);
		to.weight = S_HEIGHT / ((2 * to.y - S_HEIGHT) * to_dist_z);
		render_floor_x(from, to, wolf->world.ceil->surface, wolf->img);
		from.y--;
		to.y--;
	}
}
