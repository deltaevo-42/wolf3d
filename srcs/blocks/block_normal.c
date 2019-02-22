/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_normal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 17:41:56 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/22 15:12:50 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"


static t_bool load_block_side(t_world *w, t_json_object *o, t_block_side *side)
{
	t_json_value	*val;
	int				texture_i;
	
	if (!ft_json_color(json_object_get(o, "color"), &side->color))
		return (FALSE);
	if (!(val = json_object_get(o, "texture")) || val->type != JSON_NUMBER)
		return (FALSE);
	texture_i = (int)((t_json_number *)val)->value;
	if (texture_i >= w->textures_count)
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
		while (i <= (int)face - (face == F_ALL) || i == 0)
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

t_block		*load_normal_block(t_world *w, t_json_object *obj)
{
	t_block_normal	*block;
	t_json_value	*val;

	if (!(block = (t_block_normal *)malloc(sizeof(t_block_normal))))
		return (NULL);
	block->super.type = B_NORMAL;
	if (!ft_json_color(json_object_get(obj, "minimap_color"), &block->minimap_color))
		return (json_free_ret(block));
	val = json_object_get(obj, "height");
	block->super.height = (!val || val->type != JSON_NUMBER ? 1 : ((t_json_number *)val)->value);
	if (!load_block_sides(w, json_object_get(obj, "sides"), block->faces))
		return (json_free_ret(block));
	return ((t_block *)block);
}

int			normal_block_minimap(t_wolf *wolf, t_block_state *state)
{
	(void)wolf;
	return (ft_color_i(((t_block_normal *)state->block)->minimap_color));
}
