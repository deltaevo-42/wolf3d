/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 23:40:27 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/18 12:25:48 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
	unsigned int	index;

	int bpp = surface->format->BytesPerPixel;
	index = y * surface->pitch + x * bpp;
	Uint8 *p = (Uint8 *)surface->pixels + index;
	return ((p[0] << 16) | (p[1] << 8) | p[2]);
}

void	apply_surface(unsigned char **dest, SDL_Surface *s, SDL_Rect src, SDL_Rect dst)
{
	int i;
	int j;
	const float		s_w = src.w / (float)dst.w;
	const float		s_h = src.h / (float)dst.h;
	unsigned int index;

	i = -1;
	while (++i < dst.h)
	{
		j = -1;
		while (++j < dst.w)
		{
			index = (((dst.y + i) * (int)S_WIDTH) + (dst.x + j));
			if (index >= IMG_MAX_I)
				continue;
			((unsigned int *)*dest)[index] = getpixel(s, (int)(j * s_w) + src.x, (int)(i * s_h) + src.y);
		}
	}
}

void	draw_line(t_wolf *wolf, t_pixel p0, t_pixel p1)
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
		index = p0.y * (int)S_WIDTH + p0.x;
		if (index > IMG_MAX_I)
			continue ;
		((unsigned int *)wolf->pixels)[index] = p0.color;
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
