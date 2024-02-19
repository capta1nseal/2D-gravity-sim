#ifndef _GRAVITYSIMAPPLICATION_
#define _GRAVITYSIMAPPLICATION_

#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <mutex>

#include <SDL2/SDL.h>

#include "camera.hpp"
#include "vec2.hpp"
#include "simulation.hpp"
#include "particles.hpp"

class GravitySimApplication
{
public:
    GravitySimApplication();
    ~GravitySimApplication();

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

    Camera camera;

    GravitySimSimulation simulation;

    std::thread simulationThread;
    
    std::mutex renderMutex;

    void initializeSdl();
    void destroySdl();

    void initializeInput();

    void initializeCamera();

    void initializeSimulation();

    std::chrono::steady_clock::time_point now();

    void handleEvents();

    void tick(double delta);

    void clearWindowTexture();

    void draw();
};

#endif
