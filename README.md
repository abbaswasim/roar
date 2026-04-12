![Build Status](https://github.com/abbaswasim/roar/actions/workflows/build.yml/badge.svg)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/95539c02467442fcb9ebc97c50a1a853)](https://www.codacy.com/manual/abbaswasim/roar?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=abbaswasim/roar&amp;utm_campaign=Badge_Grade)

# Roar

Roar is my personal rendering engine. Hopefully it will support GL/GLES, Vulkan and Metal at some point :)

## What is this about

A playground, where I have freedom of getting dirty and creative.

## Platforms

* GLFW
  * MacOS
  * Linux
  * Windows (Eventually)
* Android
* iOS

## Build

Roar can be built using `CMake` minimum version 3.6.

```bash
git clone --recurse-submodules https://github.com/abbaswasim/roar.git && cd roar && mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j8
```
## Roar conventions

Roar uses colum major matrices this means the following Matrix4 = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15] has 0, 1, 2, 3 as the first colum of the matrix and 3, 4, 5, 6 as the second. Origin of the matrix is at 12, 13, 14 which is also translation.

| 0 | 4 | 8  | 12 |
| 1 | 5 | 9  | 13 |
| 2 | 6 | 10 | 14 |
| 3 | 7 | 11 | 15 |

To transform a vector using this matrix you have to post-multiply it

```c++
Matrix4 mvp;
Vector3 pos;

auto new_pos = mvp * pos;
```
If you pre-multiply position you will need to transpose the matrix to get the same result

```c++
auto same_as_new_pos = pos * mvpT;
```
### Degrees vs Radians

There is no concept of degrees in Roar. You can only use radians. If any method is called with degrees the result will be wrong.

### Order of euler angles

Roar uses NASA standard aeroplane conventions as described on page: [Euler angles](https://www.euclideanspace.com/maths/geometry/rotations/euler/index.htm)

* Coordinate System: right hand
* Positive angle: right hand
* Order of euler angles: heading first, then attitude, then bank (YZX)
  * heading=Y
  * attitude=Z
  * bank=X

### How does roar save transform data for reach scene and model nodes
Roar uses two buffers one called `nodes_models` for the whole scene and another called `nodes_offsets` per draw call.

* Nodes Models SSBO
`nodes_models` is a single big GPU array of matrices that looks like `mat4 node_model[]` in shaders and its defined in `renderer.json`. It contains the resolved (global/world) transform for every node that can be drawn/skinned/morphed. Roar packs them linearly (scene nodes first, then model-instance nodes), and updates them each frame (typically via the node-transform compute pass).

* Nodes Offsets
`nodes_offsets` is a tiny per-renderable index/offset UBO that looks like `uvec4 node_offset` in shaders and is defined in renderer.json as a template. However each `node_data` for scene as well as model nodes contain its own copy. It stores indices into other big arrays. Roar fills it per node like below:

* node_offset.x = “resolved node index” into nodes_models.node_model[] for this node.
* node_offset.y = “base index of this model instance’s node block” inside nodes_models.
* node_offset.z = “base index into the global morph weights array” for this mesh’s morph targets.
* node_offset.w = currently unused.

This data lives in NodeData’s own `nodes_offsets` buffer and gets written by NodeData::update_offsets(). This NodeData's `nodes_offset` is an instantiation of renderer's nodes_offset buffer template. Shaders can then use it like below to access the specific model matrix.

```c++
 uint node_index = in_nodes_offsets.node_offset.x;
 vertex_position = in_nodes_models.node_model[node_index] * vec4(vertex_position.xyz, 1.0);
```

Roar renders each node/mesh as a separate draw, and binds that node’s nodes_offsets before drawing it.

`node_offset.y` is used for skinning and its joint matrices. Roar stores joint transforms inside the same `nodes_models.node_model[]` array as everything else. The skin’s `joint_redirect[]` values are indices of joint nodes within the model’s node list stored as uint16_t. But `nodes_models` is a global packed array (scene + all model instances). So you need a per-instance base to “relocate” those joint indices into the packed array.

That’s exactly what this line does:
```c++
 joint_global_index = joint_redirect[local_joint] + node_offset.y
```

and it becomes like so in skinned shaders:
```c++
 inverse_transform  = inverse(in_nodes_models.node_model[node_offset.x]);                //(mesh node’s global matrix inverse)
 joint_transform    = in_nodes_models.node_model[joint_redirect[index] + node_offset.y]; //(joint node’s global matrix)
 joint_transform    = inverse_transform * joint_transform * joint_inverse_bind;
```

`node_offset.z` is used for morph targets. Morph weights are also stored in one packed array `morphs_weights.morph_weights[]``, so each mesh with morphs needs a base offset. The shader generator uses:

```c++
 uint weight_offset = in_nodes_offsets.node_offset.z;
 morph_weights[weight_offset + i].
```

### To replicate a github actions build error locally

```bash
# First pull the Image where the issue is, something like
$ docker pull ghcr.io/catthehacker/ubuntu:runner-22.04

# Create a container from the image and run bash in it
$ docker run --rm -it --entrypoint bash <image_id_from_the_pulled_images>

# Install the required things:
$ sudo apt-get update && sudo apt install xorg-dev libglu1-mesa-dev cmake vim

# Build normally
$ cmake -H"." -B"build/ubuntu/Vulkan/Debug" -DCMAKE_BUILD_TYPE="Debug" -DROAR_RENDER_TYPE="Vulkan" -DROAR_BUILD_TESTS=1 -DROAR_BUILD_EDITOR=1
$ cmake --build "build/ubuntu/Vulkan/Debug" --config Debug -- -j4

```

## Third party

Roar uses the following third party software as submodules. These come with their own licenses.

* [glfw] (https://github.com/glfw/glfw)
* [spdlog] (https://github.com/gabime/spdlog)
* [nlohmann] (https://github.com/nlohmann/json)
* [xxHash] (https://github.com/Cyan4973/xxHash)
* [CImg] (http://www.cimg.eu)
* [stb] (https://github.com/nothings/stb)
* [Vulkan Headers] (https://github.com/KhronosGroup/Vulkan-Headers)
* [MetalCPP] (https://developer.apple.com/metal/cpp/)
* [glslang] (https://github.com/KhronosGroup/glslang)
* [basis_universal] (https://github.com/BinomialLLC/basis_universal)
* [par] (https://github.com/prideout/par.git)
* [cgltf] (https://github.com/jkuhlmann/cgltf.git)
* [CTPL] (https://github.com/vit-vit/CTPL.git)
* [Dear ImGui] (https://github.com/ocornut/imgui.git)
* [SPIRV-Cross] (https://github.com/KhronosGroup/SPIRV-Cross.git)
* [gtest] (https://github.com/google/googletest.git)

## License

The code is licensed under MIT.

## Project web sites:
[www.waZim.com](http://www.waZim.com/)
