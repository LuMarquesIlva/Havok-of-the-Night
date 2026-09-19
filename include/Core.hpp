#pragma once

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#define _USE_VULKAN 1
#define VK_NO_PROTOTYPES 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_log.h>

#ifdef _USE_VULKAN
    #include <SDL3/SDL_vulkan.h>
    #include <vulkan/vulkan.h>
    #include <vulkan/vulkan_core.h>
#endif

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

class Core {
    private:

        const char *AppName = "Havok of the Night";

        int win_H = 480;
        int win_W = 640;

        #ifdef _USE_VULKAN
            VkInstance VK_instance;
            VkSurfaceKHR VK_surface;

        VkApplicationInfo appInfo = {};
        VkInstanceCreateInfo createInfo = {};
        #endif
    public:

    /* This function runs once at startup. */
    SDL_AppResult Init() {

        SDL_SetAppMetadata(AppName, "0.1", "com.example.havok-of-the-night");

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

            window = SDL_CreateWindow(AppName, win_W, win_H, SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);
            if (!window) {
                SDL_Log("Couldn't create Vulkan window: %s", SDL_GetError());
                return SDL_APP_FAILURE;
            }

            Uint32 extensionCount;
            const char* const* extensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);

            // Global entry point to get other funcitons
            PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr =
                (PFN_vkGetInstanceProcAddr)SDL_Vulkan_GetVkGetInstanceProcAddr();

            PFN_vkCreateInstance vkCreateInstance =
                (PFN_vkCreateInstance)vkGetInstanceProcAddr(NULL, "vkCreateInstance");

            vkGetInstanceProcAddr(VK_instance, *extensions);

             // ... initialization code for instance ...

            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = AppName;
            appInfo.apiVersion = VK_API_VERSION_1_3;

            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;
            createInfo.enabledExtensionCount = extensionCount;
            createInfo.ppEnabledExtensionNames = extensions;

            if (vkCreateInstance(&createInfo, nullptr, &VK_instance) != VK_SUCCESS) {
                SDL_Log("Couldn't create Vulkan instance: %s", SDL_GetError());
                return SDL_APP_FAILURE;
            }

            if (!SDL_Vulkan_CreateSurface(window, VK_instance, nullptr, &VK_surface)) {
                //SDL_Log("%s", VK_instance);
                SDL_Log("Couldn't create vulkan renderer: %s", SDL_GetError());
                return SDL_APP_FAILURE;
            };

        } else if (_USE_VULKAN == false) {
            if (!SDL_CreateWindowAndRenderer(AppName, win_W, win_H, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
                SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
                return SDL_APP_FAILURE;
            }
        }

        // Disables VSYNC to be compatible with some integrated GPUs
        if (SDL_SetRenderVSync(renderer, SDL_RENDERER_VSYNC_DISABLED) == false && _USE_VULKAN == false) {
            SDL_Log("Failed to disable VSync: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }

        SDL_SetRenderLogicalPresentation(renderer, win_W, win_H, SDL_LOGICAL_PRESENTATION_LETTERBOX);

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
            SDL_DestroyWindow(window);
            SDL_Vulkan_UnloadLibrary();
            SDL_Quit();
        } else if (_USE_VULKAN == false) {
            ;
        }
    }

};
