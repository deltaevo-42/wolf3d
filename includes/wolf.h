/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 11:58:09 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/12 15:09:15 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF_H
# define WOLF_H

#include <SDL.h>
#include "libft.h"
#include <stdlib.h>

typedef enum	e_bool
{
	TRUE = 1,
	FALSE = 0
}				t_bool;

typedef struct	s_m_op
{
	char	*prefix;
	int		seen;
	void	(*op)();
}				t_m_op;

typedef struct	s_face
{
	t_vec3		p1;
	t_vec3		p2;
	t_vec3		p3;
}				t_face;

typedef struct	s_obj
{
	t_vec2		scale;
	t_face		*faces;
}				t_obj;

typedef struct	s_map
{
	t_size		size;
	t_obj		*objs;
	int			*map;
}				t_map;

typedef struct	s_inst
{
	t_map		map;
}				t_inst;

t_map		*read_map(t_inst *inst, char *file);

#endif
