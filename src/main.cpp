#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
//#define _USE_VULKAN /* Uncomment to Use Vulkan */

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_main.h>

#include "../include/Core.hpp"
#include "../include/Entity.hpp"
#include "../include/Input.hpp"

#include "../include/VectorFuncs.hpp"

Vector2 vec2 = Vector2(60.0, 40.0);
Input input;
Input::Mouse _Mouse;
Input::Keyboard _Keyboard;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_AppResult APP_RES = Core::Init();

    #ifdef _USE_VULKAN
        const SDL_GPUShader* TriangleV = Core.LoadShader("TriangleV", "triangle.vert", SDL_GPU_SHADERSTAGE_VERTEX);
        const SDL_GPUShader* TriangleF = Core.LoadShader("TriangleF", "triangle.frag", SDL_GPU_SHADERSTAGE_FRAGMENT);

        SDL_GPURasterizerState RasterState = {
            SDL_GPU_FILLMODE_FILL,
            SDL_GPU_CULLMODE_NONE,
            SDL_GPU_FRONTFACE_CLOCKWISE
        };
        SDL_GPUMultisampleState MultisampleState = {
            SDL_GPU_SAMPLECOUNT_1,
            0,
            false,
            true
        };
        SDL_GPUStencilOpState StencilOpState = {
            SDL_GPU_STENCILOP_ZERO,
            SDL_GPU_STENCILOP_ZERO,
            SDL_GPU_STENCILOP_ZERO
        };
        SDL_GPUDepthStencilState DepthStencilState = {
            SDL_GPU_COMPAREOP_LESS,
            StencilOpState,
            StencilOpState,
            0
        };
        SDL_GPUColorTargetDescription ColorTarget = {
            SDL_GPU_TEXTUREFORMAT_BC1_RGBA_UNORM,
        };
        SDL_GPUGraphicsPipelineTargetInfo TargetInfo = {
            &ColorTarget,
            256,
            SDL_GPU_TEXTUREFORMAT_BC1_RGBA_UNORM,
            true,
        };
        SDL_GPUGraphicsPipeline GPUPipeline(&TriangleV, &TriangleF, SDL_GPU_PRIMITIVETYPE_POINTLIST, RasterState, MultisampleState, DepthStencilState, TargetInfo);
    #endif

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
        line.SetEndPosition(event->motion.x, event->motion.y);
    }

    _Keyboard.Update(event);

    // Keyboard Input Update
    if (event->type == SDL_EVENT_KEY_DOWN) {

        switch (event->key.key) {
        case SDL_Keycode(SDLK_W):
            line.Move(0.0f, -1.0f, 0.0f, 0.0f);
            break;
        case SDL_Keycode(SDLK_A):
            line.Move(-1.0f, 0.0f, 0.0f, 0.0f);
            break;
        case SDL_Keycode(SDLK_S):
            line.Move(0.0f, 1.0f, 0.0f, 0.0f);
            break;
        case SDL_Keycode(SDLK_D):
            line.Move(1.0f, 0.0f, 0.0f, 0.0f);
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

        SDL_SetRenderDrawColorFloat(Core::GetRenderer(), 0.3, 0.3, 0.3, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */

        /* clear the window to the draw color. */
        SDL_RenderClear(Core::GetRenderer());

        obj.SetColor(0.8f, 0.5f, 0.6f);
        line.SetColor(0.4f, 0.5f, 0.2f);

        obj.Draw();
        line.Draw();

        /* put the newly-cleared rendering on the screen. */
        SDL_RenderPresent(Core::GetRenderer());
    #endif

    #ifdef _USE_VULKAN

    //TODO: Create Graphics Pipeline (Understand first actually)



    #endif

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    Core::Quit();
}
