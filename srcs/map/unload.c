#include "wolf.h"

void	unload_textures(t_texture **textures, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		SDL_FreeSurface(textures[i]->surface);
		free(textures[i]);
		i++;
	}
	free(textures);
}

void	unload_blocks(t_block **blocks, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		free(blocks[i]);
		i++;
	}
	free(blocks);
}

void unload_world(t_world *world)
{
	size_t	x;
	size_t	y;

	y = 0;
	while (y < world->size.y)
	{
		x = 0;
		while (x < world->size.x)
		{
			free(world->data[y][x]);
			x++;
		}
		free(world->data[y]);
		y++;
	}
	free(world->data);
}

void	unload(t_wolf *wolf)
{
	unload_textures(wolf->textures, wolf->textures_count);
	unload_blocks(wolf->blocks, wolf->blocks_count);
	unload_world(&wolf->world);
	SDL_FreeSurface(wolf->head_overlay);
	SDL_FreeSurface(wolf->crosshair);
	SDL_FreeSurface(wolf->heads_texture.super.surface);
	SDL_FreeSurface(wolf->weapons_texture.super.surface);
}