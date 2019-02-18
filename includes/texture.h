/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 14:48:06 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/18 14:52:26 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_H
# define TEXTURE_H

# include "wolf.h"

typedef enum e_texture_type		t_texture_type;
typedef struct s_texture		t_texture;
typedef struct s_texture_normal	t_texture_normal;

enum							e_texture_type
{
	T_NORMAL,
	T_NONE
};

struct		s_texture
{
	t_texture_type	type;
};

struct		s_texture_normal
{
	t_texture	super;
	SDL_Surface	*surface;
};

/* Textures */
t_texture	*load_json_texture(t_json_object *texture_obj);

#endif
