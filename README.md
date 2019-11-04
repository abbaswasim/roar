[![Build Status](https://travis-ci.com/abbaswasim/roar.svg?branch=master)](https://travis-ci.com/abbaswasim/roar)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/95539c02467442fcb9ebc97c50a1a853)](https://www.codacy.com/manual/abbaswasim/roar?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=abbaswasim/roar&amp;utm_campaign=Badge_Grade)

# Roar

Roar is my personal rendering engine. Hopefully it will support GL/GLES, Vulkan and Metal at some point :)

## What is this about

This is my personal playground, where I have freedom of getting dirty and creative. I hope others can find it useful.

## Platforms

  - GLFW
	- MacOS
	- Linux
	- Windows (Eventually)

  - Android
  - iOS

## Build

Roar can be built using `CMake` minimum version 3.6.

```bash
git clone --recurse-submodules https://github.com/abbaswasim/roar.git && cd roar && mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j8
```
## Roar conventions

Roar uses colum major matrices this means the following Matrix4 = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15] has 0, 1, 2, 3 as the first colum of the matrix and 3, 4, 5, 6 as the second. Origin of the matrix is at 12, 13, 14 which is also translation.

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

  - Coordinate System: right hand
  - Positive angle: right hand
  - Order of euler angles: heading first, then attitude, then bank (YZX)
	- heading=Y
	- attitude=Z
	- bank=X

## Third party

Roar uses the following third party software as submodules.

- [GLFW](https://github.com/glfw/glfw)

## License

The code is licensed under MIT.

## Project web sites:
[www.waZim.com](http://www.waZim.com/)
