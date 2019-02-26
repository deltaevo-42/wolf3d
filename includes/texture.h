/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 14:48:06 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/26 18:28:56 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_H
# define TEXTURE_H

# include "wolf.h"

typedef enum e_texture_type			t_texture_type;
typedef struct s_texture			t_texture;
typedef struct s_texture_animated	t_texture_animated;

enum							e_texture_type
{
	T_NORMAL,
	T_ANIMATED,
	T_NONE
};

struct		s_texture
{
	t_texture_type	type;
	SDL_Surface		*surface;
	t_vec2			size;
};

struct		s_texture_animated
{
	t_texture		super;
	t_vec2			step_count;
	t_vec2			spacer;
	t_vec2			step_size;
	double			delay;
	int				index;
	double			last_seen;
};


/* Textures */
t_texture	*load_json_texture(t_json_object *texture_obj);
t_texture	*load_normal_texture(t_json_object *j_obj);
t_texture	*load_animated_texture(t_json_object *texture_obj);

#endif
