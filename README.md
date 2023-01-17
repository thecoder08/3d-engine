# 3D Engine
This is a little "3D Engine" that is able to render a 3D cube. It's a software renderer written in C. It uses [libxgfx](https://github.com/thecoder08/xgfx) to display to an X11 window.

If you want to spin a cube, build the project with the command:
```sh
gcc demo.c rotate.c -lm -lxgfx
```
It requires libxgfx (which requires libX11) and libm (which you should have, because it's part of libc.) Use arrow keys and home/end to spin the cube.

Check out [demo.c](demo.c) to see the code that displays the cube, or [rotate.c](rotate.c) to see the code which does the linear algebra needed to rotate the cube.