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

t_bool			render_wall(t_wolf *wolf, t_ray *from, t_ray *to)
{
	if (from->hit->block->type == B_NORMAL)
		return (render_block_normal_wall(wolf, from, to));
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


void			render_floor(t_wolf *wolf, t_ray *from, t_ray *to, t_bool f);

#include <assert.h>

void			cast_ray(t_wolf *wolf, t_ray *ray)
{
	while (1)
	{
		if (ray->hit)
		{
			render_wall(wolf, ray, ray);
			if (ray->hit->block->height == wolf->world.size.z
				|| (ray->dist <= 1 && wolf->player.pos.z + 1 <= ray->hit->block->height && wolf->player.pos.z >= 0.2))
				break ;
			t_block_state *hit = ray->hit;
			float h = S_HEIGHT / ray->dist;
			int p = S_HEIGHT_2 + h * (wolf->player.pos.z + 1) * 0.5 - h * hit->block->height;
			if (p <= 0 && p + h > S_HEIGHT)
				break;
			next_ray(wolf, ray);
			render_top(wolf, ray, hit, p);
			continue ;
		}
		if (!next_ray(wolf, ray))
			break ;
	}
	//wolf->last_rays[x] = ray;
	render_floor(wolf, ray, ray, TRUE);
}

t_bool			double_cast_ray(t_wolf *wolf, int x1, int x2)
{
	t_ray first = create_ray(wolf, x1);
	t_ray second = create_ray(wolf, x2);

	while (1)
	{
		t_bool	next_first = next_ray(wolf, &first);
		t_bool	next_second = next_ray(wolf, &second);

		if (!next_first && !next_second)
			break ;
		if (next_first != next_second)
		{
			cast_ray(wolf, &first);
			cast_ray(wolf, &second);
			return (FALSE);
		}
		if ((first.fhit && first.fhit->block->type != B_NORMAL)
			|| (second.fhit && second.fhit->block->type != B_NORMAL))
		{
			cast_ray(wolf, &first);
			cast_ray(wolf, &second);
			return (FALSE);
		}
		if (first.hit || second.hit)
		{
			if (first.hit != second.hit || first.face != second.face)
			{
				cast_ray(wolf, &first);
				cast_ray(wolf, &second);
				return (FALSE);
			}
			render_wall(wolf, &first, &second);
			if (first.hit->block->height == wolf->world.size.z
				|| (first.dist <= 1 && wolf->player.pos.z + 1 <= first.hit->block->height && wolf->player.pos.z >= 0.2))
				break ;
			t_block_state *hit = first.hit;
			float h = S_HEIGHT / first.dist;
			int p = S_HEIGHT_2 + h * (wolf->player.pos.z + 1) * 0.5 - h * hit->block->height;
			if (p <= 0 && p + h > S_HEIGHT)
				break ;
			/*next_ray(wolf, &first);
			next_ray(wolf, &second);
			render_top(wolf, &first, hit, p);*/
		}
	}
	render_floor(wolf, &first, &second, TRUE);
	/*wolf->last_rays[x] = ray;*/
	return (TRUE);
}

void			render_binary(t_wolf *wolf, int x1, int x2)
{
	if (x2 < x1)
		return ;
	if (x1 == x2)
	{
		wolf->stats.num_rays += 1;
		t_ray ray = create_ray(wolf, x1);
		cast_ray(wolf, &ray);
	} else
	{
		wolf->stats.num_rays += 2;
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
	return (floorWall);
}
void	render_floor(t_wolf *wolf, t_ray *from, t_ray *to, t_bool f)
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

	float fromDistZ = from->dist / (wolf->player.pos.z + 1);
	float fromRevDistZ = from->dist / -(wolf->world.size.z - wolf->player.pos.z + 1);
	float toDistZ = to->dist / (wolf->player.pos.z + 1);
	float toRevDistZ = to->dist / -(wolf->world.size.z - wolf->player.pos.z + 1);

	SDL_Surface *floor_tex = wolf->world.floor->surface;
	SDL_Surface *ceil_tex = wolf->world.ceil->surface;

	for (int y = 0; y < S_HEIGHT_2; y++)
	{
		// Block cado
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

			int x = from->x;
			while (x <= to->x)
			{
				float progress = (float)(x - from->x) / (float)(to->x == from->x ? 1 : to->x - from->x);
				t_vec2 curr_floor = (t_vec2)
				{
					from_curr_floor.x + (to_curr_floor.x - from_curr_floor.x) * progress,
					from_curr_floor.y + (to_curr_floor.y - from_curr_floor.y) * progress
				};
				int floorTexX, floorTexY;
				floorTexX = ft_abs((int)(curr_floor.x * floor_tex->w)) % floor_tex->w;
				floorTexY = ft_abs((int)(curr_floor.y * floor_tex->h)) % floor_tex->h;

				int bottom = fromBottom + (toBottom - fromBottom) * progress;
				if (bottom + y < S_HEIGHT && wolf->img->pixels[((bottom + y) * (int)S_WIDTH + x)] == 0)
					wolf->img->pixels[((bottom + y) * (int)S_WIDTH) + x] = getpixel(floor_tex, floorTexX, floorTexY);
				x++;
			}
		}
		// Block cado
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

			int x = from->x;
			while (x <= to->x)
			{
				float progress = (float)(x - from->x) / (float)(to->x == from->x ? 1 : to->x - from->x);
				t_vec2 curr_floor = (t_vec2)
				{
					from_curr_floor.x + (to_curr_floor.x - from_curr_floor.x) * progress,
					from_curr_floor.y + (to_curr_floor.y - from_curr_floor.y) * progress
				};
				int floorTexX, floorTexY;
				floorTexX = ft_abs((int)(curr_floor.x * ceil_tex->w)) % ceil_tex->w;
				floorTexY = ft_abs((int)(curr_floor.y * ceil_tex->h)) % ceil_tex->h;

				int top = fromTop + (toTop - fromTop) * progress;
				if (y <= top && wolf->img->pixels[((top - y) * (int)S_WIDTH + x)] == 0)
					wolf->img->pixels[((top - y) * (int)S_WIDTH) + x] = getpixel(ceil_tex, floorTexX, floorTexY);
				x++;
			}
		}
	}
}
