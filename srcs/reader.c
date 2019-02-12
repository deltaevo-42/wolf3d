/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 12:06:34 by llelievr          #+#    #+#             */
/*   Updated: 2019/01/16 11:31:47 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#define BUFF_SIZE 4096

t_map		*read_map(t_inst *inst, char *file)
{
	int				fd;
	char			buf[BUFF_SIZE + 1];
	char			*content;
	int				rd;
	t_map			map;

	(void)inst;
	if ((fd = open(file, O_RDONLY)) == -1)
		return (0);
	ft_bzero((void *)&map, sizeof(t_map));
	content = ft_strnew(0);
	while ((rd = read(fd, &buf, BUFF_SIZE)) > 0)
	{
		buf[rd] = '\0';
		ft_strcat(content, ft_strdup(buf));
	}
	printf("CONTENT %s", content);
	
	free(content);
	return (0);
}
