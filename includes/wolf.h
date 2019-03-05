/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 11:58:09 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/05 14:06:08 by dde-jesu         ###   ########.fr       */
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
# define S_WIDTH (1000.0)
# define S_HEIGHT (562.0)
# define S_WIDTH_2 (S_WIDTH / 2)
# define S_HEIGHT_2 (S_HEIGHT / 2)
# define S_RATIO (S_WIDTH / S_HEIGHT)
# define IMG_MAX_I (S_WIDTH * S_HEIGHT)
# define PLANE ((S_WIDTH / 2) / S_WIDTH)
# define DEG_TO_RAD (M_PI / 180.)

typedef struct s_world	t_world;
typedef struct s_img	t_img;

typedef struct s_wolf	t_wolf;
typedef struct s_stats	t_stats;
typedef struct s_fonts	t_fonts;
typedef struct s_ray	t_ray;
typedef struct s_player	t_player;

# include "texture.h"
# include "block.h"

struct					s_world
{
	t_block_state	***data;
	t_texture		*ceil;
	t_texture		*floor;
	t_vec3			size;
	t_vec3			spawn_pos;
	float			spawn_rotation;
};

struct					s_stats
{
	int			fps;
	double		avg_ms;
	double		delta;
	uint32_t	num_rays;
};

struct					s_fonts
{
	TTF_Font	*helvetica;
};

struct					s_ray
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
	float			sdist;
	float			extra_dist;
	int				side;
	t_block_state	*fhit;
	t_world			*world;

	float			circle_last_hit_x;
	float			circle_last_hit_y;
	float			circle_last_out_dist;
};

struct					s_player
{
	t_vec3	pos;
	float	rotation;
	t_mat2	matrix;
	int		selected_weapon;
	t_bool	shooting;
};

struct					s_img
{
	uint32_t	*pixels;
	uint32_t	size;
	uint32_t	width;
	uint32_t	height;
};

struct					s_wolf
{
	t_img				*img;
	SDL_Texture			*screen;
	SDL_Renderer		*renderer;
	SDL_Window			*win;
	t_texture			**textures;
	t_block				**blocks;
	int					textures_count;
	int					blocks_count;
	int					worlds_count;
	t_world				world;
	t_ray				last_rays[(int)S_WIDTH];
	t_bool				running;
	t_stats				stats;
	t_fonts				fonts;
	t_player			player;
	t_bool				fullscreen;

	t_texture_animated	weapons_texture;
	t_texture_animated	heads_texture;
	SDL_Surface			*head_overlay;
	SDL_Surface			*crosshair;

	uint32_t			minimap_size;
	uint32_t			minimap_padding_x;
	uint32_t			minimap_padding_y;
	float				dist_to_plane;
};

typedef	struct			s_circle {
	double	p;
	double	q;
	double	r;
}						t_circle;

typedef struct			s_line {
	double	m;
	double	c;
}						t_line;

typedef struct			s_polynom {
	double	a;
	double	b;
	double	c;
}						t_polynom;

/*
** World
*/

t_bool					load_map(t_wolf *wolf, t_world *world,
	t_json_object *obj);
t_bool					load_config(t_wolf *wolf, char *file);
void					unload(t_wolf *wolf);
void					unload_blocks(t_block **blocks, size_t size);
int						unload_map_col(t_block_state **col, size_t size);
int						unload_map(t_block_state ***map, size_t y, size_t x);
void					unload_textures(t_texture **textures, size_t size);

/*
** SDL
*/

t_bool					sdl_init(t_wolf *wolf);
t_bool					sdl_quit(t_wolf *wolf);

/*
** Events
*/

void					hook_events(t_wolf *wolf);
void					events_move(t_wolf *wolf, const Uint8 *state);

/*
** Render
*/

void					render_debug(t_wolf *wolf);
void					render_minimap(t_wolf *wolf);
void					render_main(t_wolf *wolf);
void					render_hud(t_wolf *wolf);
void					render_floor(t_wolf *wolf,
	t_ray *from, t_ray *to, int last_floor);
void					render_ceil(t_wolf *wolf,
	t_ray *from, t_ray *to);
void					render_binary(t_wolf *wolf, int x1, int x2);
void					try_portal(t_wolf *wolf, t_ray *from, t_ray *to,
	int last_y);
t_bool					render_top(t_wolf *wolf, t_ray *ray[2],
	t_block_state *hit, int p[2]);
t_bool					render_wall(t_wolf *wolf, t_ray *from,
	t_ray *to, int last_y);

/*
** Ray
*/

t_bool					next_ray(t_ray *ray);
t_bool					prev_ray(t_ray *ray);
t_ray					create_ray(t_wolf *wolf, int x, t_vec2 start);
void					ray_use_portal(t_ray *ray);
t_bool					ray_in_map(t_ray *ray);
void		    		ray_compute_dist(t_ray *ray);

/*
** Game
*/

void					game_loop(t_wolf *wolf);
t_bool					init_defaults(t_wolf *wolf);

/*
** Utils
*/

Uint32					getpixel(SDL_Surface *surface, int x, int y);
void					draw_line(t_img *img, t_pixel p0, t_pixel p1);
void					stroke_rect(t_img *img, uint32_t color, SDL_Rect rect);
void					apply_surface(t_img *img, SDL_Surface *s, SDL_Rect src,
		SDL_Rect dst);
void					apply_surface_blended(t_img *img, SDL_Surface *s,
		SDL_Rect src, SDL_Rect dst);

void					apply_texture(t_img *img, t_texture *t, SDL_Rect src,
		SDL_Rect dst);
void					apply_texture_blended(t_img *img, t_texture *t,
		SDL_Rect src, SDL_Rect dst);

#endif
