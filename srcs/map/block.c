/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 14:32:51 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/18 18:31:05 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

static t_block_type	get_block_type(char *string)
{
	if (ft_strncmp(string, "NORMAL", 6) == 0)
		return (B_NORMAL);
	return (B_NONE);
}

static t_face get_face_type(char *string)
{
	if (ft_strncmp(string, "NORTH", 5) == 0)
		return (F_NORTH);
	if (ft_strncmp(string, "SOUTH", 5) == 0)
		return (F_SOUTH);
	if (ft_strncmp(string, "WEST", 4) == 0)
		return (F_WEST);
	if (ft_strncmp(string, "EAST", 4) == 0)
		return (F_EAST);
	if (ft_strncmp(string, "TOP", 3) == 0)
		return (F_TOP);
	if (ft_strncmp(string, "BOTTOM", 6) == 0)
		return (F_BOTTOM);
	return (F_NONE);
}

static t_bool load_block_sides(t_world *w, t_json_object *obj, t_block *block)
{
	t_json_value	*val;
	t_json_element	*elem;
	t_face			face;
	t_block_side	*side;

	if (!obj || obj->type != JSON_OBJECT)
		return (FALSE);
	elem = obj->elements;
	ft_memset(block->faces, F_NONE, 6);
	while (elem)
	{
		face = get_face_type(((t_json_object *)elem->value)->string->value);
		if (elem->type != JSON_OBJECT || face == F_NONE)
			return (FALSE);
		side = &block->faces[face];
		if (!ft_json_color(json_object_get(obj, "color"), &side->color))
			return (FALSE);
		val = json_object_get("texture")
		if (!val || val->type != JSON_NUMBER)
			return (FALSE);
		side->texture = w->textures[((t_json_number)val)->value];
		elem = elem->next;
	}
}

static t_block		*load_normal_block(t_world *w, t_json_object *obj)
{
	t_block_normal	*block;

	if (!(block = (t_block_normal *)malloc(sizeof(t_block_normal))))
		return (NULL);
	block->super.type = B_NORMAL;
	if (!ft_json_color(json_object_get(obj, "minimap_color"), &block->minimap_color))
		return (json_free_ret(block));
	if (!load_block_sides(w, json_object_get(obj, "sides"), block))
		return (json_free_ret(block));
	return ((t_block *)block);
}

t_block				*load_json_block(t_world *world, t_json_object *obj)
{
	t_json_value	*val;
	t_json_string	*j_string;
	t_block_type	block_type;

	val = json_object_get(obj, "type");
	if (!val || val->type != JSON_STRING)
		return (NULL);
	j_string = (t_json_string *)val;
	block_type = get_block_type(j_string->value);
	if (block_type == B_NONE)
	{
		ft_putstr("Unknown block type: ");
		ft_putendl(j_string->value);
		return (NULL);
	}
	if (block_type == B_NORMAL)
		return (load_normal_block(world, obj));
	return (NULL);
}
