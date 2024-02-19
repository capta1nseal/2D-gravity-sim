#include "application.hpp"

#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>

#include <SDL2/SDL.h>

#include "camera.hpp"
#include "vec2.hpp"
#include "simulation.hpp"

GravitySimApplication::GravitySimApplication()
{
    initializeSdl();
    initializeInput();
    initializeSimulation();
    initializeCamera();
}
GravitySimApplication::~GravitySimApplication()
{
    destroySdl();
}


void GravitySimApplication::run()
{
    std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::duration<double, std::chrono::_V2::steady_clock::period>> start;
        
    std::chrono::duration<double> delta(0.0166667);

    int frameCounter = 0;
    
    simulationThread = std::thread(&GravitySimSimulation::simulationLoop, &simulation);

    running = true;
    clearWindowTexture();
    draw();
    while (running)
    {
        start = now();

        handleEvents();

        tick(delta.count());

        if (frameCounter % 60 == 0)
            std::cout << delta.count() << "\n";

        draw();

        delta = now() - start;

        frameCounter += 1;
    }

    simulation.stop();
    simulationThread.join();
}

void GravitySimApplication::initializeSdl()
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
    
    windowTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, displayWidth, displayHeight);

    Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE;
    renderer = SDL_CreateRenderer(window, -1, renderFlags);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    keyboardState = SDL_GetKeyboardState(NULL);
}
void GravitySimApplication::destroySdl()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(windowTexture);
    SDL_Quit();
}

void GravitySimApplication::initializeInput()
{
    input.setKeyboardState(keyboardState);
}

void GravitySimApplication::initializeCamera()
{
    camera.setInput(&input);

    camera.initializeResolution(displayWidth, displayHeight);
    camera.setPosition(Vec2(0, 0));
}

void GravitySimApplication::initializeSimulation()
{
    // The following 4 lines create a cyclic spinning pattern
    simulation.addAttractor(Vec2(0.0, -500.0), Vec2(200.0, 0.0), 50.0);
    simulation.addAttractor(Vec2(0.0, 500.0), Vec2(-200.0, 0.0), 50.0);
    simulation.addAttractor(Vec2(500.0, 0.0), Vec2(0.0, 200.0), 50.0);
    simulation.addAttractor(Vec2(-500.0, 0.0), Vec2(0.0, -200.0), 50.0);
}

std::chrono::steady_clock::time_point GravitySimApplication::now()
{
    return std::chrono::steady_clock::now();
}

void GravitySimApplication::handleEvents()
{
    SDL_PumpEvents();

    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

    worldMousePosition = camera.unMapCoordinate(Vec2(mousePosition.x, mousePosition.y));

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
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                simulation.addAttractor(worldMousePosition, 50.0);
                break;
            case SDL_BUTTON_RIGHT:
                simulation.removeAttractor(worldMousePosition);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }

    input.updateInputs();
}

void GravitySimApplication::tick(double delta)
{
    camera.tick(delta);
}

void GravitySimApplication::clearWindowTexture()
{
    SDL_SetRenderTarget(renderer, windowTexture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);
}

void GravitySimApplication::draw()
{
    SDL_SetRenderTarget(renderer, windowTexture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 63);
    SDL_RenderFillRect(renderer, NULL);

    unsigned int particleCount;
    std::vector<Vec2> positionArray, previousPositionArray;
    unsigned int attractorCount;
    std::vector<Attractor> attractorArray, previousAttractorArray;

    simulation.getFrameData(particleCount, positionArray, previousPositionArray, attractorCount, attractorArray, previousAttractorArray);
    simulation.storePreviousPositions();

    Vec2 drawPosition;
    Vec2 previousDrawPosition;
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (unsigned int i = 0; i < particleCount; i++)
    {
        drawPosition.set(camera.mapCoordinate(&positionArray[i]));
        previousDrawPosition.set(camera.mapPreviousCoordinate(&previousPositionArray[i]));
        if (((drawPosition.x < 0 or drawPosition.x > displayWidth - 1) or (drawPosition.y < 0 or drawPosition.y > displayHeight - 1)) and ((previousDrawPosition.x < 0 or previousDrawPosition.x > displayWidth - 1) or (previousDrawPosition.y < 0 or previousDrawPosition.y > displayHeight - 1)) or (subtractVec2(drawPosition, previousDrawPosition).magnitude() > 10000.0))
            continue;
        
        SDL_RenderDrawLineF(renderer, drawPosition.x, drawPosition.y, previousDrawPosition.x, previousDrawPosition.y);
    }

    SDL_SetRenderDrawColor(renderer, 191, 127, 15, 255);
    for (unsigned int i = 0; i < attractorCount; i++)
    {
        drawPosition.set(camera.mapCoordinate(&attractorArray[i].position));
        previousDrawPosition.set(camera.mapPreviousCoordinate(&previousAttractorArray[i].position));
        if (((drawPosition.x < 0 or drawPosition.x > displayWidth - 1) or (drawPosition.y < 0 or drawPosition.y > displayHeight - 1)) and ((previousDrawPosition.x < 0 or previousDrawPosition.x > displayWidth - 1) or (previousDrawPosition.y < 0 or previousDrawPosition.y > displayHeight - 1)) or (subtractVec2(drawPosition, previousDrawPosition).magnitude() > 1000.0))
            continue;
        
        SDL_RenderDrawLineF(renderer, drawPosition.x, drawPosition.y, previousDrawPosition.x, previousDrawPosition.y);
    }

    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, windowTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
