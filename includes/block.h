/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 14:46:04 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/18 17:25:52 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCK_H
# define BLOCK_H

# include "wolf.h"

typedef enum e_block_type		t_block_type;
typedef enum e_face				t_face;

typedef struct s_block			t_block;
typedef struct s_block_side		t_block_side;
typedef struct s_block_normal	t_block_normal;

enum							e_face
{
	F_NORTH,
	F_SOUTH,
	F_EAST,
	F_WEST,
	F_TOP,
	F_BOTTOM,
	F_NONE
};

enum							e_block_type
{
	B_NORMAL,
	B_NONE
};

struct							s_block_side
{
	t_texture		*texture;
	t_color			color;
};

struct							s_block
{
	t_block_type	type;
};



struct 							s_block_normal
{
	t_block			super;
	t_color			minimap_color;
	t_block_side	faces[6];

};

t_block				*load_json_block(t_json_object *obj);

#endif
