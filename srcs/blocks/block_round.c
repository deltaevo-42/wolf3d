#include "wolf.h"

t_block		*load_round_block(t_world *w, t_json_object *obj)
{
	t_block_round	*block;
	t_json_value	*val;

	if (!(block = (t_block_round *)malloc(sizeof(t_block_round))))
		return (NULL);
	block->super.type = B_ROUND;
	if (!ft_json_color(json_object_get(obj, "minimap_color"), &block->minimap_color))
		return (json_free_ret(block));
	val = json_object_get(obj, "height");
	block->super.height = (!val || val->type != JSON_NUMBER ? 1 : ((t_json_number *)val)->value);
	val = json_object_get(obj, "radius");
	block->radius = (!val || val->type != JSON_NUMBER ? 0.5 : ((t_json_number *)val)->value);
	return ((t_block *)block);
}