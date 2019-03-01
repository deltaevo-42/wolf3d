/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 14:46:04 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/24 17:08:25 by llelievr         ###   ########.fr       */
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
typedef struct s_block_round	t_block_round;
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
	B_ROUND,
	B_PORTAL,
	B_NONE
};

struct							s_block_side
{
	t_texture		*texture;
	t_color			color;
};

struct							s_block_state
{
	t_block_type	type;
	t_block			*block;
	t_pixel			portal_to;
};

struct							s_block
{
	t_block_type	type;
	float			height;
};

struct 							s_block_normal
{
	t_block			super;
	t_color			minimap_color;
	t_block_side	faces[6];
};

struct							s_block_round
{
	t_block			super;
	t_color			minimap_color;
	float			radius;
	t_texture		*texture;
};

t_block				*load_json_block(t_world *world, t_json_object *obj);
t_block_state		***load_map_data(t_world *w, t_json_value *val);

t_block				*load_normal_block(t_world *w, t_json_object *obj);
t_block				*load_round_block(t_world *w, t_json_object *obj);
int					normal_block_minimap(t_wolf *wolf, t_block_state *state);
int					round_block_minimap(t_wolf *wolf, t_block_state *state, t_vec2 map);
t_bool				render_block_normal_wall(t_wolf *wolf, t_ray *from, t_ray *to, int last_y);
t_bool				render_block_round_wall(t_wolf *wolf, t_ray *ray);
t_bool				render_block_normal_top(t_wolf *wolf, t_ray *ray, t_block_state *hit, int p);
t_bool				render_block_round_top(t_wolf *wolf, t_ray *ray, t_block_state *hit, int p);
t_face 				get_face_type(char *string);

#endif
