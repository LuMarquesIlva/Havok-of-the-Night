#pragma once

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

class Core {
public:

    /* This function runs once at startup. */
    SDL_AppResult Init() {

        SDL_SetAppMetadata("Havok of the Night", "1.0", "com.example.havok-of-the-night");
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

        if (!SDL_Init(SDL_INIT_VIDEO)) {
            SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }

        if (!SDL_CreateWindowAndRenderer("Havok of the Night", 640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL, &window, &renderer)) {
            SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }

        // Disables VSYNC to be compatible with older integrated GPUs
        if (SDL_SetRenderVSync(renderer, SDL_RENDERER_VSYNC_DISABLED) == false) {
        SDL_Log("Failed to disable VSync: %s", SDL_GetError());
        }

        SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);

        return SDL_APP_CONTINUE;  /* carry on with the program! */

    };

    SDL_Window *GetWindow() {
        return window;
    }

    SDL_Renderer *GetRenderer() {
        return renderer;
    }

};
