/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 14:32:51 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/19 01:34:03 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

static t_block_type	get_block_type(char *string)
{
	const int	len = ft_strlen(string);	

	if (len == 6 && ft_strncmp(string, "NORMAL", 6) == 0)
		return (B_NORMAL);
	return (B_NONE);
}

static t_face get_face_type(char *string)
{
	const int	len = ft_strlen(string);	

	if (len == 5 && ft_strncmp(string, "NORTH", 5) == 0)
		return (F_NORTH);
	if (len == 5 && ft_strncmp(string, "SOUTH", 5) == 0)
		return (F_SOUTH);
	if (len == 4 && ft_strncmp(string, "WEST", 4) == 0)
		return (F_WEST);
	if (len == 4 && ft_strncmp(string, "EAST", 4) == 0)
		return (F_EAST);
	if (len == 3 && ft_strncmp(string, "TOP", 3) == 0)
		return (F_TOP);
	if (len == 6 && ft_strncmp(string, "BOTTOM", 6) == 0)
		return (F_BOTTOM);
	if (len == 3 && ft_strncmp(string, "ALL", 3) == 0)
		return (F_ALL);
	return (F_NONE);
}

static t_bool load_block_side(t_world *w, t_json_object *o, t_block_side *side)
{
	t_json_value	*val;
	int				texture_i;
	
	if (!ft_json_color(json_object_get(o, "color"), &side->color))
		return (FALSE);
	if (!(val = json_object_get(o, "texture")) || val->type != JSON_NUMBER)
		return (FALSE);
	if (!!(texture_i = (int)((t_json_number *)val)->value) 
		&& texture_i >= w->textures_count)
		return (FALSE);
	side->texture = w->textures[texture_i];
	return (TRUE);
}

static t_bool load_block_sides(t_world *w, t_json_value *v, t_block_side *faces)
{
	t_json_object	*obj;
	t_json_member	*elem;
	t_face			face;
	int				i;

	if (!v || v->type != JSON_OBJECT)
		return (FALSE);
	obj = (t_json_object *)v;
	elem = obj->elements;
	ft_memset(faces, 0, sizeof(t_block_side) * 6);
	while (elem && !!(obj = (t_json_object *)elem->value))
	{
		if ((face = get_face_type(elem->string->value)) == F_NONE)
			return (FALSE);
		i = (face == F_ALL ? 0 : face);
		while (i < (int)face || i == 0)
			if(!load_block_side(w, obj, &faces[i++]))
				return (FALSE);
		elem = elem->next;
	}
	i = -1;
	while (++i < 6)
		if (!faces[i].texture)
			return (FALSE);
	return (TRUE);
}

static t_block		*load_normal_block(t_world *w, t_json_object *obj)
{
	t_block_normal	*block;

	if (!(block = (t_block_normal *)malloc(sizeof(t_block_normal))))
		return (NULL);
	block->super.type = B_NORMAL;
	if (!ft_json_color(json_object_get(obj, "minimap_color"), &block->minimap_color))
		return (json_free_ret(block));
	if (!load_block_sides(w, json_object_get(obj, "sides"), block->faces))
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
