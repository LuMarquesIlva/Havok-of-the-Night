#pragma once

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#define _USE_VULKAN 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_vulkan.h>

#ifdef _USE_VULKAN
    #include <vulkan/vulkan.h>
    #include <vulkan/vulkan_core.h>
#endif

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

class Core {
    private:

        int win_H = 480;
        int win_W = 640;

        VkInstance VK_instance;
        VkSurfaceKHR VK_surface;

        SDL_Window* VK_window;

        VkApplicationInfo appInfo = {};
        VkInstanceCreateInfo createInfo = {};
    public:

    /* This function runs once at startup. */
    SDL_AppResult Init() {

        SDL_SetAppMetadata("Havok of the Night", "0.1", "com.example.havok-of-the-night");

        if (_USE_VULKAN == 2) {
            SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
        }

        if (!SDL_Init(SDL_INIT_VIDEO)) {
            SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }

        if (_USE_VULKAN == true) {
            SDL_Log("Using VULKAN");
            // Load the Vulkan loader library
            SDL_Vulkan_LoadLibrary(nullptr);

            VK_window = SDL_CreateWindow("Havok of the Night", win_W, win_H, SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);
            if (!VK_window) {
                SDL_Log("Couldn't create Vulkan window: %s", SDL_GetError());
                return SDL_APP_FAILURE;
            }

            Uint32 extensionCount;
            const char* const* extensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);

            // staticly converts the sdl function to the vulkan one
            PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr =
                (PFN_vkGetInstanceProcAddr)SDL_Vulkan_GetVkGetInstanceProcAddr();

            // Use vkGetInstanceProcAddr to create VkInstance with extensions...
             vkGetInstanceProcAddr(VK_instance, *extensions);

             // ... initialization code for instance ...

            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = "Havok of the Night";
            appInfo.apiVersion = VK_API_VERSION_1_3;

            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;
            createInfo.enabledExtensionCount = extensionCount;
            createInfo.ppEnabledExtensionNames = extensions;

            if (vkCreateInstance(&createInfo, nullptr, &VK_instance) != VK_SUCCESS) {
                SDL_Log("Couldn't create Vulkan Instance: %s", SDL_GetError());
                return SDL_APP_FAILURE;
            }

            if (!SDL_Vulkan_CreateSurface(VK_window, VK_instance, NULL, &VK_surface)) {
                //SDL_Log("%s", VK_instance);
                SDL_Log("Couldn't create vulkan renderer: %s", SDL_GetError());
                return SDL_APP_FAILURE;
            };

        } else if (_USE_VULKAN == false) {
            if (!SDL_CreateWindowAndRenderer("Havok of the Night", win_W, win_H, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
                SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
                return SDL_APP_FAILURE;
            }
        }

        // Disables VSYNC to be compatible with some integrated GPUs
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

    /* This function runs once at shutdown. */
    void Quit()
    {
        if (_USE_VULKAN == true) {
            SDL_Vulkan_DestroySurface(VK_instance, VK_surface, NULL);
            SDL_DestroyWindow(VK_window);
            SDL_Vulkan_UnloadLibrary();
            SDL_Quit();
        } else if (_USE_VULKAN == false) {
            ;
        }
    }

};
