/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 19:54:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/14 18:51:41 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void			render_debug(t_wolf *wolf)
{
	SDL_Surface		*fps_text;
	SDL_Texture		*fps_texture;
	SDL_Rect		dest;

	fps_text = TTF_RenderText_Blended(wolf->fonts.helvetica, ft_int_to_str(wolf->stats.fps).str, (SDL_Color){255, 255, 255});
	fps_texture = SDL_CreateTextureFromSurface(wolf->renderer, fps_text);
	dest = (SDL_Rect){5, 5, 25, 25};
	SDL_RenderCopy(wolf->renderer, fps_texture, NULL, &dest);
	SDL_DestroyTexture(fps_texture);
	SDL_FreeSurface(fps_text);
	fps_text = TTF_RenderText_Blended(wolf->fonts.helvetica, ft_int_to_str((int)wolf->stats.avg_ms).str, (SDL_Color){255, 255, 255});
	fps_texture = SDL_CreateTextureFromSurface(wolf->renderer, fps_text);
	dest = (SDL_Rect){5, 35, 25, 25};
	SDL_RenderCopy(wolf->renderer, fps_texture, NULL, &dest);
	SDL_DestroyTexture(fps_texture);
	SDL_FreeSurface(fps_text);
}

void			render_main(t_wolf *wolf)
{
	int		x;
	t_ray	ray;

	
	
	ft_memset(wolf->pixels, 0, S_WIDTH * 4 * S_HEIGHT);
	//cast_ray(wolf, &ray, 15);
	//printf("RAY\n\tdir (%f, %f)\n\tdist (%f)\n\thit (%d)\n\thit_pos (%d, %d)\n\tside (%d)\n", ray.dir.x, ray.dir.y, ray.dist, ray.hit, ray.hit_pos.x, ray.hit_pos.y, ray.side);

	x = -1;
	while (++x < S_WIDTH)
	{
		cast_ray(wolf, &ray, x);
		int height = S_HEIGHT / ray.dist / 2.0;
		t_color color = (t_color) {255, 255, 255, 0};
		if (ray.hit == 1)
			color = (t_color) {255, 0, 0, 0};
		if (ray.hit == 2)
			color = (t_color) {0, 255, 0, 0};
		if (ray.hit == 3)
			color = (t_color) {0, 0, 255, 0};
		if (ray.hit == 4)
			color = (t_color) {255, 0, 255, 0};
		color = ft_color_gradient(color, (t_color) {color.r + 20, color.g + 20, color.b + 20, 0}, ray.dist / 24);
		for (int h = S_HEIGHT / 2 - height; h < S_HEIGHT / 2 + height; h++)
		{
			if (h > S_HEIGHT || h < 0)
				continue ;
			const unsigned int offset = ((h * (int)S_WIDTH) + x);
			((unsigned int *)wolf->pixels)[offset] = ft_color_i(color);
		}

		double wallX;	//where exactly the wall was hit
		if (ray.side == 0)
			wallX = wolf->player.pos.y + ray.dist * ray.dir.y;
		else
			wallX = wolf->player.pos.x + ray.dist * ray.dir.x;
		wallX -= floor((wallX));
		t_vec2 floorWall;
		if (ray.side == 0 && ray.dir.x > 0)
			floorWall = (t_vec2) {ray.hit_pos.x, ray.hit_pos.y + wallX};
		else if (ray.side == 0 && ray.dir.x < 0)
			floorWall = (t_vec2) {ray.hit_pos.x + 1.0, ray.hit_pos.y + wallX};
		else if (ray.side == 1 && ray.dir.y > 0)
			floorWall = (t_vec2) {ray.hit_pos.x + wallX, ray.hit_pos.y};
		else
			floorWall = (t_vec2) {ray.hit_pos.x + wallX, ray.hit_pos.y + 1.0};
		int texWidth = 64;
		double distWall, distPlayer, currentDist;
		int drawEnd = (S_HEIGHT / 2 + height);
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
			if (S_HEIGHT - y < 0 || S_HEIGHT - y > S_HEIGHT)
				continue;
			((unsigned int *)wolf->pixels)[(((int)S_HEIGHT - y) * (int)S_WIDTH) + x] = getpixel(wolf->texture, floorTexX + 256, floorTexY + 128);
			//SDL_BlitSurface(image, &srcrect, screen, &dstrect);
			/*dstrect = (SDL_Rect){i, S_HEIGHT - y, 1, 1};
			srcrect = (SDL_Rect){floorTexX + 64 * 5, floorTexY + 64 * 3, 1, 1};*/
			//SDL_BlitSurface(image, &srcrect, screen, &dstrect);
			//SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
			/*	SDL_SetRenderDrawColor(renderer, 255 / currentDist, 255,
				255, 255);
				SDL_RenderDrawPoint(renderer, i, y);
				SDL_RenderDrawPoint(renderer, i, HEIGHT - y);*/
		}
	}
}
