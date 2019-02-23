/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 18:33:21 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/19 19:04:20 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

#define BACKGROUND_COLOR 0x666666

static int			compute_block_minimap(t_wolf *wolf, t_vec2 map)
{
	t_block_state	*state;

	state = wolf->world.data[(int)map.y][(int)map.x];
	if (!state)
		return (-1);
	if (state->block->type == B_NORMAL)
		return (normal_block_minimap(wolf, state));
	return (-1);
}

static void			render_fov(t_wolf *wolf)
{
	const t_vec2	s = { (float)wolf->minimap_size / wolf->world.size.width,
		 (float)wolf->minimap_size / wolf->world.size.height};
	static uint32_t		pixels[(int)S_WIDTH * (int)S_WIDTH];
	int 	i;
	t_vec2	pos;


	ft_bzero(pixels, wolf->minimap_size * wolf->minimap_size * sizeof(uint32_t));
	pos = (t_vec2){
		wolf->player.pos.x * s.x,
		wolf->player.pos.y * s.y};
	i = -1;
	while (++i < S_WIDTH)
		draw_line(pixels, wolf->minimap_size, (t_pixel){pos.x, pos.y, 1}, (t_pixel){
			pos.x + (wolf->last_rays[i].dir.x * wolf->last_rays[i].dist * s.x), 
			pos.y + (wolf->last_rays[i].dir.y * wolf->last_rays[i].dist * s.y) , 0});
	i = 0;
	while (i < wolf->minimap_size * wolf->minimap_size)
	{
		int pos = (i / wolf->minimap_size + wolf->minimap_padding) * (int)S_WIDTH + (int)S_WIDTH - wolf->minimap_size + (i % wolf->minimap_size) - wolf->minimap_padding;
		if (pixels[i] && wolf->pixels[pos] == BACKGROUND_COLOR)
			wolf->pixels[pos] = BACKGROUND_COLOR + 0x002200;
		i++;
	}
}

void				render_minimap(t_wolf *wolf)
{
	int i;
	int j;
	const t_vec2	s = { wolf->world.size.width / (float)wolf->minimap_size,
		wolf->world.size.height / (float)wolf->minimap_size };
	t_vec2 map;
	int color;

	i = -1;
	while (++i < wolf->minimap_size)
	{
		j = -1;
		while (++j < wolf->minimap_size)
		{
			map = (t_vec2){j * s.x, i * s.y};
			if ((color = compute_block_minimap(wolf, map)) == -1)
				color = BACKGROUND_COLOR;
			if (map.x <= wolf->player.pos.x + 0.2 && map.x >= wolf->player.pos.x - 0.2
			&& map.y <= wolf->player.pos.y + 0.2 && map.y >= wolf->player.pos.y - 0.2)
				color = 0xFF0000;
			wolf->pixels[((i + wolf->minimap_padding) * (int)S_WIDTH) + j + (int)S_WIDTH - wolf->minimap_size - wolf->minimap_padding] = color;
		}
	}
	render_fov(wolf);
	stroke_rect(wolf->pixels, S_WIDTH, 0, (SDL_Rect) { S_WIDTH - wolf->minimap_size - wolf->minimap_padding, wolf->minimap_padding, wolf->minimap_size, wolf->minimap_size });
	stroke_rect(wolf->pixels, S_WIDTH, 0, (SDL_Rect) { S_WIDTH - wolf->minimap_size - wolf->minimap_padding + 1, wolf->minimap_padding + 1, wolf->minimap_size - 2, wolf->minimap_size - 2 });
}