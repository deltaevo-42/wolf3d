/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 19:54:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/28 03:44:39 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void			render_debug(t_wolf *wolf)
{
	SDL_Surface		*text;

	text = TTF_RenderText_Blended(wolf->fonts.helvetica, ft_int_to_str(wolf->stats.fps).str, (SDL_Color){255, 255, 255, 0});
	apply_surface_blended(wolf->img, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){5, 5, text->w + 5, text->h + 5});
	SDL_FreeSurface(text);
	text = TTF_RenderText_Blended(wolf->fonts.helvetica, ft_int_to_str((int)wolf->stats.avg_ms).str, (SDL_Color){255, 255, 255, 0});
	apply_surface_blended(wolf->img, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){5, text->h + 10, text->w + 5, text->h + 5});
	SDL_FreeSurface(text);
}

t_bool			render_wall(t_wolf *wolf, t_ray *ray)
{
	if (ray->hit->block->type == B_NORMAL)
		return (render_block_normal_wall(wolf, ray));
	if (ray->hit->block->type == B_ROUND)
		return (render_block_round_wall(wolf, ray));
	return (TRUE);
}

t_bool			render_top(t_wolf *wolf, t_ray *ray, t_block_state *hit, int p)
{
	if (hit->block->type == B_NORMAL)
		return (render_block_normal_top(wolf, ray, hit, p));
	if (hit->block->type == B_ROUND)
		return (render_block_round_top(wolf, ray, hit, p));
	return (TRUE);
}


void			render_floor(t_wolf *wolf, int x, t_ray *ray, t_bool f);

void			render_main(t_wolf *wolf)
{
	int		x;
	t_ray	ray;

	x = -1;
	while (++x < S_WIDTH)
	{
		ray = create_ray(wolf, x);
		t_ray last = ray;
		while (1)
		{
			if (ray.hit)
			{
				t_block_state *hit = ray.hit;
				float h = S_HEIGHT / ray.dist;
				int p = S_HEIGHT_2 + h * (wolf->player.pos.z + 1) * 0.5 - h * hit->block->height;
				render_floor(wolf, x, &ray, TRUE); // On veut decider quand cacher le sol au dessous d un block (C est actuellement a TRUE car j arrive pas a trouver dans quel cas le definir) cette modif n as visiblement aucun impact sur les pers (tant mieux)
				render_wall(wolf, &ray);
				if (ray.hit->block->height == wolf->world.size.z || (ray.dist <= 1 && wolf->player.pos.z + 1 <= ray.hit->block->height && wolf->player.pos.z >= 0.2))
					break ;
				if (p <= 0 && p + h > S_HEIGHT)
					break;
				last = ray;
				next_ray(wolf, &ray);
				render_top(wolf, &ray, hit, p);
				continue ;
			}
			if (!next_ray(wolf, &ray))
				break ;
		}
		wolf->last_rays[x] = ray;
	}
}

void	render_floor(t_wolf *wolf, int x, t_ray *ray, t_bool f)
{
	/*t_ray r2 = *ray;
	prev_ray(wolf, &r2);
	if (r2.hit && ray->hit)
		f = FALSE;*/

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

	SDL_Surface *floor_tex = (wolf->world.floor)->surface;
	SDL_Surface *ceil_tex = (wolf->world.ceil)->surface;

	for (int y = 0; y < S_HEIGHT_2; y++)
	{
		if (bottom + y < S_HEIGHT && f)
		{
			float weight = S_HEIGHT/((2 * (y + bottom) - S_HEIGHT) * distZ);
			t_vec2 curr_floor = (t_vec2)
			{
				weight * floorWall.x + (1.0 - weight) * wolf->player.pos.x,
				weight * floorWall.y + (1.0 - weight) * wolf->player.pos.y
			};
			int floorTexX, floorTexY;
			floorTexX = ft_abs((int)(curr_floor.x * (int)wolf->world.floor->size.x)) % (int)wolf->world.floor->size.x;
			floorTexY = ft_abs((int)(curr_floor.y * (int)wolf->world.floor->size.y)) % (int)wolf->world.floor->size.y;

			if (wolf->img->pixels[((bottom + y) * (int)S_WIDTH + x)] == 0)
				wolf->img->pixels[((bottom + y) * (int)S_WIDTH) + x] = getpixel(floor_tex, floorTexX, floorTexY);
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
			floorTexX = ft_abs((int)(curr_floor.x * (int)wolf->world.ceil->size.x)) % (int)wolf->world.ceil->size.x;
			floorTexY = ft_abs((int)(curr_floor.y * (int)wolf->world.ceil->size.y)) % (int)wolf->world.ceil->size.y;

			if (wolf->img->pixels[((top - y) * (int)S_WIDTH + x)] == 0)
				wolf->img->pixels[((top - y) * (int)S_WIDTH) + x] = getpixel(ceil_tex, floorTexX, floorTexY);
		}
	}
}
