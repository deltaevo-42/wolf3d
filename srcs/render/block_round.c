#include "wolf.h"

t_bool	render_block_round_wall(t_wolf *wolf, t_ray *ray)
{
	t_block_round *block;
	SDL_Surface	*surface;
	float		height;
	float		wallX;
	int			texX;

	block = (t_block_round *)ray->hit->block;
	surface = ((t_texture_normal *)block->texture)->surface;
	height = (S_HEIGHT / ray->dist);
	wallX = (atan2(block->last_hit_y, block->last_hit_x) + M_PI) / (2 * M_PI);
	texX = wallX * surface->w;
	apply_surface(wolf->pixels, surface,
		(SDL_Rect){ texX, 0, 1, surface->h }, 
		(SDL_Rect){ ray->x, S_HEIGHT_2 + height * (wolf->player.pos.z + 1) / 2. - height * block->super.height, 1, height * block->super.height});
	return (FALSE);
}
