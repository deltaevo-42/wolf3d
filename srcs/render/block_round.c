/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_round.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/24 22:00:17 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/03 16:01:08 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

t_bool	render_block_round_wall(t_wolf *wolf, t_ray *ray)
{
	t_block_round	*block;
	t_texture		*texture;
	float			height;
	float			wall_x;
	int				tex_x;

	block = ((t_block_round *)ray->hit->block);
	texture = block->texture;
	height = (S_HEIGHT / ray->dist);
	wall_x = (atan2(ray->circle_last_hit_y, ray->circle_last_hit_x) + M_PI)
		/ (2 * M_PI);
	tex_x = wall_x * texture->size.x;
	apply_texture(wolf->img, texture,
		(SDL_Rect){ tex_x, 0, 1, texture->size.y },
		(SDL_Rect){ ray->x, S_HEIGHT_2 + height * (wolf->player.pos.z + 1) / 2.
			- height * block->super.height, 1, height * block->super.height});
	return (FALSE);
}

t_bool	render_block_round_top(t_wolf *wolf, t_ray *ray,
			t_block_state *hit, int p1)
{
	const t_block_round	*block = (t_block_round *)hit->block;
	const float			h0 = S_HEIGHT / ray->circle_last_out_dist;
	int					p0;
	int					y;
	int					i;

	p0 = S_HEIGHT_2 + h0 * (wolf->player.pos.z + 1) / 2.
			- h0 * block->super.height;
	y = p0;
	while (p0 < p1 && y < p1)
	{
		i = y * S_WIDTH + ray->x;
		if (i < 0 || (uint32_t)i >= wolf->img->size)
			break ;
		if (wolf->img->pixels[i] == 0)
			wolf->img->pixels[i] = 0xFF0000FF;
		y++;
	}
	return (FALSE);
}
