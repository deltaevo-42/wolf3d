/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_animated.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 15:33:50 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/01 00:39:58 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

t_bool			setup_animated_texture(t_texture_animated *texture, char *path)
{
	texture->super.type = T_ANIMATED;
	texture->super.size = texture->step_size;
	if (!(texture->super.surface = IMG_Load(path)))
	{
		ft_putstr("Unnable to load texture: ");
		ft_putendl(IMG_GetError());
		return (FALSE);
	}
	texture->index = 0;
	texture->last_seen = 0;
	texture->super.surface = SDL_ConvertSurfaceFormat(texture->super.surface, SDL_PIXELFORMAT_ARGB8888, 0);
	return (TRUE);
}

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
	if (!setup_animated_texture(texture, j_path->value))
	{
		free(texture);
		return (NULL);
	}
	return ((t_texture *)texture);
}
