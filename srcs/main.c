/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 17:12:55 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/03 16:15:18 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

int		main(int argc, char **argv)
{
	t_wolf	wolf;

	wolf = (t_wolf) {
		.player = { .rotation = 0.01, .selected_weapon = 0 },
		.dist_to_plane = 1.0,
		.minimap_size = 100,
		.minimap_padding_x = 10, .minimap_padding_y = 10,
		.weapons_texture = {
			.step_count = { 5, 4 }, .step_size = { 64, 64 },
			.spacer = { 1, 1 }, .delay = 50
		},
		.heads_texture = {
			.step_count = { 3, 7 }, .step_size = { 24, 32 },
			.spacer = { 1, 1 }, .delay = 500
		}
	};
	if (!load_config(&wolf, "assets/maps/map_1.json"))
	{
		unload(&wolf);
		ft_putendl("Unable to load world");
		return (1);//FREE
	}
	sdl_init(&wolf);
	game_loop(&wolf);
	unload(&wolf);
	sdl_quit(&wolf);
	return (0);
}
