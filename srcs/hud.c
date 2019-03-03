/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 23:45:21 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/03 16:16:45 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

static void	update_texture(t_wolf *wolf, t_texture_animated *weapons)
{
	if (SDL_GetTicks() - weapons->last_seen > weapons->delay)
	{
		if (!wolf->player.shooting)
			return ;
		weapons->index++;
		weapons->last_seen = SDL_GetTicks();
		if ((weapons->index % (int)weapons->step_count.x) == 0)
		{
			weapons->index = wolf->player.selected_weapon
				* weapons->step_count.x;
			wolf->player.shooting = FALSE;
		}
	}
}

void	render_hud(t_wolf *wolf)
{
	t_texture_animated *const	weapons = &wolf->weapons_texture;

	apply_texture_blended(wolf->img, weapons,
		(SDL_Rect){ 0, 0, weapons->super.size.x, weapons->super.size.y },
		(SDL_Rect){ S_WIDTH_2 - (S_WIDTH / 3) / 2, S_HEIGHT - (S_WIDTH / 3) + 1,
					(S_WIDTH / 3), (S_WIDTH / 3) });
	apply_texture_blended(wolf->img, &wolf->heads_texture,
		(SDL_Rect){ 0, 0, wolf->heads_texture.super.size.x,
						wolf->heads_texture.super.size.y },
		(SDL_Rect){ S_WIDTH - 100 - 10, S_HEIGHT - 120 - 10, 100, 120 });
	apply_surface_blended(wolf->img, wolf->head_overlay,
		(SDL_Rect){ 0, 0, wolf->head_overlay->w + 5, wolf->head_overlay->h + 5},
		(SDL_Rect){ S_WIDTH - 100 - 12, S_HEIGHT - 120 - 12, 110, 130 });
	apply_surface_blended(wolf->img, wolf->crosshair,
		(SDL_Rect){ 0, 0, wolf->crosshair->w, wolf->crosshair->h},
		(SDL_Rect){ S_WIDTH_2 - 25, S_HEIGHT_2 - 25, 50, 50 });
	update_texture(wolf, weapons);
}
