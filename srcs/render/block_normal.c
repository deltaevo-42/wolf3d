/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_normal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 13:33:02 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/05 13:30:58 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "block_normal.h"

static t_render_info	get_render_infos(t_wolf *wolf, t_ray *ray,
	t_block_normal *block, int last_y)
{
	t_texture			*texture;
	t_render_info		infos;
	double				wall_x;
	double				garbage;

	block = (t_block_normal *)ray->hit->block;
	texture = block->faces[ray->face].texture;
	infos.height = S_HEIGHT / ray->dist;
	infos.block = block;
	infos.last_y = last_y;
	infos.face = ray->face;
	infos.dist = ray->dist;
	if (ray->side == 0)
		wall_x = ray->start.y + (ray->dist - ray->extra_dist) * ray->dir.y;
	else
		wall_x = ray->start.x + (ray->dist - ray->extra_dist) * ray->dir.x;
	wall_x = modf(wall_x, &garbage);
	infos.tex_x = wall_x * texture->size.x;
	infos.y = S_HEIGHT_2 + infos.height * (wolf->player.pos.z + 1) /
		2. - infos.height * block->super.height;
	return (infos);
}

static t_bool			render_face(t_wolf *wolf, t_render_info from,
	t_render_info to, float a)
{
	const int		y = from.y + a * (to.y - from.y);
	const double	height = from.height + a * (to.height - from.height);
	t_texture		*texture;
	float			ratio;
	int				tex_x;

	tex_x = ((1 - a) * from.tex_x / from.dist + a * to.tex_x / to.dist)
				/ ((1 - a) * 1 / from.dist + a * 1 / to.dist);
	texture = ((t_block_normal *)to.block)->faces[from.face].texture;
	if (y >= to.last_y)
		return (FALSE);
	ratio = y < 0 ? 1 : (to.last_y - y) / (height * to.block->super.height);
	if (ratio > 1)
		ratio = 1;
	apply_texture(wolf->img, texture,
		(SDL_Rect){ tex_x, 0,
			1, texture->size.y - texture->size.y * (1 - ratio) },
		(SDL_Rect){ to.x, y, 1, y < 0 ? height * to.block->super.height
			: fmin(height * to.block->super.height, to.last_y - y) });
	return (TRUE);
}

t_bool					render_block_normal_wall(t_wolf *wolf,
	t_ray *from_ray, t_ray *to_ray, int last_y)
{
	t_block_normal	*block;
	t_render_info	from;
	t_render_info	to;
	double			progress;

	block = (t_block_normal *)from_ray->hit->block;
	from = get_render_infos(wolf, from_ray, block, last_y);
	to = get_render_infos(wolf, to_ray, block, last_y);
	to.x = from_ray->x - 1;
	if (to.y >= last_y && from.y >= last_y)
		return (FALSE);
	while (++to.x <= to_ray->x)
	{
		progress = (float)(to.x - from_ray->x) /
			(float)(to_ray->x == from_ray->x ? 1 : to_ray->x - from_ray->x);
		if (!render_face(wolf, from, to, progress))
			continue;
	}
	return (FALSE);
}

t_bool					render_block_normal_top(t_wolf *wolf, t_ray *ray[2],
	t_block_state *hit, int p[2])
{
	const int	x_delta = ray[0]->x == ray[1]->x ? 1 : ray[1]->x - ray[0]->x;
	int			pb[2];
	int			x;
	int			y;
	int			pt;

	pb[0] = S_HEIGHT_2 + (S_HEIGHT / ray[0]->dist) * (wolf->player.pos.z + 1)
		/ 2. - (S_HEIGHT / ray[0]->dist) * hit->block->height;
	pb[1] = S_HEIGHT_2 + (S_HEIGHT / ray[1]->dist) * (wolf->player.pos.z + 1)
		/ 2. - (S_HEIGHT / ray[1]->dist) * hit->block->height;
	x = ray[0]->x;
	while (x <= ray[1]->x)
	{
		y = pb[0] + (pb[1] - pb[0]) * (x - ray[0]->x) / x_delta;
		pt = p[0] + (p[1] - p[0]) * (x - ray[0]->x) / x_delta;
		while (y < pt
			&& (uint32_t)(y * (int)S_WIDTH + x) < wolf->img->size)
		{
			if (wolf->img->pixels[y * (int)S_WIDTH + x] == 0)
				wolf->img->pixels[y * (int)S_WIDTH + x] = 0xFF0000FF;
			y++;
		}
		x++;
	}
	return (FALSE);
}
