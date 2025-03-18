# 3D Engine
This is a little "3D Engine" that is able to render 3D meshes. It's a software renderer written in C. It uses [libxgfx](https://github.com/thecoder08/xgfx) to display to a window.

![Curling rock](https://github.com/thecoder08/3d-engine/blob/master/rock.png?raw=true)

If you want to view a .obj mesh, Build the project with the command:
```sh
make demo
```
then run with:
```sh
LD_LIBRARY_PATH=. ./demo [mesh.obj]
```
For example, display the Utah teapot:
```sh
LD_LIBRARY_PATH=. ./demo examples/teapot.obj
```
Or just run `make` to build and run the demo with a torus example mesh.

It requires libxgfx (which requires libxcb, libX11 and libwayland-client) and libm (which you should have, because it's part of libc.) Use arrow keys and home/end to spin the mesh.

Check out [engine.c](engine.c) to see the code that renders the mesh, or [rotate.c](rotate.c) to see the code which does the linear algebra needed to rotate the mesh. [demo.c] is the main code that loads and displays the meshes.
