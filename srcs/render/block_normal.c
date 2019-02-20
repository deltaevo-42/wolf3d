/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_normal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 13:33:02 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/20 18:28:43 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

t_bool	render_block_normal_wall(t_wolf *wolf, t_ray *ray)
{
	t_block_normal		*block;
	t_texture_normal	*texture;
	int					height;
	double				wallX;
	int					texX;

	block = (t_block_normal *)ray->hit->block;
	texture = (t_texture_normal *)block->faces[ray->face].texture;
	height = S_HEIGHT / ray->dist;
	if (ray->side == 0)
		wallX = wolf->player.pos.y + ray->dist * ray->dir.y;
	else
		wallX = wolf->player.pos.x + ray->dist * ray->dir.x;
	wallX -= floor((wallX));
	texX = wallX * texture->surface->w;
	if ((ray->side == 0 && ray->dir.x > 0) || (ray->side == 1 && ray->dir.y < 0))
		texX = texture->surface->w - texX - 1;
	apply_surface(&wolf->pixels, texture->surface, 
		(SDL_Rect){ texX, 0, 1, texture->surface->h }, 
		(SDL_Rect){ ray->x, (S_HEIGHT / 2) - (height * wolf->player.pos.z), 1, height});
	return (wolf->player.pos.z < 0);
}
