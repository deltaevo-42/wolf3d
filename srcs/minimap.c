/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 18:33:21 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/24 16:42:20 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

#define BACKGROUND_COLOR 0x666666
#define BACKGROUND_COLOR_FOV (BACKGROUND_COLOR + 0x002200)

static int			compute_block_minimap(t_wolf *wolf, t_vec2 map)
{
	t_block_state	*state;

	state = wolf->world.data[(int)map.y][(int)map.x];
	if (!state)
		return (-1);
	if (state->block->type == B_NORMAL)
		return (normal_block_minimap(wolf, state));
	if (state->block->type == B_ROUND)
		return (round_block_minimap(wolf, state, map));
	return (-1);
}

static void			render_fov(t_wolf *wolf)
{
	const t_vec2	s = { (float)wolf->minimap_size / wolf->world.size.x,
		 (float)wolf->minimap_size / wolf->world.size.y};
	int 	i;
	t_vec2	pos;

	pos = (t_vec2){
		wolf->player.pos.x * s.x,
		wolf->player.pos.y * s.y};
	i = -1;
	while (++i < S_WIDTH)
		draw_line(wolf->pixels, S_WIDTH, (t_pixel){
			S_WIDTH - wolf->minimap_size - wolf->minimap_padding + pos.x,
			pos.y + wolf->minimap_padding, BACKGROUND_COLOR_FOV},
			(t_pixel){
			S_WIDTH - wolf->minimap_size - wolf->minimap_padding + (pos.x + (wolf->last_rays[i].dir.x * wolf->last_rays[i].dist * s.x)), 
			pos.y + (wolf->last_rays[i].dir.y * wolf->last_rays[i].dist * s.y) + wolf->minimap_padding , 0});
}

void				render_minimap(t_wolf *wolf)
{
	int i;
	int j;
	const t_vec2	s = { wolf->world.size.x / (float)wolf->minimap_size,
		wolf->world.size.y / (float)wolf->minimap_size };
	t_vec2 map;
	int color;

	render_fov(wolf);
	i = -1;
	while (++i < wolf->minimap_size)
	{
		j = -1;
		while (++j < wolf->minimap_size)
		{
			map = (t_vec2){j * s.x, i * s.y};
			int index = ((i + wolf->minimap_padding) * (int)S_WIDTH) + j + (int)S_WIDTH - wolf->minimap_size - wolf->minimap_padding;
			if ((color = compute_block_minimap(wolf, map)) == -1)
			{
				if (wolf->pixels[index] == BACKGROUND_COLOR_FOV)
					continue ;
				color = BACKGROUND_COLOR;
			}
			if (map.x <= wolf->player.pos.x + 0.2 && map.x >= wolf->player.pos.x - 0.2
			&& map.y <= wolf->player.pos.y + 0.2 && map.y >= wolf->player.pos.y - 0.2)
				color = 0xFF0000;
			wolf->pixels[index] = color;
		}
	}
	stroke_rect(wolf->pixels, S_WIDTH, 0, (SDL_Rect) { S_WIDTH - wolf->minimap_size - wolf->minimap_padding, wolf->minimap_padding, wolf->minimap_size, wolf->minimap_size });
	stroke_rect(wolf->pixels, S_WIDTH, 0, (SDL_Rect) { S_WIDTH - wolf->minimap_size - wolf->minimap_padding + 1, wolf->minimap_padding + 1, wolf->minimap_size - 2, wolf->minimap_size - 2 });
}