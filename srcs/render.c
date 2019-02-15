/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 19:54:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/16 00:51:40 by llelievr         ###   ########.fr       */
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

void				render_minimap(t_wolf *wolf)
{
	int i;
	int j;
	const float		s_w = (24. / 200.);
	const float		s_h = (24. / 200.);
	t_vec2 map;
	t_vec2 pos;

	i = -1;
	while (++i < 200)
	{
		j = -1;
		while (++j < 200)
		{
			map = (t_vec2){j * s_w, i * s_h};
			Uint32 color = worldMap[(int)map.x][(int)map.y] > 0 ? 0 : 0x666666;
			if (map.x <= wolf->player.pos.x + 0.2 && map.x >= wolf->player.pos.x - 0.2 && map.y <= wolf->player.pos.y + 0.2 && map.y >= wolf->player.pos.y - 0.2)
				color = 0xFF0000;
			((unsigned int *)wolf->pixels)[(i * (int)S_WIDTH) + j + (int)S_WIDTH - 200 ] = color;
		}
	}
	pos = (t_vec2){wolf->player.pos.x * (200. / 24.) + S_WIDTH - 200, wolf->player.pos.y * (200. / 24.)};
	draw_line(wolf, (t_pixel){pos.x, pos.y, 0xFFFFFF}, (t_pixel){(pos.x + (cosf(wolf->player.rotation) * 8)), (pos.y + (sinf(wolf->player.rotation) * 8)), 0xFFFFFF});
}

void			apply_surface(unsigned char **dest, SDL_Surface *s, SDL_Rect src, SDL_Rect dst)
{
	int i;
	int j;
	const float		s_w = src.w / (float)dst.w;
	const float		s_h = src.h / (float)dst.h;
	unsigned int index;

	i = -1;
	while (++i < dst.h)
	{
		j = -1;
		while (++j < dst.w)
		{
			index = (((dst.y + i) * (int)S_WIDTH) + (dst.x + j));
			if (index >= IMG_MAX_I)
				continue;
			((unsigned int *)*dest)[index] = getpixel(s, (int)(j * s_w) + src.x, (int)(i * s_h) + src.y);
		}
	}
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
		cast_ray(wolf, &ray, x);
		int height_f = S_HEIGHT / ray.dist;
		int height = height_f / 2.0;
		double wallX;
		if (ray.side == 0)
			wallX = wolf->player.pos.y + ray.dist * ray.dir.y;
		else
			wallX = wolf->player.pos.x + ray.dist * ray.dir.x;
		wallX -= floor((wallX));
		int texWidth = 64;
		int texX = wallX * texWidth;
		if (ray.side == 0 && ray.dir.x > 0)
			texX = texWidth - texX - 1;
		if (ray.side == 1 && ray.dir.y < 0)
			texX = texWidth - texX - 1;
		SDL_Rect a = { texX + ((ray.hit + (ray.side == 1)) % 6) * 64, ((ray.hit + (ray.side == 1)) / 6) * 64, 1, 64 };
		SDL_Rect b = { x, S_HEIGHT_2 - height, 1, height_f };
		apply_surface(&wolf->pixels, wolf->texture, a, b);


		t_vec2 floorWall;
		if (ray.side == 0 && ray.dir.x > 0)
			floorWall = (t_vec2) {ray.hit_pos.x, ray.hit_pos.y + wallX};
		else if (ray.side == 0 && ray.dir.x < 0)
			floorWall = (t_vec2) {ray.hit_pos.x + 1.0, ray.hit_pos.y + wallX};
		else if (ray.side == 1 && ray.dir.y > 0)
			floorWall = (t_vec2) {ray.hit_pos.x + wallX, ray.hit_pos.y};
		else
			floorWall = (t_vec2) {ray.hit_pos.x + wallX, ray.hit_pos.y + 1.0};
		double distWall, distPlayer, currentDist;
		int drawEnd = (S_HEIGHT_2 + height);
		distWall = ray.dist;
		distPlayer = 0.0;
		for (int y = drawEnd + 1; y < S_HEIGHT; y++)
		{
			if (y > S_HEIGHT || y < 0)
				continue ;
			currentDist = S_HEIGHT / (2 * y - S_HEIGHT);
			double weight = (currentDist - distPlayer) / (distWall - distPlayer);
			t_vec2 curr_floor = (t_vec2)
			{
				weight * floorWall.x + (1.0 - weight) * wolf->player.pos.x,
				weight * floorWall.y + (1.0 - weight) * wolf->player.pos.y
			};
			int floorTexX, floorTexY;
			floorTexX = (int)(curr_floor.x * texWidth) % texWidth;
			floorTexY = (int)(curr_floor.y * 64) % 64;
			((unsigned int *)wolf->pixels)[(y * (int)S_WIDTH) + x] = getpixel(wolf->texture, floorTexX, floorTexY);
			if (S_HEIGHT - y < 0 || S_HEIGHT - y > IMG_MAX_I)
				continue;
			((unsigned int *)wolf->pixels)[(((int)S_HEIGHT - y) * (int)S_WIDTH) + x] = getpixel(wolf->texture, floorTexX + 256, floorTexY + 128);
		}
	}
}
