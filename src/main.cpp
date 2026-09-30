#include <cstdint>
#include <raycast.h>
#include <math.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>

constexpr unsigned int mapWidth = 8;
constexpr unsigned int mapHeight = 8;
constexpr unsigned int pixelsPerCell = 48;
constexpr int offsetX = 24;
constexpr int offsetY = 24;

// World units -> raylib screen pixels; the only place engine and raylib vectors meet.
morph::math::Vec2 toScreen(morph::math::Vec2 world)
{
    return morph::math::Vec2 {
        offsetX + world.x * pixelsPerCell,
        offsetY + world.y * pixelsPerCell
    };
}


int main(int argc, char* argv[])
{

    (void)argc;
    (void)argv;

    constexpr int screenWidth = 320;
    constexpr int screenHeight = 200;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (!SDL_CreateWindowAndRenderer("Morph", screenWidth, screenHeight, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_Log("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
        SDL_Quit();
        return 1; 
    }   

    SDL_SetRenderVSync(renderer, 1);

    std::vector<std::uint32_t> pixels(screenWidth * screenHeight);
    SDL_Texture* frame = SDL_CreateTexture(renderer, 
        SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING,
        screenWidth, screenHeight);
    SDL_SetTextureScaleMode(frame, SDL_SCALEMODE_NEAREST);
    SDL_SetRenderLogicalPresentation(renderer, screenWidth, screenHeight,
                                 SDL_LOGICAL_PRESENTATION_LETTERBOX);

    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_EVENT_KEY_DOWN && event.type == SDLK_ESCAPE)
            {
                running = false;
            }
            else if (event.type == SDL_EVENT_KEY_DOWN)
            {
                SDL_Log("key=%s scancode=%s repeat=%d",
                    SDL_GetKeyName(event.key.key),
                    SDL_GetScancodeName(event.key.scancode),
                    event.key.repeat);SDL_GetKeyName(event.key.key);
            }
        }

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
