#include "rotate.h"
#define TINYOBJ_LOADER_C_IMPLEMENTATION 1
#include "tinyobj_loader_c.h"
#include <xgfx/window.h>
#include <xgfx/drawing.h>
#define __USE_MISC 1
#include <math.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// the below is from the tinyobjectloader-c on github
static void get_file_data(void* ctx, const char* filename, const int is_mtl,
                          const char* obj_filename, char** data, size_t* len) {
  // NOTE: If you allocate the buffer with malloc(),
  // You can define your own memory management struct and pass it through `ctx`
  // to store the pointer and free memories at clean up stage(when you quit an
  // app)
  // This example uses mmap(), so no free() required.
  (void)ctx;

  if (!filename) {
    (*data) = NULL;
    (*len) = 0;
    return;
  }
  struct stat sb;

  int fd = open(filename, O_RDONLY);

  fstat(fd, &sb);

  (*data) = (char*)mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

  (*len) = sb.st_size;
}

int main(int argc, char** argv) {
    initWindow(640, 480, "3D engine");

    // loads the specified object file
    tinyobj_attrib_t attrib;
    tinyobj_shape_t* shapes = NULL;
    size_t num_shapes;
    tinyobj_material_t* materials = NULL;
    size_t num_materials;
    unsigned int flags = TINYOBJ_FLAG_TRIANGULATE;
    if (tinyobj_parse_obj(&attrib, &shapes, &num_shapes, &materials, &num_materials, argv[1], get_file_data, NULL, flags) != TINYOBJ_SUCCESS) {
        printf("Failed to load object file!\n");
        return 0;
    }
    attrib.faces[0].v_idx;
    vec3 vertices[attrib.num_vertices];
    for (int i = 0; i < attrib.num_vertices; i++) {
        vertices[i][0] = attrib.vertices[i * 3];
        vertices[i][1] = attrib.vertices[i * 3 + 1];
        vertices[i][2] = attrib.vertices[i * 3 + 2];
    }

    float angleX = 0;
    float angleY = 0;
    float angleZ = 0;
    int upPressed = 0;
    int downPressed = 0;
    int leftPressed = 0;
    int rightPressed = 0;
    int homePressed = 0;
    int endPressed = 0;
    while(1) {
        XEvent event;
        int result = checkWindowEvents(&event);
        if (result == 2) {
            break;
        }
        if (result == 1) {
            if (event.type == KeyPress) {
                if (event.xkey.keycode == 111) {
                    upPressed = 1;
                }
                if (event.xkey.keycode == 116) {
                    downPressed = 1;
                }
                if (event.xkey.keycode == 113) {
                    leftPressed = 1;
                }
                if (event.xkey.keycode == 114) {
                    rightPressed = 1;
                }
                if (event.xkey.keycode == 110) {
                    homePressed = 1;
                }
                if (event.xkey.keycode == 115) {
                    endPressed = 1;
                }
            }
            if (event.type == KeyRelease) {
                if (event.xkey.keycode == 111) {
                    upPressed = 0;
                }
                if (event.xkey.keycode == 116) {
                    downPressed = 0;
                }
                if (event.xkey.keycode == 113) {
                    leftPressed = 0;
                }
                if (event.xkey.keycode == 114) {
                    rightPressed = 0;
                }
                if (event.xkey.keycode == 110) {
                    homePressed = 0;
                }
                if (event.xkey.keycode == 115) {
                    endPressed = 0;
                }
            }
        }

        if (upPressed) {
            angleX += 0.0625;
        }
        if (downPressed) {
            angleX -= 0.0625;
        }
        if (leftPressed) {
            angleY += 0.0625;
        }
        if (rightPressed) {
            angleY -= 0.0625;
        }
        if (homePressed) {
            angleZ += 0.0625;
        }
        if (endPressed) {
            angleZ -= 0.0625;
        }
        if (angleX >= M_PI * 2) {
                angleX -= M_PI * 2;
        }
        if (angleY >= M_PI * 2) {
                angleX -= M_PI * 2;
        }
        if (angleZ >= M_PI * 2) {
                angleX -= M_PI * 2;
        }
        vec3 transformedVertices[attrib.num_vertices];
        for (int i = 0; i < attrib.num_vertices; i++) {
            vec3 tmp;
            vec3 tmp2;
            rotateX(vertices[i], angleX, tmp);
            rotateY(tmp, angleY, tmp2);
            rotateZ(tmp2, angleZ, transformedVertices[i]);
        }
        rectangle(0, 0, 640, 480, 0x00000000);
        for (int i = 0; i < attrib.num_faces; i += 3) {
            line((int)round(transformedVertices[attrib.faces[i].v_idx][0] * 120) + 320, (int)round(transformedVertices[attrib.faces[i].v_idx][1] * -120) + 240, (int)round(transformedVertices[attrib.faces[i + 1].v_idx][0] * 120) + 320, (int)round(transformedVertices[attrib.faces[i + 1].v_idx][1] * -120) + 240, 0x0000ffff);
            line((int)round(transformedVertices[attrib.faces[i].v_idx][0] * 120) + 320, (int)round(transformedVertices[attrib.faces[i].v_idx][1] * -120) + 240, (int)round(transformedVertices[attrib.faces[i + 2].v_idx][0] * 120) + 320, (int)round(transformedVertices[attrib.faces[i + 2].v_idx][1] * -120) + 240, 0x0000ffff);
            line((int)round(transformedVertices[attrib.faces[i + 2].v_idx][0] * 120) + 320, (int)round(transformedVertices[attrib.faces[i + 2].v_idx][1] * -120) + 240, (int)round(transformedVertices[attrib.faces[i + 1].v_idx][0] * 120) + 320, (int)round(transformedVertices[attrib.faces[i + 1].v_idx][1] * -120) + 240, 0x0000ffff);
        }
        updateWindow();
        usleep(16667);
    }
    return 0;
}