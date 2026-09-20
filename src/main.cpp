#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_main.h>

#include "include/Core.hpp"
#include "include/Entity.hpp"
#include "include/Input.hpp"

Vector2 vec2 = Vector2(60.0, 40.0);
Input input;
Input::Mouse _Mouse;
Input::Keyboard _Keyboard;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_AppResult APP_RES = core.Init();
    const SDL_GPUShader* TriangeV = core.LoadShader("triangle", SDL_GPU_SHADERSTAGE_VERTEX);
    const SDL_GPUShader* TriangeF = core.LoadShader("triangle", SDL_GPU_SHADERSTAGE_FRAGMENT);
    return APP_RES;

}

Rect obj(1, 40, 60, 30, 30);
Line line(2, 40, 60, 300, 300);

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    _Mouse.UpdateMouse(event);

    if (_Mouse.IsMouseButtonDown(0) == true) {
        //SDL_Log("Mouse Button 0 Down");
        line.SetEndPosition(event->button.x, event->button.y);
    }

    _Keyboard.Update(event);

    // Keyboard Input Update
    if (event->type == SDL_EVENT_KEY_DOWN) {

        switch (event->key.key) {
        case SDL_Keycode(SDLK_W):
            obj.Move(0.0f, -1.0f);
            break;
        case SDL_Keycode(SDLK_A):
            obj.Move(-1.0f, 0.0f);
            break;
        case SDL_Keycode(SDLK_S):
            obj.Move(0.0f, 1.0f);
            //SDL_Log("X: %f | Y: %f", obj.GetPosition()->x, obj.GetPosition()->y);
            break;
        case SDL_Keycode(SDLK_D):
            obj.Move(1.0f, 0.0f);
            //SDL_Log("X: %f | Y: %f", obj.GetPosition()->x, obj.GetPosition()->y);
            break;
        case SDLK_Q:
            SDL_Quit();
            return SDL_APP_SUCCESS;
            break;
        default:
            SDL_Log("Invalid Input");
            break;
        }
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    #ifndef _USE_VULKAN
    //const double now = ((double)SDL_GetTicks()) / 1000.0;  /* convert from milliseconds to seconds. */

    SDL_SetRenderDrawColorFloat(core.GetRenderer(), 0.3, 0.3, 0.3, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */

    /* clear the window to the draw color. */
    SDL_RenderClear(core.GetRenderer());

    SDL_SetRenderDrawColorFloat(core.GetRenderer(), 0.2, 0.5, 0.8, SDL_ALPHA_OPAQUE_FLOAT);

    obj.Draw();

    SDL_SetRenderDrawColorFloat(core.GetRenderer(), 0.6, 0.8, 0.8, SDL_ALPHA_OPAQUE_FLOAT);

    line.Draw();

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(core.GetRenderer());
    #endif

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    core.Quit();
}
