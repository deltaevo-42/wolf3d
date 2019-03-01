/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 14:32:51 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/01 17:20:25 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

static t_block_type	get_block_type(char *string)
{
	const int	len = ft_strlen(string);

	if (len == 6 && ft_strncmp(string, "NORMAL", 6) == 0)
		return (B_NORMAL);
	if (len == 5 && ft_strncmp(string, "ROUND", 5) == 0)
		return (B_ROUND);
	return (B_NONE);
}

t_face				get_face_type(char *string)
{
	const int	len = ft_strlen(string);

	if (len == 5 && ft_strncmp(string, "NORTH", 5) == 0)
		return (F_NORTH);
	if (len == 5 && ft_strncmp(string, "SOUTH", 5) == 0)
		return (F_SOUTH);
	if (len == 4 && ft_strncmp(string, "WEST", 4) == 0)
		return (F_WEST);
	if (len == 4 && ft_strncmp(string, "EAST", 4) == 0)
		return (F_EAST);
	if (len == 3 && ft_strncmp(string, "TOP", 3) == 0)
		return (F_TOP);
	if (len == 6 && ft_strncmp(string, "BOTTOM", 6) == 0)
		return (F_BOTTOM);
	if (len == 3 && ft_strncmp(string, "ALL", 3) == 0)
		return (F_ALL);
	return (F_NONE);
}

t_block				*load_json_block(t_world *world, t_json_object *obj)
{
	t_json_string	*j_string;
	t_block_type	block_type;

	if (!(j_string = json_get_string(obj, "type")))
		return (NULL);
	block_type = get_block_type(j_string->value);
	if (block_type == B_NONE)
	{
		ft_putstr("Unknown block type: ");
		ft_putendl(j_string->value);
		return (NULL);
	}
	if (block_type == B_NORMAL)
		return (load_normal_block(world, obj));
	if (block_type == B_ROUND)
		return (load_round_block(world, obj));
	return (NULL);
}
