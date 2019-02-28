/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_normal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 13:33:02 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/26 17:54:10 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

typedef struct s_render_info {
	int		texX;
	int		y;
	float	height;
}				t_render_info;


t_render_info	get_render_infos(t_wolf *wolf, t_ray *ray)
{
	t_block_normal		*block;
	t_texture			*texture;
	t_render_info		infos;
	double				wallX;
	double				garbage;

	block = (t_block_normal *)ray->hit->block;
	texture = block->faces[ray->face].texture;
	infos.height = S_HEIGHT / ray->dist;
	if (ray->side == 0)
		wallX= wolf->player.pos.y + ray->dist * ray->dir.y;
	else
		wallX = wolf->player.pos.x + ray->dist * ray->dir.x;
	wallX = modf(wallX, &garbage);
	infos.texX = wallX * texture->size.x;
	infos.y = S_HEIGHT_2 + infos.height * (wolf->player.pos.z + 1) / 2. - infos.height * block->super.height;
	return (infos);
}

#include <assert.h>

t_bool	render_block_normal_wall(t_wolf *wolf, t_ray *from_ray, t_ray *to_ray, int last_y)
{
	t_block_normal		*block;
	t_texture	*texture;
	t_render_info	from;
	t_render_info	to;

	block = (t_block_normal *)from_ray->hit->block;
	texture = block->faces[from_ray->face].texture;
	from = get_render_infos(wolf, from_ray);
/*	if (from_ray == to_ray)
	{
		apply_texture(wolf->img, texture,
			(SDL_Rect){ from.texX, 0, 1, texture->size.y }, 
			(SDL_Rect){ from_ray->x, from.y, 1, from.height * block->super.height });
	} 
	else
	{*/
		to = get_render_infos(wolf, to_ray);
		int x = from_ray->x;
		if (to.y >= last_y && from.y >= last_y)
			return (FALSE);
		while (x <= to_ray->x)
		{
			float progress = (float)(x - from_ray->x)/(float)(to_ray->x == from_ray->x ? 1 : to_ray->x - from_ray->x);
			int texX = from.texX + progress * (float)(to.texX - from.texX);
			int y = from.y + progress * (to.y - from.y);
			double height = from.height + progress * (to.height - from.height);
			if (y >= last_y)
			{
				x++;
				continue ;
			}
			float ratio = y < 0 ? 1 : (last_y - y) / (height * block->super.height);
			if (ratio > 1)
				ratio = 1;
			int texY = texture->size.y * (1 - ratio);

			apply_texture(wolf->img, texture,
				(SDL_Rect){ texX, 0, 1, texture->size.y - texY }, 
				(SDL_Rect){ x, y, 1, y < 0 ? height * block->super.height : fmin(height * block->super.height, last_y - y) });
			x++;
		}
	//}
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
			wolf->img->pixels[i] = 255;
	}
	return (FALSE);
}
