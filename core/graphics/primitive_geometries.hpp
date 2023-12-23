// clang-format off
// Based on https://github.com/KhronosGroup/Vulkan-Tools/blob/main/cube/cube.c
static const float cube_vertex_buffer_interleaved[] = {
    -1.0f,-1.0f,-1.0f,    0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 0.5f,    // -X side
    -1.0f,-1.0f, 1.0f,    1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 0.5f,
    -1.0f, 1.0f, 1.0f,    1.0f, 0.0f,    1.0f, 0.0f, 0.0f, 0.5f,
    -1.0f, 1.0f, 1.0f,    1.0f, 0.0f,    1.0f, 0.0f, 0.0f, 0.5f,
    -1.0f, 1.0f,-1.0f,    0.0f, 0.0f,    1.0f, 0.0f, 0.0f, 0.5f,
    -1.0f,-1.0f,-1.0f,    0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 0.5f,
    -1.0f,-1.0f,-1.0f,    1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 0.5f,    // -Z side
     1.0f, 1.0f,-1.0f,    0.0f, 0.0f,    0.0f, 1.0f, 0.0f, 0.5f,
     1.0f,-1.0f,-1.0f,    0.0f, 1.0f,    0.0f, 1.0f, 0.0f, 0.5f,
    -1.0f,-1.0f,-1.0f,    1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 0.5f,
    -1.0f, 1.0f,-1.0f,    1.0f, 0.0f,    0.0f, 1.0f, 0.0f, 0.5f,
     1.0f, 1.0f,-1.0f,    0.0f, 0.0f,    0.0f, 1.0f, 0.0f, 0.5f,
    -1.0f,-1.0f,-1.0f,    1.0f, 0.0f,    0.0f, 0.0f, 1.0f, 0.5f,    // -Y side
     1.0f,-1.0f,-1.0f,    1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 0.5f,
     1.0f,-1.0f, 1.0f,    0.0f, 1.0f,    0.0f, 0.0f, 1.0f, 0.5f,
    -1.0f,-1.0f,-1.0f,    1.0f, 0.0f,    0.0f, 0.0f, 1.0f, 0.5f,
     1.0f,-1.0f, 1.0f,    0.0f, 1.0f,    0.0f, 0.0f, 1.0f, 0.5f,
    -1.0f,-1.0f, 1.0f,    0.0f, 0.0f,    0.0f, 0.0f, 1.0f, 0.5f,
    -1.0f, 1.0f,-1.0f,    1.0f, 0.0f,    1.0f, 1.0f, 0.0f, 0.5f,    // +Y side
    -1.0f, 1.0f, 1.0f,    0.0f, 0.0f,    1.0f, 1.0f, 0.0f, 0.5f,
     1.0f, 1.0f, 1.0f,    0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 0.5f,
    -1.0f, 1.0f,-1.0f,    1.0f, 0.0f,    1.0f, 1.0f, 0.0f, 0.5f,
     1.0f, 1.0f, 1.0f,    0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 0.5f,
     1.0f, 1.0f,-1.0f,    1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 0.5f,
     1.0f, 1.0f,-1.0f,    1.0f, 0.0f,    1.0f, 0.0f, 1.0f, 0.5f,    // +X side
     1.0f, 1.0f, 1.0f,    0.0f, 0.0f,    1.0f, 0.0f, 1.0f, 0.5f,
     1.0f,-1.0f, 1.0f,    0.0f, 1.0f,    1.0f, 0.0f, 1.0f, 0.5f,
     1.0f,-1.0f, 1.0f,    0.0f, 1.0f,    1.0f, 0.0f, 1.0f, 0.5f,
     1.0f,-1.0f,-1.0f,    1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 0.5f,
     1.0f, 1.0f,-1.0f,    1.0f, 0.0f,    1.0f, 0.0f, 1.0f, 0.5f,
    -1.0f, 1.0f, 1.0f,    0.0f, 0.0f,    1.0f, 1.0f, 1.0f, 0.5f,    // +Z side
    -1.0f,-1.0f, 1.0f,    0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 0.5f,
     1.0f, 1.0f, 1.0f,    1.0f, 0.0f,    1.0f, 1.0f, 1.0f, 0.5f,
    -1.0f,-1.0f, 1.0f,    0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 0.5f,
     1.0f,-1.0f, 1.0f,    1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 0.5f,
     1.0f, 1.0f, 1.0f,    1.0f, 0.0f,    1.0f, 1.0f, 1.0f, 0.5f
};

static const float cube_vertex_position_uv_interleaved[] = {
    -1.0f,-1.0f,-1.0f,    0.0f, 1.0f,    // -X side
    -1.0f,-1.0f, 1.0f,    1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,    1.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,    1.0f, 0.0f,
    -1.0f, 1.0f,-1.0f,    0.0f, 0.0f,
    -1.0f,-1.0f,-1.0f,    0.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,    1.0f, 1.0f,    // -Z side
     1.0f, 1.0f,-1.0f,    0.0f, 0.0f,
     1.0f,-1.0f,-1.0f,    0.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,    1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,    1.0f, 0.0f,
     1.0f, 1.0f,-1.0f,    0.0f, 0.0f,
    -1.0f,-1.0f,-1.0f,    1.0f, 0.0f,    // -Y side
     1.0f,-1.0f,-1.0f,    1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,    0.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,    1.0f, 0.0f,
     1.0f,-1.0f, 1.0f,    0.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,    0.0f, 0.0f,
    -1.0f, 1.0f,-1.0f,    1.0f, 0.0f,    // +Y side
    -1.0f, 1.0f, 1.0f,    0.0f, 0.0f,
     1.0f, 1.0f, 1.0f,    0.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,    1.0f, 0.0f,
     1.0f, 1.0f, 1.0f,    0.0f, 1.0f,
     1.0f, 1.0f,-1.0f,    1.0f, 1.0f,
     1.0f, 1.0f,-1.0f,    1.0f, 0.0f,    // +X side
     1.0f, 1.0f, 1.0f,    0.0f, 0.0f,
     1.0f,-1.0f, 1.0f,    0.0f, 1.0f,
     1.0f,-1.0f, 1.0f,    0.0f, 1.0f,
     1.0f,-1.0f,-1.0f,    1.0f, 1.0f,
     1.0f, 1.0f,-1.0f,    1.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,    0.0f, 0.0f,    // +Z side
    -1.0f,-1.0f, 1.0f,    0.0f, 1.0f,
     1.0f, 1.0f, 1.0f,    1.0f, 0.0f,
    -1.0f,-1.0f, 1.0f,    0.0f, 1.0f,
     1.0f,-1.0f, 1.0f,    1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,    1.0f, 0.0f
};

static const float cube_vertex_buffer_position[] = {
    -1.0f,-1.0f,-1.0f,    // -X side
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,    // -Z side
     1.0f, 1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
     1.0f, 1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,    // -Y side
     1.0f,-1.0f,-1.0f,
     1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
     1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,    // +Y side
    -1.0f, 1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f, 1.0f,-1.0f,
     1.0f, 1.0f,-1.0f,    // +X side
     1.0f, 1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,
     1.0f,-1.0f,-1.0f,
     1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,    // +Z side
    -1.0f,-1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,
     1.0f, 1.0f, 1.0f
};

static const float cube_vertex_buffer_uv[] = {
    0.0f, 1.0f,    // -X side
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,    // -Z side
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,    // -Y side
    1.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,    // +Y side
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,    // +X side
    0.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,    // +Z side
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
};

static const float cube_vertex_buffer_color_rgb8_unorm[] = {
    1.0f, 0.0f, 0.0f, 1.0f,    // -X side
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,    // -Z side
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,    // -Y side
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,    // +Y side
    1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f,    // +X side
    1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,    // +Z side
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f
};

static const unsigned char cube_vertex_buffer_color_rgb8[] = {
    255,   0,   0, 255,    // -X side
    255,   0,   0, 255,
    255,   0,   0, 255,
    255,   0,   0, 255,
    255,   0,   0, 255,
    255,   0,   0, 255,
      0, 255,   0, 255,    // -Z side
      0, 255,   0, 255,
      0, 255,   0, 255,
      0, 255,   0, 255,
      0, 255,   0, 255,
      0, 255,   0, 255,
      0,   0, 255, 255,    // -Y side
      0,   0, 255, 255,
      0,   0, 255, 255,
      0,   0, 255, 255,
      0,   0, 255, 255,
      0,   0, 255, 255,
    255, 255,   0, 255,    // +Y side
    255, 255,   0, 255,
    255, 255,   0, 255,
    255, 255,   0, 255,
    255, 255,   0, 255,
    255, 255,   0, 255,
    255,   0, 255, 255,    // +X side
    255,   0, 255, 255,
    255,   0, 255, 255,
    255,   0, 255, 255,
    255,   0, 255, 255,
    255,   0, 255, 255,
    255, 255, 255, 255,    // +Z side
    255, 255, 255, 255,
    255, 255, 255, 255,
    255, 255, 255, 255,
    255, 255, 255, 255,
    255, 255, 255, 255
};

static const unsigned short cube_index_buffer_uint16[] = {
    0,     // -X side
    1,
    2,
    3,
    4,
    5,
    6,     // -Z side
    7,
    8,
    9,
    10,
    11,
    12,    // -Y side
    13,
    14,
    15,
    16,
    17,
    18,    // +Y side
    19,
    20,
    21,
    22,
    23,
    24,    // +X side
    25,
    26,
    27,
    28,
    29,
    30,    // +Z side
    31,
    32,
    33,
    34,
    35
};

static const float scale{2.0f};
static const float location{0.0f};
static const float quad_vertex_buffer_interleaved[] = {
    -scale, -scale,  location,     0.0f,  0.0f,
     scale, -scale,  location,     1.0f,  0.0f,
    -scale,  scale,  location,     0.0f,  1.0f,
    -scale,  scale,  location,     0.0f,  1.0f,
     scale, -scale,  location,     1.0f,  0.0f,
     scale,  scale,  location,     1.0f,  1.0f
};

static const float quad_vertex_buffer_positions[] = {
    -scale, -scale,  location,
     scale, -scale,  location,
    -scale,  scale,  location,
    -scale,  scale,  location,
     scale, -scale,  location,
     scale,  scale,  location
};

static const float scale2{0.01f};
static const float quad_vertex_buffer_positions_centered[] = {
    -scale2, -scale2,  location,
     scale2, -scale2,  location,
    -scale2,  scale2,  location,
    -scale2,  scale2,  location,
     scale2, -scale2,  location,
     scale2,  scale2,  location
};

// clang-format on