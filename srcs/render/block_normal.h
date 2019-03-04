/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_normal.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/03 15:54:10 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/03 18:55:51 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCK_NORMAL_H
# define BLOCK_NORMAL_H

# include "wolf.h"

typedef struct	s_render_info {
	float			tex_x;
	int				y;
	float			height;
	int				last_y;
	t_block_normal	*block;
	int				x;
	t_face			face;
}				t_render_info;

#endif
