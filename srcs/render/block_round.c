#include "wolf.h"

t_bool	render_block_round_wall(t_wolf *wolf, t_ray *ray)
{
	t_block_round *block;
	SDL_Surface	*surface;
	float		height;
	double		wallX;
	int			texX;

	block = (t_block_round *)ray->hit->block;
	surface = ((t_texture_normal *)block->texture)->surface;
	height = (S_HEIGHT / ray->dist);
	if (ray->side == 0)
		wallX = wolf->player.pos.y + ray->dist * ray->dir.y;
	else
		wallX = wolf->player.pos.x + ray->dist * ray->dir.x;
	wallX -= floor((wallX));
	texX = wallX * surface->w;
	if ((ray->side == 0 && ray->dir.x > 0) || (ray->side == 1 && ray->dir.y < 0))
		texX = surface->w - texX - 1;
	apply_surface(wolf->pixels, surface,
		(SDL_Rect){ texX, 0, 1, surface->h }, 
		(SDL_Rect){ ray->x, S_HEIGHT_2 + height * (wolf->player.pos.z + 1) / 2. - height * block->super.height, 1, height * block->super.height});
	return (FALSE);
}
