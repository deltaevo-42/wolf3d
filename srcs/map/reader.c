/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/17 22:46:01 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/01 17:12:38 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include <fcntl.h>
#include <unistd.h>
#define BUFF_SIZE 1

static char			*get_file_content(char *file, int *content_len)
{
	int		fd;
	char	*content;
	char	buf[BUFF_SIZE + 1];
	int		rd;

	if ((fd = open(file, O_RDONLY)) == -1)
		return (NULL);
	*content_len = 0;
	content = NULL;
	while ((rd = read(fd, &buf, BUFF_SIZE)) > 0)
	{
		buf[rd] = 0;
		content = ft_realloc(content, *content_len, *content_len + rd);
		ft_memcpy(content + *content_len, buf, rd);
		*content_len += rd;
	}
	if (rd != -1)
		return (content);
	if (content)
		free(content);
	return (NULL);
}

static t_texture	**load_textures(t_world *world, t_json_object *obj)
{
	t_json_array	*arr;
	t_json_element	*e;
	t_texture		**tex;
	int				i;

	if (!(arr = json_get_array(obj, "textures")) ||
	!(tex = (t_texture **)malloc(sizeof(t_texture *) * (arr->elems_count))))
		return (NULL);
	e = arr->elements;
	i = 0;
	while (e)
	{
		if (e->value->type != JSON_OBJECT
			|| !(tex[i++] = load_json_texture((t_json_object *)e->value)))
		{
			ft_putstr("Invalid texture at index: "); //TODO: clean previous loaded textures
			ft_putnbr(i);
			ft_putchar('\n');
			free(tex);
			return (NULL);
		}
		e = e->next;
	}
	world->textures_count = arr->elems_count;
	return (tex);
}

static t_block		**load_blocks(t_world *world, t_json_object *obj)
{
	t_json_array	*arr;
	t_json_element	*e;
	t_block			**blocks;
	int				i;

	if (!(arr = json_get_array(obj, "blocks")) ||
		!(blocks = (t_block **)malloc(sizeof(t_block *) * (arr->elems_count))))
		return (NULL);
	e = arr->elements;
	i = 0;
	while (e)
	{
		if (e->value->type != JSON_OBJECT
		|| !(blocks[i++] = load_json_block(world, (t_json_object *)e->value)))
		{
			ft_putstr("Invalid block at index: ");//TODO: clean previous loaded blocks
			ft_putnbr(i - 1);
			ft_putchar('\n');
			free(blocks);
			return (NULL);
		}
		e = e->next;
	}
	world->blocks_count = arr->elems_count;
	return (blocks);
}

static t_bool		load_map(t_world *world, t_json_object *obj)
{
	double	*texture_i;

	if (!(obj = json_get_object(obj, "map")))
		return (FALSE);
	if (!ft_json_vec3(json_object_get(obj, "size"), &world->size))
		return (FALSE);
	printf("Size %f %f\n", world->size.x, world->size.y);
	world->data = load_map_data(world, json_object_get(obj, "data"));
	if (!(texture_i = json_get_number(obj, "ceil_texture")))
		return (FALSE);
	world->ceil = world->textures[(int)*texture_i];
	if (!(texture_i = json_get_number(obj, "floor_texture")))
		return (FALSE);
	world->floor = world->textures[(int)*texture_i];
	return (TRUE);
}

void				load_world(t_world *world, char *file)
{
	t_json_state	state;
	t_json_value	*val;
	int				content_len;
	const char		*content = get_file_content(file, &content_len);

	state = (t_json_state){(char *)content, 0, content_len};
	if (!content || !(val = parse_value(&state)))
	{
		printf("Invalid file\n");
		if (content)
			free((void *)content);
		return ;
	}
	free((void *)content);
	if (!(world->textures = load_textures(world, (t_json_object *)val)))
		return ;
	if (!(world->blocks = load_blocks(world, (t_json_object *)val)))
		return ;
	load_map(world, (t_json_object *)val);
	json_free_value(val);
}
