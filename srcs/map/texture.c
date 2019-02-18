/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 01:49:10 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/18 03:27:14 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

static t_texture_type	get_texture_type(char *string)
{
	if (ft_strncmp(string, "NORMAL", 6) == 0)
		return (NORMAL);
	return (NONE);
}

static t_texture		*load_normal_texture(t_json_object *j_obj)
{
	t_texture_normal	*texture;
	t_json_value		*val;

	val = json_object_get(j_obj, "path");
	if ((!val || val->type != JSON_STRING)
		|| !(texture = (t_texture_normal*)malloc(sizeof(t_texture_normal))))
		return (NULL);
	texture->super.type = NORMAL;
	if (!(texture->surface = IMG_Load(((t_json_string *)val)->value)))
	{
		ft_putstr("Unnable to load texture: ");
		ft_putendl(IMG_GetError());
		free(texture);
		return (NULL);
	}
	return ((t_texture *)texture);
}

t_texture				*load_json_texture(t_json_object *texture_obj)
{
	t_json_value	*val;
	t_json_string	*j_string;
	t_texture_type	texture_type;

	val = json_object_get(texture_obj, "type");
	if (!val || val->type != JSON_STRING)
		return (NULL);
	j_string = ((t_json_string *)val);
	texture_type = get_texture_type(j_string->value);
	if (texture_type == NONE)
	{
		ft_putstr("Unknown texture type: ");
		ft_putendl(j_string->value);
		return (NULL);
	}
	if (texture_type == NORMAL)
		return (load_normal_texture(texture_obj));
	return (NULL);
}