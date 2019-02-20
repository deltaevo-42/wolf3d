/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 11:58:09 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/20 17:29:34 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF_H
# define WOLF_H

# include <SDL.h>
# include <SDL_image.h>
# include <SDL_ttf.h>
# include <math.h>
# include "libft.h"
# include <stdlib.h>

# define S_WIDTH (1000.0)
# define S_HEIGHT (1000.0)
# define S_WIDTH_2 (S_WIDTH / 2)
# define S_HEIGHT_2 (S_HEIGHT / 2)
# define IMG_MAX_I (S_WIDTH * S_HEIGHT)


typedef enum e_texture_type		t_texture_type;
typedef struct s_texture		t_texture;
typedef struct s_texture_normal	t_texture_normal;

typedef struct s_world			t_world;

typedef struct s_wolf			t_wolf;
typedef struct s_stats			t_stats;
typedef struct s_fonts			t_fonts;
typedef struct s_ray			t_ray;
typedef struct s_player			t_player;

# include "texture.h"
# include "block.h"

struct		s_world
{
	int				textures_count;
	t_texture		**textures;
	int				blocks_count;
	t_block			**blocks;
	t_size			size;
	t_block_state	***data;
};


struct		s_stats
{
	int		fps;
	double	avg_ms;
	double	delta;
};

struct		s_fonts
{
	TTF_Font	*helvetica;
};

struct		s_ray
{
	int				x;
	t_vec2			dir;
	t_vec2			side_dist;
	t_vec2			delta_dist;
	t_pixel			hit_pos;
	t_face			face;
	float			dist;
	int				side;
	t_block_state	*hit;
};

struct		s_player
{
	t_vec3	pos;
	float	rotation;
	t_mat2	matrix;
};



struct		s_wolf
{
	SDL_Window		*win;
	SDL_Renderer	*renderer;
	SDL_Texture		*screen;
	SDL_Event		event;
	unsigned char	*pixels;
	t_ray			last_rays[(int)S_WIDTH];
	t_bool			running;
	t_stats			stats;
	t_fonts			fonts;
	t_player		player;
	t_world			world;
};

/* World */
void		load_world(t_world *world, char *file);

/* SDL */
void		sdl_init(t_wolf *wolf);
void		sdl_quit(t_wolf *wolf);

/* Events */
void		hook_events(t_wolf *wolf);

/* Render */
void		render_debug(t_wolf *wolf);
void		render_minimap(t_wolf *wolf);
void		render_main(t_wolf *wolf);

/* Ray */
void		next_ray(t_wolf *wolf, t_ray *ray);
t_ray		create_ray(t_wolf *wolf, int x);

/* Game */
void		game_loop(t_wolf *wolf);

/* Utils */
Uint32 		getpixel(SDL_Surface *surface, int x, int y);
void		draw_line(t_wolf *wolf, t_pixel p0, t_pixel p1);
void		apply_surface(unsigned char **dest, SDL_Surface *s, SDL_Rect src, SDL_Rect dst);

#endif
