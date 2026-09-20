#pragma once

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#define _USE_VULKAN 1
#define VK_NO_PROTOTYPES 1

#include <fstream>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_video.h>

#include "include/Utility.hpp"

#ifdef _USE_VULKAN
    #include <SDL3/SDL_vulkan.h>
    #include <vulkan/vulkan.h>
    #include <vulkan/vulkan_core.h>
    #include <SDL3/SDL_gpu.h>
#endif

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;

#ifndef _USE_VULKAN
static SDL_Renderer *renderer = NULL;
#endif

static std::string ShaderFolder = "assets/shaders/";

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
            SDL_GPUDevice* GPUDevice = nullptr;
        #endif
    public:

    /* This function runs once at startup. */
    SDL_AppResult Init() {

        SDL_SetAppMetadata(AppName, "0.1", "com.example.havok-of-the-night");

        //SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

        if (!SDL_Init(SDL_INIT_VIDEO)) {
            SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }

        #ifdef _USE_VULKAN
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

            GPUDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, "vulkan");
            if (GPUDevice == nullptr) {
                SDL_Log("Couldn't Create GPU Device: %s", SDL_GetError());
                return SDL_APP_FAILURE;
            }

            if (!SDL_ClaimWindowForGPUDevice(GPUDevice, window)) {
                SDL_Log("Couldn't claim window for GPUDevice: %s", SDL_GetError());
                return SDL_APP_FAILURE;
            }
        #endif
        #ifndef _USE_VULKAN
            if (!SDL_CreateWindowAndRenderer(AppName, win_W, win_H, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
                SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
                return SDL_APP_FAILURE;
            }
        #endif

        #ifndef _USE_VULKAN
        // Disables VSYNC to be compatible with some integrated GPUs
        if (SDL_SetRenderVSync(renderer, SDL_RENDERER_VSYNC_DISABLED) == false) {
            SDL_Log("Failed to disable VSync: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }
        #endif

        #ifndef _USE_VULKAN
        SDL_SetRenderLogicalPresentation(renderer, win_W, win_H, SDL_LOGICAL_PRESENTATION_LETTERBOX);
        #endif
        return SDL_APP_CONTINUE;  /* carry on with the program! */

    };

    SDL_Window *GetWindow() {
        return window;
    }

    #ifndef _USE_VULKAN
    SDL_Renderer *GetRenderer() {
        return renderer;
    }
    #endif

    // Filename is the name
    const SDL_GPUShader *LoadShader(std::string Filename, SDL_GPUShaderStage ShaderStage) {
        std::string ShaderFile = ShaderFolder.append(Filename);

        const Uint8* BinaryShaderFile = reinterpret_cast<const uint8_t*>(LoadFile_B(ShaderFile).data());

        SDL_GPUShaderCreateInfo* Shader;

            Shader->code_size = sizeof(BinaryShaderFile);
            Shader->code = BinaryShaderFile;
            Shader->entrypoint = "main";
            Shader->format = SDL_GPU_SHADERFORMAT_SPIRV;
            Shader->stage = ShaderStage;
            Shader->num_samplers = 0;
            Shader->num_storage_textures = 0;
            Shader->num_storage_buffers = 0;
            Shader->num_uniform_buffers = 0;
            Shader->props = 0;

        const SDL_GPUShader* ShaderInstance = SDL_CreateGPUShader(GPUDevice, Shader);

        if (!ShaderInstance) {
            SDL_Log("Could not create shader instance: %s", SDL_GetError());
        }

        return ShaderInstance;
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
