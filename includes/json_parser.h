/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_parser.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 12:19:06 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/16 22:44:14 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JSON_PARSER_H
# define JSON_PARSER_H

#include <stddef.h>

typedef enum		e_bool
{
	TRUE = 1,
	FALSE = 0,
	TRUENT = 0,
	FALSENT = 1
}					t_bool;

typedef enum		e_json_types
{
	JSON_STRING,
	JSON_NUMBER,
	JSON_OBJECT,
	JSON_ARRAY,
	JSON_BOOLEAN,
	JSON_NULL,
	JSON_EOF
}					t_json_types;

typedef struct		s_json_value
{
	t_json_types	type;
}					t_json_value;

typedef struct		s_json_member
{
	t_json_value			*value;
	struct s_json_string	*string;
	struct s_json_member	*next;
}					t_json_member;

typedef struct		s_json_element
{
	t_json_value			*value;
	struct s_json_element	*next;
}					t_json_element;


struct				s_json_object
{
	t_json_value	super;
	t_json_member	*elements;
};

struct				s_json_array
{
	t_json_value	super;
	t_json_element	*elements;
};

struct				s_json_string
{
	t_json_value	super;
	size_t			value_len;
	char			value[];
};

struct				s_json_number
{
	t_json_value	super;
	double			value;
};

struct				s_json_boolean
{
	t_json_value	super;
	t_bool			value;
};

#endif
