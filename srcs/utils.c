/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 23:40:27 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/26 19:05:52 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include <assert.h>

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
	assert(surface->format->BytesPerPixel == 4);
    return *(Uint32 *)(surface->pixels + y * surface->pitch + x * 4);
}

void	apply_surface_blended(t_img *img, SDL_Surface *s, SDL_Rect src, SDL_Rect dst)
{
	int 			i;
	int 			j;
	const float		s_w = src.w / (float)dst.w;
	const float		s_h = src.h / (float)dst.h;
	unsigned int 	index;

	i = -1;
	while (++i < dst.h)
	{
		j = -1;
		while (++j < dst.w)
		{
			index = (((dst.y + i) * (int)S_WIDTH) + (dst.x + j));
			if (index >= img->size)
				continue;

			unsigned int value = getpixel(s, (int)(j * s_w) + src.x, (int)(i * s_h) + src.y);
			int g1 = (value >> 0) & 0xFF;
			int b1 = (value >> 8) & 0xFF;
			int r1 = (value >> 16) & 0xFF;
			float a1 = ((value >> 24) & 0xFF) / 255.0;
			int g2 = (img->pixels[index] >> 0) & 0xFF;
			int b2 = (img->pixels[index] >> 8) & 0xFF;
			int r2 = (img->pixels[index] >> 16) & 0xFF;
			float a2 = ((img->pixels[index] >> 24) & 0xFF) / 255.0;

			float a = a2 * (1 - a1);
			img->pixels[index] = (((int)((a1 + a) * 0xFF) << 24)
						| ((int)(r1 * a1 + r2 * a) & 0xFF) << 16
						| ((int)(b1 * a1 + b2 * a) & 0xFF) << 8
						| ((int)(g1 * a1 + g2 * a) & 0xFF));
		}
	}
}

void	apply_surface(t_img *img, SDL_Surface *s, SDL_Rect src, SDL_Rect dst)
{
	int 			i;
	int 			j;
	const float		s_w = src.w / (float)dst.w;
	const float		s_h = src.h / (float)dst.h;
	unsigned int 	index;

	i = -1;
	while (++i < dst.h)
	{
		j = -1;
		while (++j < dst.w)
		{
			index = (((dst.y + i) * (int)S_WIDTH) + (dst.x + j));
			if (index >= img->size)
				break ;
			if (img->pixels[index] == 0)
				img->pixels[index] = getpixel(s, (int)(j * s_w) + src.x, (int)(i * s_h) + src.y);
		}
	}
}

void	apply_texture(t_img *img, t_texture *t, SDL_Rect src, SDL_Rect dst)
{
	t_texture_animated *a;
	SDL_Rect s;

	s = src;
	if (t->type == T_ANIMATED)
	{
		a = (t_texture_animated *)t;
		s.x = src.x + (int)(a->step_size.x + a->spacer.x) * (a->index % (int)a->step_count.x);
		s.y = src.y + (int)(a->step_size.y + a->spacer.y) * (a->index % (int)a->step_count.y);
	}
	apply_surface(img, t->surface, s, dst);
}

void	apply_texture_blended(t_img *img, t_texture *t, SDL_Rect src, SDL_Rect dst)
{
	apply_surface_blended(img, t->surface, src, dst);
}

void	draw_line(t_img *img, t_pixel p0, t_pixel p1)
{
	t_pixel			d;
	t_pixel			s;
	int				e[2];
	unsigned int 	index;

	d = (t_pixel){ abs(p1.x - p0.x), abs(p1.y - p0.y), 0 };
	s = (t_pixel){ (p0.x < p1.x ? 1 : -1), (p0.y < p1.y ? 1 : -1), 0 };
	e[0] = (d.x > d.y ? d.x : -d.y) / 2;
	while (p0.x != p1.x || p0.y != p1.y)
	{
		index = p0.y * img->width + p0.x;
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
