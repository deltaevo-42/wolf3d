/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_normal.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/03 15:54:10 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/03 16:05:58 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_FLOOR_H
# define RENDER_FLOOR_H

# include "wolf.h"

typedef struct s_render_info {
	int				tex_x;
	int				y;
	float			height;
	int				last_y;
	t_block_normal	*block;
	int				x;
	t_face			face;
}				t_render_info;

#endif
