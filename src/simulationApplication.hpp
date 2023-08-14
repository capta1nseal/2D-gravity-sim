#ifndef _GRAVITYSIMULATIONAPPLICATION_
#define _GRAVITYSIMULATIONAPPLICATION_

#include <iostream>
#include <cmath>
#include <chrono>

#include <SDL2/SDL.h>

#include "camera.hpp"
#include "vec2.hpp"
#include "attractorArray.hpp"
#include "particles.hpp"

std::chrono::_V2::steady_clock::time_point now()
{
    return std::chrono::steady_clock::now();
}

class SimulationApplication
{
public:
    SimulationApplication()
    {
        initializeSdl();
        initializeInput();
        initializeAttractorArray();
        initializeParticles();
        initializeCamera();
    }

    void run()
    {
        std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::duration<double, std::chrono::_V2::steady_clock::period>> start;
        
        std::chrono::duration<double> delta;

        int counter = 0;
        
        running = true;
        draw();
        while (running)
        {
            start = now();

            handleEvents();

            tick(delta.count());

            if (counter % 60 == 0)
                std::cout << delta.count() << "\n";

            draw();

            delta = now() - start;

            counter += 1;
        }

        deInitializeSdl();
    }

private:
    bool running;
    int displayWidth, displayHeight;

    SDL_DisplayMode displayMode;
    SDL_Window *window;
    SDL_Renderer *renderer;

    double framerate;

    SDL_Event event;
    const Uint8 *keyboardState;
    
    SDL_Point mousePosition;

    Input input;

    int physicsSubsteps = 50;

    Camera camera;

    AttractorArray attractorArray;

    Particles particles;

    void initializeSdl()
    {
        SDL_Init(SDL_INIT_EVERYTHING);

        SDL_GetCurrentDisplayMode(0, &displayMode);

        displayWidth = displayMode.w;
        displayHeight = displayMode.h;

        Uint32 windowFlags = SDL_WINDOW_FULLSCREEN_DESKTOP;
        window = SDL_CreateWindow(
            "gravitysim",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            displayWidth, displayHeight,
            windowFlags);
        
        Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
        renderer = SDL_CreateRenderer(window, -1, renderFlags);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        keyboardState = SDL_GetKeyboardState(NULL);
    }

    void initializeInput()
    {
        input.setKeyboardState(keyboardState);
    }

    void initializeCamera()
    {
        camera.setInput(&input);

        camera.initializeResolution(displayWidth, displayHeight);
        camera.setPosition(Vec2(0, 0));
    }

    void initializeAttractorArray()
    {
        attractorArray.setMousePosition(&mousePosition);
<<<<<<< Updated upstream
=======

        attractorArray.addAttractor(Vec2(-500, 0));
        attractorArray.addAttractor(Vec2( 500, 0));
>>>>>>> Stashed changes
    }

    void initializeParticles()
    {
        particles.setAttractorArray(&attractorArray);
        particles.generateParticles(1, Vec2(-50, -50), Vec2(50, 50), Vec2(0, 0));
    }

    void handleEvents()
    {
        SDL_PumpEvents();

        SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    running = false;
                    break;
                default:
                    break;
                }
            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    attractorArray.leftClick(&camera);
                }
            default:
                break;
            }
        }

        input.updateInputs();
    }

    void tick(double delta)
    {
        particles.fetchTickData();

        for (int i = 0; i < physicsSubsteps; i++)
        {
            particles.tick(delta / static_cast<double>(physicsSubsteps));
        }

        camera.tick(delta);
    }

    void draw()
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        particles.draw(renderer, &camera);

        SDL_RenderPresent(renderer);
    }

    void deInitializeSdl()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

#endif
