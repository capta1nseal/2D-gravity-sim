#ifndef _GRAVITYSIMAPPLICATION_
#define _GRAVITYSIMAPPLICATION_

#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

#include "camera.hpp"
#include "vec2.hpp"
#include "simulation.hpp"

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

    SDL_GLContext gContext;
    GLuint gProgramID;
    GLint gVertexPos2DLocation = -1;
    GLuint gVBO = 0;
    GLuint gIBO = 0;

    double framerate;

    SDL_Event event;
    const Uint8 *keyboardState;
    
    SDL_Point mousePosition;
    Vec2 worldMousePosition;

    Input input;

    Camera camera;

    GravitySimSimulation simulation;

    std::thread simulationThread;

    void initializeGraphics();
    void destroyGraphics();

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
