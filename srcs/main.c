/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 17:12:55 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/04 15:08:13 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

static int		start(t_wolf *wolf, char *map)
{
	if (!load_config(wolf, map))
	{
		unload(wolf);
		ft_putendl("Unable to load world");
		return (1);
	}
	if (!init_defaults(wolf))
	{
		unload(wolf);
		ft_putendl("Unable to load default settings");
		return (1);
	}
	sdl_init(wolf);
	game_loop(wolf);
	unload(wolf);
	sdl_quit(wolf);
	return (0);
}

int				main(int argc, char **argv)
{
	t_wolf	wolf;

	if (argc != 2)
	{
		ft_putendl("Usage: ./wolf3d [map.json]");
		return (1);
	}
	wolf = (t_wolf) {
		.player = { .selected_weapon = 0 },
		.dist_to_plane = 1.0,
		.minimap_size = 100,
		.minimap_padding_x = 10, .minimap_padding_y = 10,
		.weapons_texture = {
			.step_count = { 5, 4 }, .step_size = { 64, 64 },
			.spacer = { 1, 1 }, .delay = 50},
		.heads_texture = {
			.step_count = { 3, 7 }, .step_size = { 24, 32 },
			.spacer = { 1, 1 }, .delay = 500}
	};
	return (start(&wolf, argv[1]));
}
