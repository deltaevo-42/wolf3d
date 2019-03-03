/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 12:29:48 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/03 13:45:01 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

t_block_state		*create_block_state(t_wolf *wolf, int block_id)
{
	t_block_state	*state;

	if (block_id >= wolf->blocks_count || block_id < -1)
	{
		ft_putstr("Block non declared used: id -> ");
		ft_putnbr(block_id + 1);
		ft_putchar('\n');
		return (NULL);
	}
	if (!(state = (t_block_state *)malloc(sizeof(t_block_state))))
		return (NULL);
	if (block_id == 6)
	{
		state->type = B_PORTAL;
		state->portal_to = (t_pixel) { 3, 3, 0 };
	}
	else
		state->type = B_NORMAL;
	state->block = wolf->blocks[block_id];
	return (state);
}

t_bool				load_map_cols(t_wolf *wolf, t_world *w, int i,
	t_json_element *col_elem, t_block_state ***s)
{
	int		j;
	int		block_id;

	if (!(s[i] = (t_block_state **)malloc(sizeof(t_block_side *) * w->size.x)))
		return (FALSE);
	j = 0;
	while (col_elem)
	{
		if (col_elem->value->type != JSON_NUMBER)
			return (FALSE);
		block_id = (int)((t_json_number *)col_elem->value)->value - 1;
		if (block_id == -1)
			s[i][j++] = NULL;
		else if (!(s[i][j++] = create_block_state(wolf, block_id)))
			return (FALSE);
		col_elem = col_elem->next;
	}
	return (TRUE);
}

t_block_state		***load_map_data(t_wolf *wolf, t_world *w,
	t_json_value *val)
{
	t_json_array	*arr;
	t_json_element	*row_elem;
	t_json_element	*col_elem;
	t_block_state	***states;
	int				i;

	if (!(arr = json_to_array(val)) || arr->elems_count != w->size.y ||
	!(states = (t_block_state ***)malloc(sizeof(t_block_state **) * w->size.y)))
		return (NULL);
	row_elem = arr->elements;
	i = 0;
	while (row_elem)
	{
		if (row_elem->value->type != JSON_ARRAY ||
			((t_json_array *)row_elem->value)->elems_count != w->size.x)
			return (NULL);
		col_elem = ((t_json_array *)row_elem->value)->elements;
		if (!load_map_cols(wolf, w, i++, col_elem, states))
			return (NULL);
		row_elem = row_elem->next;
	}
	return (states);
}


t_bool		load_map(t_wolf *wolf, t_world *world, t_json_object *obj)
{
	double	*texture_i;

	if (!obj)
		return (FALSE);
	if (!ft_json_vec3(json_object_get(obj, "size"), &world->size))
		return (FALSE);
	printf("Size %f %f\n", world->size.x, world->size.y);
	if (!(world->data = load_map_data(wolf, world, json_object_get(obj, "data"))))
		return (FALSE);
	if (!(texture_i = json_get_number(obj, "ceil_texture"))
		|| *texture_i < 0 || *texture_i >= wolf->textures_count)
		return (FALSE);
	world->ceil = wolf->textures[(int)*texture_i];
	if (!(texture_i = json_get_number(obj, "floor_texture"))
		|| *texture_i < 0 || *texture_i >= wolf->textures_count)
		return (FALSE);
	world->floor = wolf->textures[(int)*texture_i];
	return (TRUE);
}
