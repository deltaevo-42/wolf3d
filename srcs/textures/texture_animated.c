/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_animated.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 15:33:50 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/26 18:27:56 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

t_texture		*load_animated_texture(t_json_object *j_obj)
{
	t_texture_animated	*texture;
	t_json_string		*j_path;
	t_bool				error;

	if (!(j_path = json_get_string(j_obj, "path")))
		return (NULL);
	if(!(texture = (t_texture_animated*)malloc(sizeof(t_texture_animated))))
		return (NULL);
	error = !ft_json_vec2(json_object_get(j_obj, "step_count"), &texture->step_count)
	 || !ft_json_vec2(json_object_get(j_obj, "spacer"), &texture->spacer)
	 || !ft_json_vec2(json_object_get(j_obj, "step_size"), &texture->step_size)
	 || !(texture->delay = *json_get_number(j_obj, "delay"));
	texture->super.size = texture->step_size;
	texture->super.type = T_ANIMATED;
	if (error || !(texture->super.surface = IMG_Load(j_path->value)))
	{
		ft_putstr("Unnable to load texture: ");
		ft_putendl(IMG_GetError());
		free(texture);
		return (NULL);
	}
	texture->index = 0;
	texture->last_seen = 0;
	return ((t_texture *)texture);
}
