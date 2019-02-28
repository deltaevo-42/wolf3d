/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 11:58:09 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/28 18:04:31 by llelievr         ###   ########.fr       */
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


# define BAR_H (100.0)
# define S_WIDTH (1280.0)
# define S_HEIGHT (720.0)
# define S_WIDTH_2 (S_WIDTH / 2)
# define S_HEIGHT_2 (S_HEIGHT / 2)
# define S_RATIO (S_WIDTH / S_HEIGHT)
# define IMG_MAX_I (S_WIDTH * S_HEIGHT)
# define PLANE ((S_WIDTH / 2) / S_WIDTH)


typedef struct s_world			t_world;
typedef struct s_img			t_img;

typedef struct s_wolf			t_wolf;
typedef struct s_stats			t_stats;
typedef struct s_fonts			t_fonts;
typedef struct s_ray			t_ray;
typedef struct s_player			t_player;

# include "texture.h"
# include "block.h"

struct		s_world
{
	t_block_state	***data;
	t_texture		**textures;
	t_block			**blocks;
	t_texture		*ceil;
	t_texture		*floor;
	int				textures_count;
	int				blocks_count;
	t_vec3			size;
};


struct		s_stats
{
	int		fps;
	double	avg_ms;
	double	delta;
	uint32_t num_rays;
};

struct		s_fonts
{
	TTF_Font	*helvetica;
};

struct		s_ray
{
	t_block_state	*hit;
	int				x;
	t_vec2			start;
	t_vec2			dir;
	t_pixel			step;
	t_vec2			side_dist;
	t_vec2			delta_dist;
	t_pixel			hit_pos;
	t_face			face;
	float			dist;
	int				side;
	t_block_state	*fhit;
	t_world			*world;

	float			circle_last_hit_x;
	float			circle_last_hit_y;
	float			circle_last_out_dist;
};

struct		s_player
{
	t_vec3	pos;
	float	rotation;
	t_mat2	matrix;
};

struct		s_img
{
	uint32_t	*pixels;
	uint32_t	size;
	uint32_t	width;
	uint32_t	height;
};

struct		s_wolf
{
	t_img			*img;
	SDL_Texture		*screen;
	SDL_Surface		*tmp_texture;
	SDL_Renderer	*renderer;
	SDL_Window		*win;
	t_world			world;
	SDL_Event		event;
	t_ray			last_rays[(int)S_WIDTH];
	t_bool			running;
	t_stats			stats;
	t_fonts			fonts;
	t_player		player;
	t_bool			fullscreen;

	uint32_t		minimap_size;
	uint32_t		minimap_padding_x;
	uint32_t		minimap_padding_y;
	float			dist_to_plane;
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
t_bool		next_ray(t_ray *ray);
t_bool		prev_ray(t_ray *ray);
t_ray		create_ray(t_wolf *wolf, int x, t_vec2 start);

/* Game */
void		game_loop(t_wolf *wolf);

/* Utils */
Uint32 		getpixel(SDL_Surface *surface, int x, int y);
void		draw_line(t_img *img, t_pixel p0, t_pixel p1);
void		stroke_rect(t_img *img, uint32_t color, SDL_Rect rect);
void		apply_surface(t_img *img, SDL_Surface *s, SDL_Rect src, SDL_Rect dst);
void		apply_surface_blended(t_img *img, SDL_Surface *s, SDL_Rect src, SDL_Rect dst);

void		apply_texture(t_img *img, t_texture *t, SDL_Rect src, SDL_Rect dst);
void		apply_texture_blended(t_img *img, t_texture *t, SDL_Rect src, SDL_Rect dst);

#endif
