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
t_bool	render_block_normal_wall(t_wolf *wolf, t_ray *ray)
{
	t_block_normal		*block;
	t_texture			*texture;
	float				height;
	double				wallX;
	int					texX;

	block = (t_block_normal *)ray->hit->block;
	texture = block->faces[ray->face].texture;
	height = (S_HEIGHT / ray->dist);
	if (ray->side == 0)
		wallX = wolf->player.pos.y + ray->dist * ray->dir.y;
	else
		wallX = wolf->player.pos.x + ray->dist * ray->dir.x;
	wallX -= floor((wallX));
	texX = wallX * texture->size.x;
	if ((ray->side == 0 && ray->dir.x > 0) || (ray->side == 1 && ray->dir.y < 0))
		texX = texture->size.x - texX - 1;
	apply_texture(wolf->img, texture,
		(SDL_Rect){ texX, 0, 1, texture->size.y }, 
		(SDL_Rect){ ray->x, S_HEIGHT_2 + height * (wolf->player.pos.z + 1) / 2. - height * block->super.height, 1, height * block->super.height});
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
