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

int		unload_map_col(t_block_state **col, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		free(col[i]);
		i++;
	}
	free(col);
	return (0);
}

int		unload_map(t_block_state ***map, size_t y, size_t x)
{
	size_t	i;

	i = 0;
	while (i < y)
	{
		unload_map_col(map[i], x);
		i++;
	}
	free(map);
	return (0);
}

void	unload(t_wolf *wolf)
{
	if (wolf->textures)
		unload_textures(wolf->textures, wolf->textures_count);
	if (wolf->blocks)
		unload_blocks(wolf->blocks, wolf->blocks_count);
	if (wolf->world.data)
		unload_map(wolf->world.data, wolf->world.size.y, wolf->world.size.x);
	SDL_FreeSurface(wolf->head_overlay);
	SDL_FreeSurface(wolf->crosshair);
	SDL_FreeSurface(wolf->heads_texture.super.surface);
	SDL_FreeSurface(wolf->weapons_texture.super.surface);
}