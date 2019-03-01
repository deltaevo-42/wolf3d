/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_round.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 17:00:00 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/01 18:38:43 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

t_block		*load_round_block(t_world *w, t_json_object *obj)
{
	t_block_round	*b;
	t_json_value	*val;
	double			*texture_i;

	if (!(b = (t_block_round *)malloc(sizeof(t_block_round))))
		return (NULL);
	b->super.type = B_ROUND;
	if (!ft_json_color(json_object_get(obj, "minimap_color"),
		&b->minimap_color))
		return (json_free_ret(b));
	val = json_object_get(obj, "height");
	b->super.height = (!val || val->type != JSON_NUMBER ? 1
		: ((t_json_number *)val)->value);
	val = json_object_get(obj, "radius");
	b->radius = (!val || val->type != JSON_NUMBER ? 0.5
		: ((t_json_number *)val)->value);
	if (!(texture_i = json_get_number(obj, "texture")) 
		|| *texture_i < 0 || *texture_i >= w->textures_count)
		return (NULL);
	b->texture = w->textures[(int)*texture_i];
	return ((t_block *)b);
}

int			round_block_minimap(t_wolf *wolf, t_block_state *state, t_vec2 map)
{
	t_block_round	*b;
	const float		x = map.x - (int)map.x - 0.5;
	const float		y = map.y - (int)map.y - 0.5;

	b = (t_block_round *)state->block;
	if (x * x + y * y < b->radius * b->radius)
		return (ft_color_i(b->minimap_color));
	else
		return (-1);
}
