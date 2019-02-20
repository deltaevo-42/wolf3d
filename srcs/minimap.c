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
	const t_vec2	s = { 200. / wolf->world.size.width,
		 200. / wolf->world.size.height};
	int 	i;
	t_vec2	pos;
	
	pos = (t_vec2){
		wolf->player.pos.x * s.x + S_WIDTH - 200, 
		wolf->player.pos.y * s.y};
	i = -1;
	while (++i < S_WIDTH)
		draw_line(wolf, (t_pixel){pos.x, pos.y, 0xFF00FF}, (t_pixel){
			pos.x + (wolf->last_rays[i].dir.x * wolf->last_rays[i].dist * s.x), 
			pos.y + (wolf->last_rays[i].dir.y * wolf->last_rays[i].dist * s.y) , 0x666666});
}

void				render_minimap(t_wolf *wolf)
{
	int i;
	int j;
	const t_vec2	s = { wolf->world.size.width / 200.,
		wolf->world.size.height / 200.};
	t_vec2 map;
	int color;

	i = -1;
	while (++i < 200)
	{
		j = -1;
		while (++j < 200)
		{
			map = (t_vec2){j * s.x, i * s.y};
			if ((color = compute_block_minimap(wolf, map)) == -1)
				color = 0x666666;
			if (map.x <= wolf->player.pos.x + 0.2 && map.x >= wolf->player.pos.x - 0.2
			&& map.y <= wolf->player.pos.y + 0.2 && map.y >= wolf->player.pos.y - 0.2)
				color = 0xFF0000;
			((unsigned int *)wolf->pixels)[(i * (int)S_WIDTH) + j + (int)S_WIDTH - 200 ] = color;
		}
	}
	render_fov(wolf);
}