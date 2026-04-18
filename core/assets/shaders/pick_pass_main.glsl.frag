layout(location = 0) out uint out_index;

vec4 base_color()
{
	return @
}
@
void main()
{
	vec4 color = base_color();
	@
	if (color.a < 0.3)
		discard;

	out_index = in_nodes_offsets.node_offset.x + 1; // 1 based indexed for picking so we can use 0-based index as a nothing selected signal
}
