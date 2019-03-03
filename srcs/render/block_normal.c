/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_normal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 13:33:02 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/03 16:07:44 by llelievr         ###   ########.fr       */
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

t_bool	render_face(t_wolf *wolf, t_render_info from, t_render_info to,
	float progress)
{
	const int		y = from.y + progress * (to.y - from.y);
	const double	height = from.height + progress * (to.height - from.height);
	t_texture		*texture;
	float			ratio;

	texture = ((t_block_normal *)to.block)->faces[from.face].texture;
	if (y >= to.last_y)
		return (FALSE);
	ratio = y < 0 ? 1 : (to.last_y - y) / (height * to.block->super.height);
	if (ratio > 1)
		ratio = 1;
	apply_texture(wolf->img, texture,
		(SDL_Rect){ from.tex_x + progress * (float)(to.tex_x - from.tex_x), 0,
			1, texture->size.y - texture->size.y * (1 - ratio) },
		(SDL_Rect){ to.x, y, 1, y < 0 ? height * to.block->super.height
			: fmin(height * to.block->super.height, to.last_y - y) });
	return (TRUE);
}

t_bool	render_block_normal_wall(t_wolf *wolf, t_ray *from_ray, t_ray *to_ray,
	int last_y)
{
	t_block_normal	*block;
	t_render_info	from;
	t_render_info	to;
	float			progress;

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

t_bool	render_block_normal_top(t_wolf *wolf, t_ray *ray, t_block_state *hit, int p1)
{
	float hit_h = hit->block->height;
	float h0 = S_HEIGHT / ray->dist;
	int p0 = S_HEIGHT_2 + h0 * (wolf->player.pos.z + 1) / 2. - h0 * hit_h;

	for (int y = p0; p0 < p1 && y < p1; y++)
	{
		int i = y * S_WIDTH + ray->x;
		if (i < 0 || (uint32_t)i >= wolf->img->size)
			break ;
		if (wolf->img->pixels[i] == 0)
			wolf->img->pixels[i] = 0xFF0000FF;
	}
	return (FALSE);
}
