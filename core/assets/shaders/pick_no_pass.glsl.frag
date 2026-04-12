#version 460

layout(early_fragment_tests) in;

layout(location = 0) flat in uint v_object_id;
layout(location = 1) in float v_opacity;

layout(location = 0) in vec4 in_vertex_color_0;
layout(location = 0) out vec4 out_color;


layout(set = 0, binding = 0, std140) uniform PickingParams
{
    uvec2 cursor_pos;
    float opacity_threshold;
} u_picking;

layout(set = 0, binding = 1, std430) buffer PickingBuffer
{
    uint depth_and_lock;
    uint object_id;
} g_pick;

const uint PICK_LOCK_BIT   = 0x80000000u;
const uint PICK_DEPTH_MASK = 0x7fffffffu;

void update_picking_buffer(ivec2 pixel_pos, uint object_id, float z, float opacity)
{
    if (opacity < u_picking.opacity_threshold)
        return;

    if (any(notEqual(uvec2(pixel_pos), u_picking.cursor_pos)))
        return;

    // Keep only the lower 31 bits for the depth payload.
    uint d = floatBitsToUint(z) & PICK_DEPTH_MASK;

    for (;;)
    {
        uint observed = g_pick.depth_and_lock;

        // Someone else owns the lock, keep spinning.
        if ((observed & PICK_LOCK_BIT) != 0u)
            continue;

        uint current_depth = observed & PICK_DEPTH_MASK;

        // Larger z means farther away in Vulkan depth space.
        if (d >= current_depth)
            return;

        // Try to acquire lock and publish our candidate depth.
        uint locked_value = d | PICK_LOCK_BIT;
        uint prev = atomicCompSwap(g_pick.depth_and_lock, observed, locked_value);

        if (prev == observed)
        {
            // We now own the lock.
            g_pick.object_id = object_id;

            // Ensure ID write is visible before unlock.
            memoryBarrierBuffer();

            // Unlock and finalize the winning depth.
            atomicExchange(g_pick.depth_and_lock, d);
            return;
        }
    }
}

void main()
{
    update_picking_buffer(ivec2(gl_FragCoord.xy), v_object_id, gl_FragCoord.z, v_opacity);
    out_color = in_vertex_color_0;
}