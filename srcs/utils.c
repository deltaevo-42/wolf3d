/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 23:40:27 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/24 21:52:05 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
	const int			bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp)
    {
        case 1:
            return *p;
        case 2:
            return *(Uint16 *)p;
        case 3:
            if (SDL_BYTEORDER != SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            if (SDL_BYTEORDER != SDL_BIG_ENDIAN)
                return p[3] << 24 | p[0] << 16 | p[1] << 8 | p[2];
            else
                return *(Uint32 *)p;
        default:
            return 0;
    }
}

void	apply_surface_blended(uint32_t *dest, SDL_Surface *s, SDL_Rect src, SDL_Rect dst)
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
			if (index >= IMG_MAX_I)
				continue;

			unsigned int value = getpixel(s, (int)(j * s_w) + src.x, (int)(i * s_h) + src.y);
			int g1 = (value >> 0) & 0xFF;
			int b1 = (value >> 8) & 0xFF;
			int r1 = (value >> 16) & 0xFF;
			float a1 = ((value >> 24) & 0xFF) / 255.0;
			int g2 = (dest[index] >> 0) & 0xFF;
			int b2 = (dest[index] >> 8) & 0xFF;
			int r2 = (dest[index] >> 16) & 0xFF;
			float a2 = ((dest[index] >> 24) & 0xFF) / 255.0;

			float a = a2 * (1 - a1);
			dest[index] = (((int)((a1 + a) * 0xFF) << 24)
						| ((int)(r1 * a1 + r2 * a) & 0xFF) << 16
						| ((int)(b1 * a1 + b2 * a) & 0xFF) << 8
						| ((int)(g1 * a1 + g2 * a) & 0xFF));
		}
	}
}

void	apply_surface(uint32_t *dest, SDL_Surface *s, SDL_Rect src, SDL_Rect dst)
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
			if (index >= IMG_MAX_I || dest[index] != 0)
				continue;
			dest[index] = getpixel(s, (int)(j * s_w) + src.x, (int)(i * s_h) + src.y);
		}
	}
}

void	draw_line(uint32_t *pixels, uint32_t width, t_pixel p0, t_pixel p1)
{
	t_pixel			d;
	t_pixel			s;
	int				e[2];
	unsigned int 	index;

	d = (t_pixel){ abs(p1.x - p0.x), abs(p1.y - p0.y) };
	s = (t_pixel){ (p0.x < p1.x ? 1 : -1), (p0.y < p1.y ? 1 : -1) };
	e[0] = (d.x > d.y ? d.x : -d.y) / 2;
	while (p0.x != p1.x || p0.y != p1.y)
	{
		index = p0.y * width + p0.x;
		pixels[index] = p0.color;
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

void	stroke_rect(uint32_t *pixels, uint32_t width, uint32_t color, SDL_Rect rect)
{
	draw_line(pixels, width,
		(t_pixel) {rect.x, rect.y, color},
		(t_pixel) {rect.x + rect.w, rect.y, color});
	draw_line(pixels, width,
		(t_pixel) {rect.x + rect.w, rect.y, color},
		(t_pixel) {rect.x + rect.w, rect.y + rect.h, color});
	draw_line(pixels, width,
		(t_pixel) {rect.x + rect.w, rect.y + rect.h, color},
		(t_pixel) {rect.x, rect.y + rect.h, color});
	draw_line(pixels, width,
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
