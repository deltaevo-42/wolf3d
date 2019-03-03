/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 23:40:27 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/03 18:52:48 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

Uint32	getpixel(SDL_Surface *surface, int x, int y)
{
	return (*(Uint32 *)(surface->pixels + y * surface->pitch + x * 4));
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
