/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/17 22:46:01 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/18 03:32:00 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include <fcntl.h>
#include <unistd.h>
#define BUFF_SIZE 1

static char	*get_file_content(char *file, int *content_len)
{
	int		fd;
	char	*content;
	char 	buf[BUFF_SIZE + 1];
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
	t_json_value	*val;
	t_json_array	*arr;
	t_json_element	*elem;
	t_texture		**textures;
	int				i;
	
	val = json_object_get(obj, "textures");
	if (!val || val->type != JSON_ARRAY)
		return (NULL);
	arr = (t_json_array *)val;
	if (!(textures = (t_texture **)malloc(sizeof(t_texture *) * (arr->elems_count))))
		return (NULL);
	elem = arr->elements;
	i = 0;
	while (elem)
	{
		if (elem->value->type != JSON_OBJECT 
			|| !(textures[i++] = load_json_texture((t_json_object *)elem->value)))
		{
			//TODO: clean previous loaded textures
			free(textures);
			return (NULL);
		}
		elem = elem->next;
	}
	world->textures_count = arr->elems_count;
	return (textures);
}

void		load_world(char *file)
{
	t_json_state	state;
	t_json_value	*val;
	int				content_len;
	const char		*content = get_file_content(file, &content_len);
	t_world			world;

	state = (t_json_state){(char *)content, 0, content_len};
	if (!content || !(val = parse_value(&state)))
	{
		printf("Invalid file\n");
		if (content)
			free((void *)content);
		return ;
	}
	free((void *)content);
	load_textures(&world, (t_json_object *)val);
	json_free_value(val);
}