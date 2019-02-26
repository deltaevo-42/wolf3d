/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_round.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/24 22:00:17 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/26 17:54:18 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

t_bool	render_block_round_wall(t_wolf *wolf, t_ray *ray)
{
	t_block_round *block;
	t_texture 	*texture;
	float		height;
	float		wallX;
	int			texX;

	block = (t_block_round *)ray->hit->block;
	texture = block->texture;
	height = (S_HEIGHT / ray->dist);
	wallX = (atan2(ray->circle_last_hit_y, ray->circle_last_hit_x) + M_PI) / (2 * M_PI);
	texX = wallX * texture->size.x;
	apply_texture(wolf->img, texture,
		(SDL_Rect){ texX, 0, 1, texture->size.y }, 
		(SDL_Rect){ ray->x, S_HEIGHT_2 + height * (wolf->player.pos.z + 1) / 2. - height * block->super.height, 1, height * block->super.height});
	return (FALSE);
}

t_bool	render_block_round_top(t_wolf *wolf, t_ray *ray, t_block_state *hit, int p1)
{
	t_block_round *block;
	block = (t_block_round *)hit->block;
	float hit_h = block->super.height;
	float h0 = S_HEIGHT / ray->circle_last_out_dist;
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
