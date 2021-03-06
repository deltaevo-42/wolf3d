/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/17 22:46:01 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/05 14:00:49 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include <fcntl.h>
#include <unistd.h>
#define BUFF_SIZE 2048

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

static t_texture	**load_textures(t_wolf *wolf, t_json_object *obj)
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
			ft_putstr("Invalid texture at index: ");
			ft_putnbr(i - 1);
			ft_putchar('\n');
			unload_textures(tex, i - 1);
			return (NULL);
		}
		e = e->next;
	}
	wolf->textures_count = arr->elems_count;
	return (tex);
}

static t_block		**load_blocks(t_wolf *wolf, t_json_object *obj)
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
		|| !(blocks[i++] = load_json_block(wolf, (t_json_object *)e->value)))
		{
			ft_putstr("Invalid block at index: ");
			ft_putnbr(i - 1);
			ft_putchar('\n');
			unload_blocks(blocks, i - 1);
			return (NULL);
		}
		e = e->next;
	}
	wolf->blocks_count = arr->elems_count;
	return (blocks);
}

t_bool				load_config(t_wolf *wolf, char *file)
{
	t_json_state	state;
	t_json_value	*val;
	int				content_len;
	const char		*content = get_file_content(file, &content_len);

	state = (t_json_state){(char *)content, 0, content_len};
	if (!content || !(val = parse_value(&state)))
	{
		ft_putendl("Invalid file");
		if (content)
			free((void *)content);
		return (FALSE);
	}
	free((void *)content);
	if (val->type != JSON_OBJECT
		|| !(wolf->textures = load_textures(wolf, (t_json_object *)val))
		|| !(wolf->blocks = load_blocks(wolf, (t_json_object *)val))
		|| !load_map(wolf, &wolf->world,
				json_get_object((t_json_object *)val, "map")))
	{
		json_free_value(val);
		return (FALSE);
	}
	json_free_value(val);
	return (TRUE);
}
