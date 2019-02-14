/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 11:58:09 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/14 16:41:26 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF_H
# define WOLF_H

# include <SDL.h>
# include <SDL_image.h>
# include <SDL_ttf.h>
# include <math.h>
# include <time.h>
# include "libft.h"
# include <stdlib.h>

# define S_WIDTH (800.0)
# define S_HEIGHT (800.0)

typedef enum e_bool		t_bool;
typedef enum e_face		t_face;

typedef struct s_wolf	t_wolf;
typedef struct s_stats	t_stats;
typedef struct s_fonts	t_fonts;
typedef struct s_ray	t_ray;
typedef struct s_player	t_player;

enum		e_bool
{
	TRUE = 1,
	FALSE = 0
};

enum		e_face
{
	NORTH,
	SOUTH,
	EAST,
	WEST
};

struct		s_stats
{
	int		fps;
	double	avg_ms;
	clock_t	delta;
};

struct		s_fonts
{
	TTF_Font	*helvetica;
};

struct		s_ray
{
	t_vec2	dir;
	t_vec2	side_dist;
	t_vec2	delta_dist;
	t_pixel	hit_pos;
	t_face	face;
	float	dist;
	int		side;
	int		hit;
};

struct		s_player
{
	t_vec2	pos;
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
	t_bool			running;
	t_stats			stats;
	t_fonts			fonts;
	t_player		player;

};

/* SDL */
void		sdl_init(t_wolf *wolf);
void		sdl_quit(t_wolf *wolf);

/* Events */
void		hook_events(t_wolf *wolf);

/* Render */
void		render_debug(t_wolf *wolf);
void		render_main(t_wolf *wolf);

/* Ray */
t_bool		cast_ray(t_wolf *wolf, t_ray *ray, int x);

/* Game */
void		game_loop(t_wolf *wolf);

/* Utils */
double		ticks_to_ms(clock_t ticks);

#endif
