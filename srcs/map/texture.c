/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 01:49:10 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/01 17:59:08 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

static t_texture_type	get_texture_type(char *string)
{
	const int	len = ft_strlen(string);

	if (len == 6 && ft_strncmp(string, "NORMAL", 6) == 0)
		return (T_NORMAL);
	if (len == 8 && ft_strncmp(string, "ANIMATED", 8) == 0)
		return (T_ANIMATED);
	return (T_NONE);
}

t_texture				*load_json_texture(t_json_object *texture_obj)
{
	t_json_string	*j_string;
	t_texture_type	texture_type;

	if (!(j_string = json_get_string(texture_obj, "type")))
		return (NULL);
	texture_type = get_texture_type(j_string->value);
	if (texture_type == T_NONE)
	{
		ft_putstr("Unknown texture type: ");
		ft_putendl(j_string->value);
		return (NULL);
	}
	if (texture_type == T_NORMAL)
		return (load_normal_texture(texture_obj));
	else if (texture_type == T_ANIMATED)
		return (load_animated_texture(texture_obj));
	else
		return (NULL);
}
