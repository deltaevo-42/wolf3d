/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 14:46:04 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/05 14:19:13 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCK_H
# define BLOCK_H

# include "libft.h"
# include "texture.h"

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

struct							s_block_normal
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

t_face							get_face_type(char *string);

#endif
