/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 18:33:21 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/24 23:58:10 by llelievr         ###   ########.fr       */
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
	uint32_t 	i;
	t_vec2	pos;

	i = -1;
	while (++i < wolf->stats.num_rays)
	{
		pos = (t_vec2){
		wolf->last_rays[i].start.x * s.x,
		wolf->last_rays[i].start.y * s.y};
		float dist = wolf->last_rays[i].dist - wolf->last_rays[i].extra_dist;
		draw_line(wolf->img, (t_pixel){
		S_WIDTH - wolf->minimap_size - wolf->minimap_padding_x + pos.x,
		pos.y + wolf->minimap_padding_y, BACKGROUND_COLOR_FOV},
		(t_pixel){
			S_WIDTH - wolf->minimap_size - wolf->minimap_padding_x + (pos.x + (wolf->last_rays[i].dir.x * dist * s.x)),
			pos.y + (wolf->last_rays[i].dir.y * dist * s.y) + wolf->minimap_padding_y , 0});
	}
}

void				render_minimap(t_wolf *wolf)
{
	uint32_t	i;
	uint32_t	j;
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
			int index = ((i + wolf->minimap_padding_y) * (int)S_WIDTH) + j + (int)S_WIDTH - wolf->minimap_size - wolf->minimap_padding_x;
			if ((color = compute_block_minimap(wolf, map)) == -1)
				color = wolf->img->pixels[index] == BACKGROUND_COLOR_FOV ? BACKGROUND_COLOR_FOV : BACKGROUND_COLOR;
			if (map.x <= wolf->player.pos.x + 0.2 && map.x >= wolf->player.pos.x - 0.2
			&& map.y <= wolf->player.pos.y + 0.2 && map.y >= wolf->player.pos.y - 0.2)
				color = 0xFF0000;
			wolf->img->pixels[index] = color;
		}
	}
	stroke_rect(wolf->img, 0, (SDL_Rect) { S_WIDTH - wolf->minimap_size - wolf->minimap_padding_x, wolf->minimap_padding_y, wolf->minimap_size, wolf->minimap_size });
	stroke_rect(wolf->img, 0, (SDL_Rect) { S_WIDTH - wolf->minimap_size - wolf->minimap_padding_x + 1, wolf->minimap_padding_y + 1, wolf->minimap_size - 2, wolf->minimap_size - 2 });
}