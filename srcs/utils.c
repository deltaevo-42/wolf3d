/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 23:40:27 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/03 15:38:53 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include <assert.h>

Uint32	getpixel(SDL_Surface *surface, int x, int y)
{
	return (*(Uint32 *)(surface->pixels + y * surface->pitch + x * 4));
}

static t_color	alpha_blend(t_color c1, t_color c2)
{
	float	a1;
	float	a2;
	float	a;

	a1 = c1.a / 255.0;
	a2 = c2.a / 255.0;
	a = a2 * (1 - a1);
	return ((t_color) {
		.r = c1.r * a1 + c2.r * a,
		.g = c1.g * a1 + c2.g * a,
		.b = c1.b * a1 + c2.b * a,
		.a = (a1 + a) * 0xFF
	});
}

void	apply_surface_blended(t_img *img, SDL_Surface *s, SDL_Rect src,
	SDL_Rect dst)
{
	int				i;
	int				j;
	const float		s_w = src.w / (float)dst.w;
	const float		s_h = src.h / (float)dst.h;
	uint32_t		index;

	i = -1;
	while (++i < dst.h)
	{
		j = -1;
		while (++j < dst.w)
		{
			index = (((dst.y + i) * (int)S_WIDTH) + (dst.x + j));
			if (index >= img->size)
				break ;
			img->pixels[index] = ft_color_i(alpha_blend(ft_i_color(
				getpixel(s, (int)(j * s_w) + src.x, (int)(i * s_h) + src.y)),
				ft_i_color(img->pixels[index])));
		}
	}
}

int		min(int a, int b)
{
	return (a < b ? a : b);
}

void	apply_surface(t_img *img, SDL_Surface *s, SDL_Rect src,
	SDL_Rect dst)
{
	int				i;
	int				j;
	const SDL_Rect	d2 = dst;
	const t_vec2	s_v = (t_vec2){src.w / (float)dst.w, src.h / (float)dst.h};
	t_pixel			s_p;

	s_p = (t_pixel){min(img->width - 1, dst.x + dst.w),
					min(img->height - 1, dst.y + dst.h), 0};
	if (dst.y < 0)
		dst.y = 0;
	if (dst.x < 0)
		dst.x = 0;
	i = dst.y - 1;
	while (++i < s_p.y)
	{
		j = dst.x - 1;
		while (++j < s_p.x)
		{
			if (img->pixels[((i * (int)S_WIDTH) + j)] == 0)
				img->pixels[((i * (int)S_WIDTH) + j)] = getpixel(s,
				(int)((j - d2.x) * s_v.x) + src.x,
				(int)((i - d2.y) * s_v.y) + src.y);
		}
	}
}

void	apply_texture(t_img *img, t_texture *t, SDL_Rect src, SDL_Rect dst)
{
	t_texture_animated	*a;
	SDL_Rect			s;
	int					row;

	s = src;
	if (t->type == T_ANIMATED)
	{
		a = (t_texture_animated *)t;
		row = a->index / (int)a->step_count.x;
		s.y = src.y + (int)(a->step_size.y + a->spacer.y) * row;
		s.x = src.x + (int)(a->step_size.x + a->spacer.x)
			* (a->index - (int)(row * (int)a->step_count.x));
	}
	apply_surface(img, t->surface, s, dst);
}

void	apply_texture_blended(t_img *img, t_texture *t, SDL_Rect src,
		SDL_Rect dst)
{
	t_texture_animated	*a;
	SDL_Rect			s;
	int					row;

	s = src;
	if (t->type == T_ANIMATED)
	{
		a = (t_texture_animated *)t;
		row = a->index / (int)a->step_count.x;
		s.y = src.y + (int)(a->step_size.y + a->spacer.y) * row;
		s.x = src.x + (int)(a->step_size.x + a->spacer.x)
			* (a->index - (int)(row * (int)a->step_count.x));
	}
	apply_surface_blended(img, t->surface, s, dst);
}

void	draw_line(t_img *img, t_pixel p0, t_pixel p1)
{
	t_pixel			d;
	t_pixel			s;
	int				e[2];
	unsigned int	index;

	d = (t_pixel){ abs(p1.x - p0.x), abs(p1.y - p0.y), 0 };
	s = (t_pixel){ (p0.x < p1.x ? 1 : -1), (p0.y < p1.y ? 1 : -1), 0 };
	e[0] = (d.x > d.y ? d.x : -d.y) / 2;
	while (p0.x != p1.x || p0.y != p1.y)
	{
		if ((index = p0.y * img->width + p0.x) >= img->size)
			break ;
		img->pixels[index] = p0.color;
		e[1] = e[0];
		if (e[1] > -d.x)
		{
			e[0] -= d.y;
			p0.x += s.x;
		}
		if (e[1] < d.y)
		{
			e[0] += d.x;
			p0.y += s.y;
		}
	}
}

void	stroke_rect(t_img *img, uint32_t color, SDL_Rect rect)
{
	draw_line(img,
		(t_pixel) {rect.x, rect.y, color},
		(t_pixel) {rect.x + rect.w, rect.y, color});
	draw_line(img,
		(t_pixel) {rect.x + rect.w, rect.y, color},
		(t_pixel) {rect.x + rect.w, rect.y + rect.h, color});
	draw_line(img,
		(t_pixel) {rect.x + rect.w, rect.y + rect.h, color},
		(t_pixel) {rect.x, rect.y + rect.h, color});
	draw_line(img,
		(t_pixel) {rect.x, rect.y + rect.h, color},
		(t_pixel) {rect.x, rect.y, color});
}

t_bool	between(t_pixel p, SDL_Rect bounds)
{
	if (p.x < bounds.x || p.x >= bounds.x + bounds.w
		|| p.y < bounds.y || p.y >= bounds.y + bounds.h)
		return (FALSE);
	return (TRUE);
}
