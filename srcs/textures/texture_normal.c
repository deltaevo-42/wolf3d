/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_normal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 17:43:28 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/26 17:58:56 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

t_texture		*load_normal_texture(t_json_object *j_obj)
{
	t_texture		*texture;
	t_json_value	*val;

	val = json_object_get(j_obj, "path");
	if ((!val || val->type != JSON_STRING)
		|| !(texture = (t_texture*)malloc(sizeof(t_texture))))
		return (NULL);
	texture->type = T_NORMAL;
	if (!(texture->surface = IMG_Load(((t_json_string *)val)->value)))
	{
		ft_putstr("Unnable to load texture: ");
		ft_putendl(IMG_GetError());
		free(texture);
		return (NULL);
	}
	texture->size = (t_vec2){ texture->surface->w, texture->surface->h};
	return ((t_texture *)texture);
}
