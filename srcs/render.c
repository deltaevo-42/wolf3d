/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 19:54:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/21 17:34:45 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void			render_debug(t_wolf *wolf)
{
	SDL_Surface		*fps_text;
	SDL_Texture		*fps_texture;
	SDL_Rect		dest;

	fps_text = TTF_RenderText_Blended(wolf->fonts.helvetica, ft_int_to_str(wolf->stats.fps).str, (SDL_Color){255, 255, 255, 0});
	fps_texture = SDL_CreateTextureFromSurface(wolf->renderer, fps_text);
	dest = (SDL_Rect){5, 5, 25, 25};
	SDL_RenderCopy(wolf->renderer, fps_texture, NULL, &dest);
	SDL_DestroyTexture(fps_texture);
	SDL_FreeSurface(fps_text);
	fps_text = TTF_RenderText_Blended(wolf->fonts.helvetica, ft_int_to_str((int)wolf->stats.avg_ms).str, (SDL_Color){255, 255, 255, 0});
	fps_texture = SDL_CreateTextureFromSurface(wolf->renderer, fps_text);
	dest = (SDL_Rect){5, 35, 25, 25};
	SDL_RenderCopy(wolf->renderer, fps_texture, NULL, &dest);
	SDL_DestroyTexture(fps_texture);
	SDL_FreeSurface(fps_text);
}

t_bool			render_wall(t_wolf *wolf, t_ray *ray)
{
	if (ray->hit->block->type == B_NORMAL)
		return (render_block_normal_wall(wolf, ray));
	return (TRUE);
}

void			render_main(t_wolf *wolf)
{
	int		x;
	t_ray	ray;

	ft_memset(wolf->pixels, 0, S_WIDTH * 4 * S_HEIGHT);
	//printf("RAY\n\tdir (%f, %f)\n\tdist (%f)\n\thit (%d)\n\thit_pos (%d, %d)\n\tside (%d)\n", ray.dir.x, ray.dir.y, ray.dist, ray.hit, ray.hit_pos.x, ray.hit_pos.y, ray.side);
	x = -1;
	while (++x < S_WIDTH)
	{
		ray = create_ray(wolf, x);
		t_ray r2 = ray;
		t_ray r3;
	/*	next_ray(wolf, &ray);
		render_wall(wolf, &ray);*/
		//wolf->last_rays[x] = ray;
		while (next_ray(wolf, &r2))
		{
			if (r2.hit)
			{
				render_wall(wolf, &r2);
			
				float h1 = S_HEIGHT / r2.dist;
				int p1 = S_HEIGHT_2 + h1 * (wolf->player.pos.z + 1) * 0.5 - h1;
				r3 = r2;
				if (p1 < S_HEIGHT_2)
					break;
				while (next_ray(wolf, &r2))
				{
					if (!r2.hit)
						break;
					r3 = r2;
				}
				float h0 = S_HEIGHT / r2.dist;
				int p0 = S_HEIGHT_2 + h0 * (wolf->player.pos.z + 1) / 2. - h0;
				for (int y = p0; p0 < p1 && y < p1; y++)
				{
					if (y > S_HEIGHT || y < 0)
						continue ;
					((unsigned int *)wolf->pixels)[(y * (int)S_WIDTH) + x] = 255;
				}
			}
		
		}
		ray = r3;
		//printf("%d %d, %d\n", !!ray.hit, ray.hit_pos.x, ray.hit_pos.y);
		//render_wall(wolf, &ray);
		/*t_texture_normal *texture = ((t_texture_normal *)((t_block_normal *)((t_block_state *)ray.hit)->block)->faces[ray.face].texture);
		
		double wallX;
		if (ray.side == 0)
			wallX = wolf->player.pos.y + ray.dist * ray.dir.y;
		else
			wallX = wolf->player.pos.x + ray.dist * ray.dir.x;
		wallX -= floor((wallX));
		int texWidth = texture->surface->w;
		int texX = wallX * texWidth;
		if (ray.side == 0 && ray.dir.x > 0)
			texX = texWidth - texX - 1;
		if (ray.side == 1 && ray.dir.y < 0)
			texX = texWidth - texX - 1;
		SDL_Rect a = { texX, 0, 1, texture->surface->h };
		SDL_Rect b = { x, S_HEIGHT_2 - height, 1, height_f };
		apply_surface(&wolf->pixels, texture->surface, a, b);*/

		// float height = S_HEIGHT / ray.dist;
		// float wallX = 0;
		// if (ray.side == 0)
		// 	wallX = wolf->player.pos.y + ray.dist * ray.dir.y;
		// else
		// 	wallX = wolf->player.pos.x + ray.dist * ray.dir.x;
		// wallX -= floor((wallX));
		// int texWidth = wolf->tmp_texture->w;
		// t_vec2 floorWall;
		// if (ray.side == 0 && ray.dir.x > 0)
		// 	floorWall = (t_vec2) {ray.hit_pos.x, ray.hit_pos.y + wallX};
		// else if (ray.side == 0 && ray.dir.x < 0)
		// 	floorWall = (t_vec2) {ray.hit_pos.x + 1.0, ray.hit_pos.y + wallX};
		// else if (ray.side == 1 && ray.dir.y > 0)
		// 	floorWall = (t_vec2) {ray.hit_pos.x + wallX, ray.hit_pos.y};
		// else
		// 	floorWall = (t_vec2) {ray.hit_pos.x + wallX, ray.hit_pos.y + 1.0};
		// float distWall;
		// float drawEnd = S_HEIGHT_2 + height * (wolf->player.pos.z + 1) / 2. - height;
		// distWall = ray.dist / (wolf->player.pos.z + 1);
		// float d = S_HEIGHT + 2 * wolf->player.pos.z;
		// for (int y = drawEnd ; y < S_HEIGHT; y++)
		// {
		// 	if (y > S_HEIGHT || y < 0 || ((unsigned int *)wolf->pixels)[(y * (int)S_WIDTH) + x] != 0)
		// 		continue ;
		// 	float weight = S_HEIGHT / ((2 * y - d) * distWall);
		// 	t_vec2 curr_floor = (t_vec2)
		// 	{
		// 		weight * floorWall.x + (1.0 - weight) * wolf->player.pos.x,
		// 		weight * floorWall.y + (1.0 - weight) * wolf->player.pos.y
		// 	};
		// 	int floorTexX, floorTexY;
		// 	floorTexX = ft_abs((int)(curr_floor.x * texWidth)) % texWidth;
		// 	floorTexY = ft_abs((int)(curr_floor.y * wolf->tmp_texture->h)) % wolf->tmp_texture->h;
		
		// 	((unsigned int *)wolf->pixels)[(y * (int)S_WIDTH) + x] = getpixel(wolf->tmp_texture, floorTexX, floorTexY)/*0x00FF00*/;
		// 	if (S_HEIGHT - y < 0 || S_HEIGHT - y > IMG_MAX_I)
		// 		continue;
		// 	//((unsigned int *)wolf->pixels)[(((int)S_HEIGHT - y) * (int)S_WIDTH) + x] = getpixel(wolf->tmp_texture, floorTexX, floorTexY)/*0x0000FF*/;
		// }
	}
}
