/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 19:54:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/24 21:52:52 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void			render_debug(t_wolf *wolf)
{
	SDL_Surface		*fps_text;
	SDL_Texture		*fps_texture;
	SDL_Rect		dest;

	fps_text = TTF_RenderText_Blended(wolf->fonts.helvetica, ft_int_to_str(wolf->stats.fps).str, (SDL_Color){255, 255, 255, 0});
	//fps_texture = SDL_CreateTextureFromSurface(wolf->renderer, fps_text);
	//	printf("%d %d\n", fps_text->w, fps_text->h);
	apply_surface_blended(wolf->pixels, fps_text, (SDL_Rect){0, 0, fps_text->w, fps_text->h}, (SDL_Rect){5, 5, fps_text->w + 5, fps_text->h + 5});
	/*SDL_RenderCopy(wolf->renderer, fps_texture, NULL, &dest);
	SDL_DestroyTexture(fps_texture);
	SDL_FreeSurface(fps_text);
	fps_text = TTF_RenderText_Blended(wolf->fonts.helvetica, ft_int_to_str((int)wolf->stats.avg_ms).str, (SDL_Color){255, 255, 255, 0});
	fps_texture = SDL_CreateTextureFromSurface(wolf->renderer, fps_text);
	dest = (SDL_Rect){5, 35, 25, 25};
	SDL_RenderCopy(wolf->renderer, fps_texture, NULL, &dest);
	SDL_DestroyTexture(fps_texture);*/
	SDL_FreeSurface(fps_text);
}

t_bool			render_wall(t_wolf *wolf, t_ray *ray)
{
	if (ray->hit->block->type == B_NORMAL)
		return (render_block_normal_wall(wolf, ray));
	if (ray->hit->block->type == B_ROUND)
		return (render_block_round_wall(wolf, ray));
	return (TRUE);
}

void			render_floor(t_wolf *wolf, int x, t_ray *ray);

void			render_main(t_wolf *wolf)
{
	int		x;
	t_ray	ray;

	x = -1;
	while (++x < S_WIDTH)
	{
		ray = create_ray(wolf, x);
		t_bool skip = FALSE;

		while (skip || next_ray(wolf, &ray))
		{
			skip = FALSE;
			if (ray.hit)
			{
				render_wall(wolf, &ray);
				if (ray.hit->block->height == wolf->world.size.z
					|| (ray.dist <= 1 && wolf->player.pos.z + 1 <= ray.hit->block->height))
					break ;
				float hit_h = ray.hit->block->height;
				float h1 = S_HEIGHT / ray.dist;
				int p1 = S_HEIGHT_2 + h1 * (wolf->player.pos.z + 1) * 0.5 - h1 * hit_h;
				if (next_ray(wolf, &ray))
					skip = TRUE;
				float h0 = S_HEIGHT / ray.dist;
				int p0 = S_HEIGHT_2 + h0 * (wolf->player.pos.z + 1) / 2. - h0 * hit_h;
				for (int y = p0; p0 < p1 && y < p1; y++)
				{
					int i = y * S_WIDTH + x;
					if (i >= IMG_MAX_I || i < 0)
						break ;
					if (wolf->pixels[i] == 0)
						wolf->pixels[i] = 255;
				}
			}
		}
		wolf->last_rays[x] = ray;
		if (ray.dist > 1 && ray.hit)
			render_floor(wolf, x, &ray);
	}
}

void	render_floor(t_wolf *wolf, int x, t_ray *ray)
{
	int height = S_HEIGHT / ray->dist;

	int world_height = height * wolf->world.size.z;
	int offset = height * (wolf->player.pos.z + 1) / 2. - height;
	int top = S_HEIGHT_2 - world_height / 2 + offset - 1;
	int bottom = S_HEIGHT_2 + world_height / 2 + offset + 1;

	float wallX = 0;
	if (ray->side == 0)
		wallX = wolf->player.pos.y + ray->dist * ray->dir.y;
	else
		wallX = wolf->player.pos.x + ray->dist * ray->dir.x;
	wallX -= floor((wallX));
	int texWidth = wolf->tmp_texture->w;
	t_vec2 floorWall;
	if (ray->side == 0 && ray->dir.x > 0)
		floorWall = (t_vec2) {ray->hit_pos.x, ray->hit_pos.y + wallX};
	else if (ray->side == 0 && ray->dir.x < 0)
		floorWall = (t_vec2) {ray->hit_pos.x + 1.0, ray->hit_pos.y + wallX};
	else if (ray->side == 1 && ray->dir.y > 0)
		floorWall = (t_vec2) {ray->hit_pos.x + wallX, ray->hit_pos.y};
	else
		floorWall = (t_vec2) {ray->hit_pos.x + wallX, ray->hit_pos.y + 1.0};
	float distZ = ray->dist / (wolf->player.pos.z + 1);
	float rDistZ = ray->dist / -(wolf->world.size.z - wolf->player.pos.z + 1);

	for (int y = 0; y < S_HEIGHT_2; y++)
	{
		if (bottom + y < S_HEIGHT)
		{
			float weight = S_HEIGHT/((2 * (y + bottom) - S_HEIGHT) * distZ);
			t_vec2 curr_floor = (t_vec2)
			{
				weight * floorWall.x + (1.0 - weight) * wolf->player.pos.x,
				weight * floorWall.y + (1.0 - weight) * wolf->player.pos.y
			};
			int floorTexX, floorTexY;
			floorTexX = ft_abs((int)(curr_floor.x * texWidth)) % texWidth;
			floorTexY = ft_abs((int)(curr_floor.y * wolf->tmp_texture->h)) % wolf->tmp_texture->h;

			if (wolf->pixels[((bottom + y) * (int)S_WIDTH + x)] == 0)
				wolf->pixels[((bottom + y) * (int)S_WIDTH) + x] = getpixel(wolf->tmp_texture, floorTexX, floorTexY);
		}
		if (y <= top)
		{
			float weight = S_HEIGHT/((2 * (top - y) - S_HEIGHT) * rDistZ);
			t_vec2 curr_floor = (t_vec2)
			{
				weight * floorWall.x + (1.0 - weight) * wolf->player.pos.x,
				weight * floorWall.y + (1.0 - weight) * wolf->player.pos.y
			};
			int floorTexX, floorTexY;
			floorTexX = ft_abs((int)(curr_floor.x * texWidth)) % texWidth;
			floorTexY = ft_abs((int)(curr_floor.y * wolf->tmp_texture->h)) % wolf->tmp_texture->h;

			if (wolf->pixels[((top - y) * (int)S_WIDTH + x)] == 0)
				wolf->pixels[((top - y) * (int)S_WIDTH) + x] = getpixel(wolf->tmp_texture, floorTexX, floorTexY);
		}
	}
}
