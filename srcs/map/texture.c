/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 01:49:10 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/19 17:43:56 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

static t_texture_type	get_texture_type(char *string)
{
	const int	len = ft_strlen(string);	

	if (len == 6 && ft_strncmp(string, "NORMAL", 6) == 0)
		return (T_NORMAL);
	return (T_NONE);
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
	if (texture_type == T_NONE)
	{
		ft_putstr("Unknown texture type: ");
		ft_putendl(j_string->value);
		return (NULL);
	}
	if (texture_type == T_NORMAL)
		return (load_normal_texture(texture_obj));
	return (NULL);
}
