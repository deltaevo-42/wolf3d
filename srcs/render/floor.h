/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/03 18:56:06 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/03/03 18:56:07 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_FLOOR_H
# define RENDER_FLOOR_H

# include "wolf.h"

enum			e_wall_pos {
	WALL_TOP,
	WALL_BOTTOM
};

typedef struct	s_wall_info {
	t_ray	*r;
	t_vec2	floor_wall;
	int		y;
	float	weight;
}				t_wall_info;

#endif
