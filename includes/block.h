/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 14:46:04 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/19 19:46:40 by llelievr         ###   ########.fr       */
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
typedef struct s_block_state	t_block_state;

enum							e_face
{
	F_NORTH,
	F_SOUTH,
	F_EAST,
	F_WEST,
	F_TOP,
	F_BOTTOM,
	F_ALL,
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


struct							s_block_state
{
	t_block_type	type;
	t_block			*block;
};

t_block				*load_json_block(t_world *world, t_json_object *obj);
t_block_state		***load_map_data(t_world *w, t_json_value *val);

t_block				*load_normal_block(t_world *w, t_json_object *obj);
int					normal_block_minimap(t_wolf *wolf, t_block_state *state);
t_face 				get_face_type(char *string);

#endif
