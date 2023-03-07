# 3D Engine
This is a little "3D Engine" that is able to render 3D meshes. It's a software renderer written in C. It uses [libxgfx](https://github.com/thecoder08/xgfx) to display to an X11 window.

If you want to view a .obj mesh, Build the project with the command:
```sh
gcc demo.c rotate.c -lm -lxgfx -o 3dengine
```
then run with:
```sh
./3dengine [mesh.obj]
```
For example, display the Utah teapot:
```sh
./3dengine examples/teapot.obj
```
It requires libxgfx (which requires libX11) and libm (which you should have, because it's part of libc.) Use arrow keys and home/end to spin the mesh.

Check out [demo.c](demo.c) to see the code that displays the mesh, or [rotate.c](rotate.c) to see the code which does the linear algebra needed to rotate the mesh.