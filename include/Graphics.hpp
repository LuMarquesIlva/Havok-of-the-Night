#pragma once

#include <SDL3/SDL_gpu.h>

#include "Core.hpp"

class Vertex
{
    private:
        SDL_GPUVertexInputState VertexInputState;
        SDL_GPUVertexBufferDescription VertexBuffer;
        SDL_GPUVertexAttribute VertexAttribute;
        //{1,1+1,SDL_GPU_VERTEXINPUTRATE_VERTEX,0}
    public:
        Vertex() {
            this->VertexBuffer.slot = 1;
            this->VertexBuffer.pitch = 1+1;
            this->VertexBuffer.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
            this->VertexBuffer.instance_step_rate = 0;

            this->VertexAttribute.location = 2;
            this->VertexAttribute.buffer_slot = 1;
            this->VertexAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_INT;
            this->VertexAttribute.offset = 1;

            this->VertexInputState.vertex_attributes = &this->VertexAttribute;
            this->VertexInputState.num_vertex_attributes = 1;
            this->VertexInputState.vertex_buffer_descriptions = &this->VertexBuffer;
            this->VertexInputState.num_vertex_buffers = 1;
        };
};

class GraphicsPipeline
{
    private:
        SDL_GPUGraphicsPipelineCreateInfo PipelineInfo;
        SDL_GPUGraphicsPipeline* HavokGraphicsPipeline;
        SDL_GPURasterizerState RasterState;
        SDL_GPUMultisampleState MultisampleState;
    public:
        GraphicsPipeline(
            SDL_GPUShader* V_Shader,
            SDL_GPUShader* F_Shader,
            SDL_GPUPrimitiveType PrimType,
            SDL_GPURasterizerState RasterState,
            SDL_GPUMultisampleState MultisampleState,
            SDL_GPUDepthStencilState DepthStencilState,
            SDL_GPUGraphicsPipelineTargetInfo TargetInfo) {

            this->PipelineInfo.vertex_shader = V_Shader;
            this->PipelineInfo.fragment_shader = F_Shader;
            this->PipelineInfo.primitive_type = PrimType;
            this->PipelineInfo.rasterizer_state = RasterState;
            this->PipelineInfo.multisample_state = MultisampleState;
            this->PipelineInfo.depth_stencil_state = DepthStencilState;
            this->PipelineInfo.target_info = TargetInfo;

            this->HavokGraphicsPipeline = SDL_CreateGPUGraphicsPipeline(Core::GetGPUDevice(), &this->PipelineInfo);
        }
}
