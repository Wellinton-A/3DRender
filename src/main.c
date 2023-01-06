#include <SDL2/SDL_timer.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"
#include "array.h"

triangle_t* triangle_to_render =NULL;

vec3_t camera_position = { 0,0,-5};

float fov_factor = 628;

bool is_running = false;
int previous_frame_time = 0;

void setup(void) {
    //Allocate the required memory in bytes to hold the color buffer
    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
    //creating a texture to copy the color buffer
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width, 
        window_height
        );
    load_obj_mesh_data("./assets/f22.obj");
}

// function that receives a 3d vector and return a projected 2d point
vec2_t project(vec3_t point) {
    vec2_t projected_point = {
        .x = (fov_factor * point.x) / point.z,
        .y = (fov_factor * point.y) / point.z,
    };
    return projected_point;
}

void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                is_running = false;
            break;
    }
}

void update(void) {
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks64() - previous_frame_time);

    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    triangle_to_render = NULL;

    previous_frame_time = SDL_GetTicks64();
  
    mesh.rotation.x += 0.01;
    mesh.rotation.y += 0.00;
    mesh.rotation.z += 0.00;

    //Loop all triangle faces of our meshes
    int face_length = array_length(mesh.faces);
    for (int i = 0; i < face_length; i++) {
        face_t mesh_face = mesh.faces[i];

        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices[mesh_face.a - 1];
        face_vertices[1] = mesh.vertices[mesh_face.b - 1];
        face_vertices[2] = mesh.vertices[mesh_face.c - 1];

        triangle_t projected_triangle;
        //Loop all three vertices of the current face and aply transformations
        for (int j = 0; j < 3; j++) {
            vec3_t transformed_vertex = face_vertices[j];                

            transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
            transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
            transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

            //Translate the points away from the camera
            transformed_vertex.z -= camera_position.z;
            //project the current vertices
            vec2_t projected_point = project(transformed_vertex);

            //Scale and center
            projected_point.x += (float)window_width / 2,
            projected_point.y += (float)window_height / 2,

            projected_triangle.points[j] = projected_point;
        }
        //save the projected triangle in the array of triangle to render
        array_push(triangle_to_render, projected_triangle)

        
    }
}

void render(void) {

    int length_array = array_length(triangle_to_render);
    for (int i = 0; i < length_array; i++) {
        triangle_t triangle = triangle_to_render[i];

        draw_rect(triangle.points[0].x, triangle.points[0].y, 1, 1, 0xFFFFFF00);
        draw_rect(triangle.points[1].x, triangle.points[1].y, 1, 1, 0xFFFFFF00);
        draw_rect(triangle.points[2].x, triangle.points[2].y, 1, 1, 0xFFFFFF00);

        draw_line(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x, triangle.points[1].y, 0xFF6F7282);
        draw_line(triangle.points[1].x, triangle.points[1].y, triangle.points[2].x, triangle.points[2].y, 0xFF6F7282);
        draw_line(triangle.points[2].x, triangle.points[2].y, triangle.points[0].x, triangle.points[0].y, 0xFF6F7282);

        // draw_triangle(
        //     triangle.points[0].x, triangle.points[0].y,
        //     triangle.points[1].x, triangle.points[1].y,
        //     triangle.points[2].x, triangle.points[2].y,
        //     0xFFFFFFFF   
        // );
    }

    array_free(triangle_to_render);

    render_color_buffer();
    clear_color_buffer(0xff000000);
    
    SDL_RenderPresent(renderer);
}
// Free the memory dynamically allocated by the program
void free_resources(void) {
    free(color_buffer);
    array_free(mesh.vertices);
    array_free(mesh.faces);
}

int main(void) {

    is_running = initialize_window();

    setup();

    while (is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();
    free_resources();
    return 0;
}