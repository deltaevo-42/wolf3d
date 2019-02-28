/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 17:12:55 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/19 22:15:45 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

int		main(int argc, char **argv)
{
	t_wolf	wolf;

	wolf = (t_wolf) {
		.player = {
			.rotation = 0.01
		},
		.dist_to_plane = 1.0,
		.minimap_size = 100,
		.minimap_padding_x = 10,
		.minimap_padding_y = 10
	};
	load_world(&wolf.world, "assets/maps/map_1.json");
	sdl_init(&wolf);
	game_loop(&wolf);
	sdl_quit(&wolf);
	return (0);
}
