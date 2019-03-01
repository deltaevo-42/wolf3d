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
	text = TTF_RenderText_Blended(wolf->fonts.helvetica, ft_int_to_str(wolf->stats.num_rays).str, (SDL_Color){255, 255, 255, 0});
	apply_surface_blended(wolf->img, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){5, text->h * 2 + 15, text->w + 5, text->h + 5});
	SDL_FreeSurface(text);
}

t_bool			render_wall(t_wolf *wolf, t_ray *from, t_ray *to, int last_y)
{
	if (from->hit->block->type == B_NORMAL)
		return (render_block_normal_wall(wolf, from, to, last_y));
	if (from->hit->block->type == B_ROUND)
		return (render_block_round_wall(wolf, from));
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


void			render_floor(t_wolf *wolf, t_ray *from, t_ray *to, t_bool f, int last_floor);

#include <assert.h>
#include <limits.h>

void			cast_ray(t_wolf *wolf, t_ray *ray, int last_y)
{
	while (1)
	{
		if (ray->hit)
		{
			t_block_state *hit = ray->hit;
			float h = S_HEIGHT / ray->dist;
			int p = S_HEIGHT_2 + h * (wolf->player.pos.z + 1) * 0.5 - h * hit->block->height;
			render_wall(wolf, ray, ray, last_y);
			if (p < last_y)
				render_floor(wolf, ray, ray, TRUE, last_y);
			if (p < last_y)
				last_y = p;
			if (ray->hit->block->height == wolf->world.size.z)
				break ;
			if (p <= 0 && p + h > S_HEIGHT)
				break;
			next_ray(ray);
			render_top(wolf, ray, hit, p);
			continue ;
		}
		if (!next_ray(ray))
			break ;
	}
	wolf->last_rays[wolf->stats.num_rays++] = *ray;
	render_floor(wolf, ray, ray, FALSE, S_HEIGHT);
}

t_bool			double_cast_ray(t_wolf *wolf, int x1, int x2)
{
	t_ray	first = create_ray(wolf, x1, (t_vec2) { wolf->player.pos.x, wolf->player.pos.y });
	t_ray	second = create_ray(wolf, x2, (t_vec2) { wolf->player.pos.x, wolf->player.pos.y });
	int		last_y = INT_MAX;

	while (1)
	{
		t_bool	next_first = next_ray(&first);
		t_bool	next_second = next_ray(&second);

		if (!next_first && !next_second)
			break ;
		if (next_first != next_second)
		{
			cast_ray(wolf, &first, last_y);
			cast_ray(wolf, &second, last_y);
			return (FALSE);
		}
		if ((first.fhit && first.fhit->block->type != B_NORMAL)
			|| (second.fhit && second.fhit->block->type != B_NORMAL))
		{
			cast_ray(wolf, &first, last_y);
			cast_ray(wolf, &second, last_y);
			return (FALSE);
		}
		if (first.hit || second.hit)
		{
			if (first.hit != second.hit || first.face != second.face)
			{
				cast_ray(wolf, &first, last_y);
				cast_ray(wolf, &second, last_y);
				return (FALSE);
			}
			t_block_state *hit = first.hit;
			float hf = S_HEIGHT / first.dist;
			int pf = S_HEIGHT_2 + hf * (wolf->player.pos.z + 1) * 0.5 - hf * hit->block->height;
			float hs = S_HEIGHT / second.dist;
			int ps = S_HEIGHT_2 + hs * (wolf->player.pos.z + 1) * 0.5 - hs * hit->block->height;
			float h = pf > ps ? hf : hs;
			int p = pf > ps ? pf : ps;
			render_wall(wolf, &first, &second, last_y);
			if (p < last_y)
				render_floor(wolf, &first, &second, TRUE, last_y);
			if (p < last_y)
				last_y = p;
			if (first.hit->block->height == wolf->world.size.z)
				break ;
			if (p <= 0 && p + h > S_HEIGHT)
				break;
			next_ray(&first);
			next_ray(&second);
			render_top(wolf, &first, hit, pf);
			prev_ray(&first);
			prev_ray(&second);
		}
	}
	render_floor(wolf, &first, &second, FALSE, S_HEIGHT);
	wolf->last_rays[wolf->stats.num_rays++] = first;
	wolf->last_rays[wolf->stats.num_rays++] = second;
	return (TRUE);
}

void			render_binary(t_wolf *wolf, int x1, int x2)
{
	if (x2 < x1)
		return ;
	if (x1 == x2)
	{
		t_ray ray = create_ray(wolf, x1, (t_vec2) { wolf->player.pos.x, wolf->player.pos.y });
		cast_ray(wolf, &ray, INT_MAX);
	} else
	{
		if (double_cast_ray(wolf, x1, x2))
			return ;
		if (x2 - x1 < 2)
			return ;
		int mid = (x1 + x2) / 2;
		render_binary(wolf, x1 + 1, mid - 1);
		render_binary(wolf, mid, x2 - 1);
	}
}	

void			render_main(t_wolf *wolf)
{
	wolf->stats.num_rays = 0;
	render_binary(wolf, 0, S_WIDTH - 1);
}

/*void	render_floor(t_wolf *wolf, t_ray *ray)
{
	int x = ray->x;
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
}*/

t_vec2	get_floor_wall(t_wolf *wolf, t_ray *ray)
{
	float wallX = 0;

	if (ray->side == 0)
		wallX = wolf->player.pos.y + ray->sdist * ray->dir.y;
	else
		wallX = wolf->player.pos.x + ray->sdist * ray->dir.x;
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
	return (floorWall);
}
void	render_floor(t_wolf *wolf, t_ray *from, t_ray *to, t_bool f, int last_floor)
{
	float fromHeight = S_HEIGHT / from->dist;

	int from_world_height = fromHeight * wolf->world.size.z;
	int fromOffset = fromHeight * (wolf->player.pos.z + 1) / 2. - fromHeight;
	int fromTop = S_HEIGHT_2 - from_world_height / 2 + fromOffset - 1;
	int fromBottom = S_HEIGHT_2 + from_world_height / 2 + fromOffset + 1;

	float toHeight = S_HEIGHT / to->dist;
	int to_world_height = toHeight * wolf->world.size.z;
	int toOffset = toHeight * (wolf->player.pos.z + 1) / 2. - toHeight;
	int toTop = S_HEIGHT_2 - to_world_height / 2 + toOffset - 1;
	int toBottom = S_HEIGHT_2 + to_world_height / 2 + toOffset + 1;

	t_vec2 fromFloorWall = get_floor_wall(wolf, from);
	t_vec2 toFloorWall = get_floor_wall(wolf, to);

	float fromDistZ = from->sdist / (wolf->player.pos.z + 1);
	float fromRevDistZ = from->sdist / -(wolf->world.size.z - wolf->player.pos.z + 1);
	float toDistZ = to->sdist / (wolf->player.pos.z + 1);
	float toRevDistZ = to->sdist / -(wolf->world.size.z - wolf->player.pos.z + 1);

	SDL_Surface *floor_tex = wolf->world.floor->surface;
	SDL_Surface *ceil_tex = wolf->world.ceil->surface;

	for (int y = 0; y < S_HEIGHT_2; y++)
	{
		if (f)
		{
			float fromWeight = S_HEIGHT/((2 * (y + fromBottom) - S_HEIGHT) * fromDistZ);
			t_vec2 from_curr_floor = (t_vec2)
			{
				fromWeight * fromFloorWall.x + (1.0 - fromWeight) * wolf->player.pos.x,
				fromWeight * fromFloorWall.y + (1.0 - fromWeight) * wolf->player.pos.y
			};

			float toWeight = S_HEIGHT/((2 * (y + toBottom) - S_HEIGHT) * toDistZ);
			t_vec2 to_curr_floor = (t_vec2)
			{
				toWeight * toFloorWall.x + (1.0 - toWeight) * wolf->player.pos.x,
				toWeight * toFloorWall.y + (1.0 - toWeight) * wolf->player.pos.y
			};

			float toFloorTexX, toFloorTexY;
			toFloorTexX = to_curr_floor.x * floor_tex->w;
			toFloorTexY = to_curr_floor.y * floor_tex->h;

			float fromFloorTexX, fromFloorTexY;
			fromFloorTexX = from_curr_floor.x * floor_tex->w;
			fromFloorTexY = from_curr_floor.y * floor_tex->h;

			float incX = (toFloorTexX - fromFloorTexX)/(to->x == from->x ? 1 : to->x - from->x);
			float incY = (toFloorTexY - fromFloorTexY)/(to->x == from->x ? 1 : to->x - from->x);

			//float bottom = fromBottom;
			float incBottom = (float)(toBottom - fromBottom)/(to->x == from->x ? 1 : to->x - from->x);

			int x = from->x;
			if (fromBottom + y >= last_floor && toBottom + y >= last_floor)
				break ;
			while (x <= to->x)
			{
				int floorTexX = (int)fabs(fromFloorTexX + incX * (x - from->x)) % floor_tex->w;
				int floorTexY = (int)fabs(fromFloorTexY + incY * (x - from->x)) % floor_tex->h;

				float bottom = fromBottom + incBottom * (x - from->x);

				// TODO: Remove zero checking by better last_floor
				if ((int)bottom + y < last_floor && (int)bottom + y < (int)S_HEIGHT && wolf->img->pixels[(((int)bottom + y) * (int)S_WIDTH + x)] == 0)
					wolf->img->pixels[(((int)bottom + y) * (int)S_WIDTH) + x] = getpixel(floor_tex, floorTexX, floorTexY);
				x++;
			}
		}
		if (!f)
		{
			float fromWeight = S_HEIGHT/((2 * (fromTop - y) - S_HEIGHT) * fromRevDistZ);
			t_vec2 from_curr_floor = (t_vec2)
			{
				fromWeight * fromFloorWall.x + (1.0 - fromWeight) * wolf->player.pos.x,
				fromWeight * fromFloorWall.y + (1.0 - fromWeight) * wolf->player.pos.y
			};

			float toWeight = S_HEIGHT/((2 * (toTop - y) - S_HEIGHT) * toRevDistZ);
			t_vec2 to_curr_floor = (t_vec2)
			{
				toWeight * toFloorWall.x + (1.0 - toWeight) * wolf->player.pos.x,
				toWeight * toFloorWall.y + (1.0 - toWeight) * wolf->player.pos.y
			};

			float toFloorTexX, toFloorTexY;
			toFloorTexX = to_curr_floor.x * ceil_tex->w;
			toFloorTexY = to_curr_floor.y * ceil_tex->h;

			float fromFloorTexX, fromFloorTexY;
			fromFloorTexX = from_curr_floor.x * ceil_tex->w;
			fromFloorTexY = from_curr_floor.y * ceil_tex->h;

			float incX = (toFloorTexX - fromFloorTexX)/(to->x == from->x ? 1 : to->x - from->x);
			float incY = (toFloorTexY - fromFloorTexY)/(to->x == from->x ? 1 : to->x - from->x);

			float incTop = (float)(toTop - fromTop)/(to->x == from->x ? 1 : to->x - from->x);

			int x = from->x;
			while (x <= to->x)
			{
				int floorTexX = (int)fabs(fromFloorTexX + incX * (x - from->x)) % ceil_tex->w;
				int floorTexY = (int)fabs(fromFloorTexY + incY * (x - from->x)) % ceil_tex->h;

				float top = fromTop + incTop * (x - from->x);

				if (y <= (int)top && wolf->img->pixels[(((int)top - y) * (int)S_WIDTH + x)] == 0)
					wolf->img->pixels[(((int)top - y) * (int)S_WIDTH) + x] = getpixel(ceil_tex, floorTexX, floorTexY);
				x++;
			}
		}
	}
}
