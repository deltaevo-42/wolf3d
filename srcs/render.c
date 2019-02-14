/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 19:54:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/14 17:27:13 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void			render_debug(t_wolf *wolf)
{
	SDL_Surface		*fps_text;
	SDL_Texture		*fps_texture;
	SDL_Rect		dest;

	fps_text = TTF_RenderText_Blended(wolf->fonts.helvetica, ft_int_to_str(wolf->stats.fps).str, (SDL_Color){255, 255, 255});
	fps_texture = SDL_CreateTextureFromSurface(wolf->renderer, fps_text);
	dest = (SDL_Rect){5, 5, 25, 25};
	SDL_RenderCopy(wolf->renderer, fps_texture, NULL, &dest);
	SDL_DestroyTexture(fps_texture);
	SDL_FreeSurface(fps_text);
	fps_text = TTF_RenderText_Blended(wolf->fonts.helvetica, ft_int_to_str((int)wolf->stats.avg_ms).str, (SDL_Color){255, 255, 255});
	fps_texture = SDL_CreateTextureFromSurface(wolf->renderer, fps_text);
	dest = (SDL_Rect){5, 35, 25, 25};
	SDL_RenderCopy(wolf->renderer, fps_texture, NULL, &dest);
	SDL_DestroyTexture(fps_texture);
	SDL_FreeSurface(fps_text);
}

void			render_main(t_wolf *wolf)
{
	int		x;
	t_ray	ray;

	wolf->player.matrix = ft_mat2_rotation(wolf->player.rotation - M_PI_2);
	
	for(unsigned int i = 0; i < S_WIDTH * 4 * S_HEIGHT; i++ )
	{
		wolf->pixels[i] = 0;
	}
	
	//cast_ray(wolf, &ray, 15);
	//printf("RAY\n\tdir (%f, %f)\n\tdist (%f)\n\thit (%d)\n\thit_pos (%d, %d)\n\tside (%d)\n", ray.dir.x, ray.dir.y, ray.dist, ray.hit, ray.hit_pos.x, ray.hit_pos.y, ray.side);

	x = -1;
	while (++x < S_WIDTH)
	{
		cast_ray(wolf, &ray, x);
		int height = S_HEIGHT / ray.dist / 2.0;
		t_color color = (t_color) {255, 255, 255, 0};
		if (ray.hit == 1)
			color = (t_color) {255, 0, 0, 0};
		if (ray.hit == 2)
			color = (t_color) {0, 255, 0, 0};
		if (ray.hit == 3)
			color = (t_color) {0, 0, 255, 0};
		if (ray.hit == 4)
			color = (t_color) {255, 0, 255, 0};
		color = ft_color_gradient(color, (t_color) {color.r + 20, color.g + 20, color.b + 20, 0}, ray.dist / 24);
		for (int h = S_HEIGHT / 2 - height; h < S_HEIGHT / 2 + height; h++)
		{
			const unsigned int offset = (h * (int)S_WIDTH * 4) + x * 4;
			wolf->pixels[offset] = color.b;
			wolf->pixels[offset + 1] = color.g;
			wolf->pixels[offset + 2] = color.r;
			wolf->pixels[offset + 3] = SDL_ALPHA_OPAQUE;
		}
	}
}
