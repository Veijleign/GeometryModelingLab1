#include <iostream>
#include <vector>

#include "SDL.h"
#include "imgui.h"
#include "imgui_impl_sdlrenderer.h"
#include "imgui_impl_sdl.h"
#include "cmath"
#include <string>
//#include <D3d9types.h>
#include "color.h"

using namespace std;


int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL init falure\n";
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    
    SDL_Window* window = SDL_CreateWindow("Window",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cout << "Couldn't create window\n";
        return -1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cout << "Couldn't create renderer\n";
        return -1;
    }

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

    size_t textureWidth = 500;
    size_t textureHeight = 500;
    SDL_Texture* texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING | SDL_TEXTUREACCESS_TARGET,
        textureWidth,
        textureHeight);
    std::vector<Uint32> bufferForUpdate(textureWidth * textureHeight);
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!
    for (auto& pixel : bufferForUpdate) // закрашиваются пиксели
        pixel = 0xaa000000; // обновление цвета фона
    SDL_UpdateTexture(texture, nullptr, bufferForUpdate.data(), textureWidth * sizeof(bufferForUpdate[0])); // обновление текстуры

    bool programWorks = true;
    int fps = 60;
    Uint64 trueFrameTime = 1000.f / fps;
    while (programWorks)
    {
        Uint64 frameTime = SDL_GetTicks();
        // HandleInputs
        {
            SDL_Event e;
            while (SDL_PollEvent(&e))
            {
                ImGui_ImplSDL2_ProcessEvent(&e);
                if (e.type == SDL_QUIT)
                    programWorks = false;
            }
        }
        // UpdateImage
        {
            ImGui_ImplSDLRenderer_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            static float color_float[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
            // Render Gui
            {
                ImGui::Begin("Window");
               

                /*color_float[0] = (float)((*color >> ) & 0xFF) / 255.0f;
                color_float[1] = (float)((*color >> 8) & 0xFF) / 255.0f;
                color_float[2] = (float)((*color >> 16) & 0xFF) / 255.0f;*/


                
                if (ImGui::ColorEdit3("ColorEdit", (float*)&color_float, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel)) {

                    ImU32 col32_no_alpha = ImGui::ColorConvertFloat4ToU32(
                        ImVec4(color_float[3], color_float[2], color_float[1],color_float[0]));

                    float a = (col32_no_alpha >> 24) & 255;
                    float r = (col32_no_alpha >> 16) & 255;
                    float g = (col32_no_alpha >> 8) & 255;
                    float b = col32_no_alpha & 255;

                    for (auto& pixel : bufferForUpdate)
                        pixel = col32_no_alpha;
                    SDL_UpdateTexture(texture, nullptr, bufferForUpdate.data(), textureWidth * sizeof(bufferForUpdate[0]));

                }

                // тест кнопок и цветов
                if (ImGui::Button("3"))
                {
                    for (auto& pixel : bufferForUpdate)
                        pixel = 0x0000000;
                    SDL_UpdateTexture(texture, nullptr, bufferForUpdate.data(), textureWidth * sizeof(bufferForUpdate[0]));
                }

  
                if (ImGui::Button("1"))
                {
                    for (auto& pixel : bufferForUpdate)
                        pixel = 0x000000;
                    SDL_UpdateTexture(texture, nullptr, bufferForUpdate.data(), textureWidth * sizeof(bufferForUpdate[0]));
                }
                if (ImGui::Button("2"))
                {
                    for (auto& pixel : bufferForUpdate)
                        pixel = 0xffffff;
                    SDL_UpdateTexture(texture, nullptr, bufferForUpdate.data(), textureWidth * sizeof(bufferForUpdate[0]));
                }

                ImGui::End();
            }

            ImGui::Render();

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, nullptr, nullptr);

            ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
            SDL_RenderPresent(renderer);
        }
        
        frameTime = SDL_GetTicks() - frameTime;
        if (frameTime < trueFrameTime)
            SDL_Delay(trueFrameTime - frameTime);
    }

    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}

