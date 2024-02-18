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

std::chrono::_V2::steady_clock::time_point now();

class SimulationApplication
{
public:
    SimulationApplication();

    void run();


private:
    bool running;
    int displayWidth, displayHeight;

    SDL_DisplayMode displayMode;
    SDL_Window *window;
    SDL_Texture *windowTexture;
    SDL_Renderer *renderer;

    double framerate;

    SDL_Event event;
    const Uint8 *keyboardState;
    
    SDL_Point mousePosition;
    Vec2 worldMousePosition;

    Input input;

    int physicsSubsteps = 50;

    Camera camera;

    AttractorArray attractorArray;

    Particles particles;

    void initializeSdl();

    void initializeInput();

    void initializeCamera();

    void initializeAttractorArray();

    void initializeParticles();

    void handleEvents();

    void tick(double delta);

    void clearWindowTexture();

    void draw();

    void deInitializeSdl();
};

#endif
