/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 23:45:21 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/01 01:20:18 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void	render_hud(t_wolf *wolf)
{
	apply_texture_blended(wolf->img, &wolf->weapons_texture,
		(SDL_Rect){ 0, 0, wolf->weapons_texture.super.size.x, wolf->weapons_texture.super.size.y },
		(SDL_Rect){ S_WIDTH_2 - (S_WIDTH / 3) / 2, S_HEIGHT - (S_WIDTH / 3) + 1, (S_WIDTH / 3), (S_WIDTH / 3) });
	apply_texture_blended(wolf->img, &wolf->heads_texture,
		(SDL_Rect){ 0, 0, wolf->heads_texture.super.size.x, wolf->heads_texture.super.size.y },
		(SDL_Rect){ S_WIDTH - 100 - 10, S_HEIGHT - 120 - 10, 100, 120 });
	apply_surface_blended(wolf->img, wolf->head_overlay, 
		(SDL_Rect){ 0, 0, wolf->head_overlay->w + 5, wolf->head_overlay->h + 5 },
		(SDL_Rect){ S_WIDTH - 100 - 12, S_HEIGHT - 120 - 12, 110, 130 });

	if (SDL_GetTicks() - wolf->weapons_texture.last_seen > wolf->weapons_texture.delay)
	{
		if (!wolf->player.shooting)
			return ;
		wolf->weapons_texture.index++;
		wolf->weapons_texture.last_seen = SDL_GetTicks();
		if ((wolf->weapons_texture.index % (int)wolf->weapons_texture.step_count.x) == 0)
		{
			wolf->weapons_texture.index = wolf->player.selected_weapon * wolf->weapons_texture.step_count.x;
			wolf->player.shooting = FALSE;
		}
	}
}