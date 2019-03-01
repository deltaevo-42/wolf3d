/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_animated.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 15:33:50 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/01 16:32:25 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

t_bool			setup_animated_texture(t_texture_animated *t, char *path)
{
	t->super.type = T_ANIMATED;
	t->super.size = t->step_size;
	if (!(t->super.surface = IMG_Load(path)))
	{
		ft_putstr("Unnable to load t: ");
		ft_putendl(IMG_GetError());
		return (FALSE);
	}
	t->index = 0;
	t->last_seen = 0;
	t->super.surface = SDL_ConvertSurfaceFormat(t->super.surface,
		SDL_PIXELFORMAT_ARGB8888, 0);
	return (TRUE);
}

t_texture		*load_animated_texture(t_json_object *j_obj)
{
	t_texture_animated	*t;
	t_json_string		*j_path;
	t_bool				error;

	if (!(j_path = json_get_string(j_obj, "path")))
		return (NULL);
	if (!(t = (t_texture_animated*)malloc(sizeof(t_texture_animated))))
		return (NULL);
	error = !ft_json_vec2(json_object_get(j_obj, "step_count"), &t->step_count)
		|| !ft_json_vec2(json_object_get(j_obj, "spacer"), &t->spacer)
		|| !ft_json_vec2(json_object_get(j_obj, "step_size"), &t->step_size)
		|| !(t->delay = *json_get_number(j_obj, "delay"));
	if (!setup_animated_texture(t, j_path->value))
	{
		free(t);
		return (NULL);
	}
	return ((t_texture *)t);
}
