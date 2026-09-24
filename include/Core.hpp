#pragma once

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
//#define _USE_VULKAN
#define VK_NO_PROTOTYPES 1

#include <string>

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_video.h>

#ifndef _USE_VULKAN
    #include <SDL3/SDL_render.h>
#endif

#include "Utility.hpp"

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

static const char *AppName = "Havok of the Night";

static int win_H = 480;
static int win_W = 640;

#ifdef _USE_VULKAN
    static SDL_GPUDevice* GPUDevice = nullptr;

    struct Havok_Shader {
        const char* Name;
        const SDL_GPUShader* ShaderData;
    };
#endif

class Core {
    private:

        #ifdef _USE_VULKAN
            VkInstance VK_instance;
            VkSurfaceKHR VK_surface;

            VkApplicationInfo appInfo = {};
            VkInstanceCreateInfo createInfo = {};
            SDL_GPUCommandBuffer* CM_Buffer;

            std::vector<Havok_Shader> LoadedShaderList;
        #endif
    public:

    /* This function runs once at startup. */
    static SDL_AppResult Init() {

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

            CM_Buffer = SDL_AcquireGPUCommandBuffer(GPUDevice);
            if (!CM_Buffer) {
                SDL_Log("Could Not Aquire Command Buffer: %s", SDL_GetError());
                return SDL_APP_FAILURE;
            }
        #endif
        #ifndef _USE_VULKAN
            if (!SDL_CreateWindowAndRenderer(AppName, win_W, win_H, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
                SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
                return SDL_APP_FAILURE;
            }

            // Disables VSYNC to be compatible with some integrated GPUs
            if (SDL_SetRenderVSync(renderer, SDL_RENDERER_VSYNC_DISABLED) == false) {
                SDL_Log("Failed to disable VSync: %s", SDL_GetError());
                return SDL_APP_FAILURE;
            }

            SDL_SetRenderLogicalPresentation(renderer, win_W, win_H, SDL_LOGICAL_PRESENTATION_LETTERBOX);
        #endif
        return SDL_APP_CONTINUE;  /* carry on with the program! */

    };

    static SDL_Window *GetWindow() {
        return window;
    }

    static const char *GetAppName() {
        return AppName;
    }

    static int GetWindowWidth() {
        return win_W;
    }

    static int GetWindowHeight() {
        return win_H;
    }

    #ifndef _USE_VULKAN
        static SDL_Renderer *GetRenderer() {
            return renderer;
        }
    #endif

    #ifdef _USE_VULKAN
        // Filename is the name
        const SDL_GPUShader *LoadShader(const char* ShaderName, std::string Filename, SDL_GPUShaderStage ShaderStage) {
            std::string ShaderFolderTmp = ShaderFolder;
            std::string ShaderFile = ShaderFolderTmp.append("SPIR-V/" + Filename + ".spv");

            SDL_Log("%s", ShaderFile.c_str());

            size_t ShaderFileSize = 0;
            const Uint8* BinaryShaderFile = load_spirv_file(ShaderFile.c_str(), &ShaderFileSize);

            const SDL_GPUShader* ShaderInstance;

            if (ShaderStage == SDL_GPU_SHADERSTAGE_VERTEX) {
                SDL_GPUShaderCreateInfo ShaderTmp = {
                    .code_size = ShaderFileSize,
                    .code = BinaryShaderFile,
                    .entrypoint = "main",
                    .format = SDL_GPU_SHADERFORMAT_SPIRV,
                    .stage = ShaderStage,
                    .num_samplers = 0,
                    .num_storage_textures = 0,
                    .num_storage_buffers = 0,
                    .num_uniform_buffers = 0,
                    .props = 0,
                };
                ShaderInstance = SDL_CreateGPUShader(GPUDevice, &ShaderTmp);
            } else if (ShaderStage == SDL_GPU_SHADERSTAGE_FRAGMENT) {
                SDL_GPUShaderCreateInfo ShaderTmp = {
                    .code_size = ShaderFileSize,
                    .code = BinaryShaderFile,
                    .entrypoint = "main",
                    .format = SDL_GPU_SHADERFORMAT_SPIRV,
                    .stage = ShaderStage,
                    .num_samplers = 2,
                    .num_storage_textures = 2,
                    .num_storage_buffers = 2,
                    .num_uniform_buffers = 3,
                    .props = 0,
                };
                ShaderInstance = SDL_CreateGPUShader(GPUDevice, &ShaderTmp);
            } else {
                SDL_Log("Could Not Find ShaderStage Property");
            }

            if (!ShaderInstance) {
                SDL_Log("Could not create shader instance for %s: %s", ShaderFile.c_str(), SDL_GetError());
            }

            Havok_Shader EndValue = {ShaderName, ShaderInstance};

            LoadedShaderList.push_back(EndValue);

            return ShaderInstance;
        }

        SDL_GPUShader* GetShaderFromLoaded(const char* ShaderName)
        {
            for (size_t i = 0; i < LoadedShaderList.size(); i++) {
                Havok_Shader Shader = LoadedShaderList.at(i);
                if (Shader.Name == ShaderName) {
                    return (SDL_GPUShader*)Shader.ShaderData;
                } else {
                    return nullptr;
                }
            }
            return nullptr;
        }

        static SDL_GPUDevice* GetGPUDevice() {
            return GPUDevice;
        }
    #endif

    /* This function runs once at shutdown. */
    static void Quit()
    {
        #ifdef _USE_VULKAN
            SDL_Vulkan_DestroySurface(VK_instance, VK_surface, NULL);
            SDL_DestroyWindow(window);
            SDL_Vulkan_UnloadLibrary();
            SDL_Quit();
        #endif
    }

};
