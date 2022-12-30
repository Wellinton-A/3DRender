#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h>

bool is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int window_width = 800;
int window_height = 600;

SDL_Texture* color_buffer_texture = NULL;
uint32_t* color_buffer = NULL;

bool initialize_window(void) {
    //Initializating a SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    //Use SDL to query what is tha full screen max. width and height
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    window_width = display_mode.w;
    window_height = display_mode.h;
    //Create a SDL Window
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }
    //Create a SDL renderer
    renderer = SDL_CreateRenderer(
        window,
        -1,
        0
    );
    if (!renderer) {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}

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

}

void render_color_buffer(void) {
    SDL_UpdateTexture(
    color_buffer_texture,
    NULL,
    color_buffer,
    (window_width * sizeof(uint32_t))
    );
    SDL_RenderCopy(
    renderer,
    color_buffer_texture,
    NULL,
    NULL
    );
}

void draw_grid(void) {
    for (int y = 0; y < window_height; y += 10) {
        for (int x = 0; x < window_width; x++) {
            color_buffer[(window_width * y) + x] = 0xffff3333;
        }
    }
    for (int y = 0; y < window_height; y ++) {
        for (int x = 0; x < window_width; x += 10) {
            color_buffer[(window_width * y) + x] = 0xffff3333;
        }
    }
}

// void draw_grid(void) {
//     for (int y = 0; y < window_height; y ++) {
//         for (int x = 0; x < window_width; x++) {
//             if ( x % 10 == 0 || y % 10 ==0) {
//                 color_buffer[(window_width * y) + x] = 0xff000000;
//             }
//         }
//     }
// }

void draw_rect(int x, int y, int width, int height, uint32_t color) {
    for (int i = x; i < (width + x); i++) {
        for (int j = y; j < (height + y); j++) {
            color_buffer[(window_width * j) + i] = color;
        }
    }
}
//iterating pixel in window
void clear_color_buffer(uint32_t color) {
    for (int y = 0; y < window_height; y++) {
        for (int x = 0; x < window_width; x++) {
            color_buffer[(window_width * y) + x] = color;
        }
    }
}

void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderClear(renderer);

    draw_grid();
    draw_rect(0, 500, 800, 580, 0xff00ff00);

    render_color_buffer();
    clear_color_buffer(0xff000000);
    
    SDL_RenderPresent(renderer);
}

void destroy_window(void) {
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
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

    return 0;
}