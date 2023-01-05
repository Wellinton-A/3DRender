#include "mesh.h"
#include "array.h"
#include "triangle.h"
#include "vector.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = { 0, 0, 0}
};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
    {.x = -1, .y = -1, .z = -1},
    {.x = -1, .y =  1, .z = -1},
    {.x =  1, .y =  1, .z = -1},
    {.x =  1, .y = -1, .z = -1},
    {.x =  1, .y =  1, .z =  1},
    {.x =  1, .y = -1, .z =  1},
    {.x = -1, .y =  1, .z =  1},
    {.x = -1, .y = -1, .z =  1},
};

face_t cube_faces[N_CUBE_FACES] = {
    //Front
    {.a = 1, .b = 2, .c = 3},
    {.a = 1, .b = 3, .c = 4},
    //Right
    {.a = 4, .b = 3, .c = 5},
    {.a = 4, .b = 5, .c = 6},
    //Back
    {.a = 6, .b = 5, .c = 7},
    {.a = 6, .b = 7, .c = 8},
    //Left
    {.a = 8, .b = 7, .c = 2},
    {.a = 8, .b = 2, .c = 1},
    //Top
    {.a = 2, .b = 7, .c = 5},
    {.a = 2, .b = 5, .c = 3},
    //Bottom
    {.a = 6, .b = 8, .c = 1},
    {.a = 6, .b = 1, .c = 4},
};

void load_cube_mesh_data(void) {
    for (int i = 0; i < N_CUBE_VERTICES; i++) {
        vec3_t cube_vertex = cube_vertices[i];
        array_push(mesh.vertices, cube_vertex);
    }

    for (int i = 0; i < N_CUBE_FACES; i++) {
        face_t cube_face = cube_faces[i];
        array_push(mesh.faces, cube_face);
    }
}

void load_obj_mesh_data(char* filename) {
    FILE* fp;
    fp = fopen(filename, "r");

    char line[1024];

    while (fgets(line, 1024, fp)) {
        if (strncmp(line, "v ", 2) == 0) {
            vec3_t vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            array_push(mesh.vertices, vertex)
        }

        if (strncmp(line, "f ", 2) == 0) {
            face_t face;
            face_t bin;
            face_t bin2;
            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                    &face.a, &bin.a, &bin2.a,
                    &face.b, &bin.b, &bin2.b,
                    &face.c, &bin.c, &bin2.c);
            array_push(mesh.faces, face)
        }
    }
}
