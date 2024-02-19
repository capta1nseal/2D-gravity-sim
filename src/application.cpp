#include "application.hpp"

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

GravitySimApplication::GravitySimApplication()
{
    initializeGraphics();
    initializeInput();
    initializeSimulation();
    initializeCamera();
}
GravitySimApplication::~GravitySimApplication()
{
    destroyGraphics();
}


void GravitySimApplication::run()
{
    std::cout << "test reached run\n";
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

void GravitySimApplication::initializeGraphics()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    keyboardState = SDL_GetKeyboardState(NULL);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GetCurrentDisplayMode(0, &displayMode);

    displayWidth = displayMode.w;
    displayHeight = displayMode.h;

    Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP;
    window = SDL_CreateWindow(
        "gravitysim",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        displayWidth, displayHeight,
        windowFlags);
    
    gContext = SDL_GL_CreateContext(window);

    glewInit();

    SDL_GL_SetSwapInterval(1);

    std::cout << "test2\n";

    gProgramID = glCreateProgram();

    std::cout << "test3\n";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    const GLchar *vertexShaderSource[] =
    {
        "#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
    };

    glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

    glCompileShader(vertexShader);

    glAttachShader(gProgramID, vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar *fragmentShaderSource[] =
    {
        "#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
    };

    glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

    glCompileShader(fragmentShader);

    glAttachShader(gProgramID, fragmentShader);

    glLinkProgram(gProgramID);

    gVertexPos2DLocation = glGetAttribLocation(gProgramID, "LVertexPos2D");

    glClearColor(0.f, 0.f, 0.f, 1.f);

    GLfloat vertexData[] =
    {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    GLuint indexData[] = {0, 1, 2, 3};

    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

    glGenBuffers(1, &gIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
}
void GravitySimApplication::destroyGraphics()
{
    glDeleteProgram(gProgramID);

    SDL_DestroyWindow(window);
    window = NULL;

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
}

void GravitySimApplication::draw()
{
    /*
    unsigned int particleCount;
    std::vector<Vec2> positionArray, previousPositionArray;
    unsigned int attractorCount;
    std::vector<Attractor> attractorArray, previousAttractorArray;

    simulation.getFrameData(particleCount, positionArray, previousPositionArray, attractorCount, attractorArray, previousAttractorArray);
    simulation.storePreviousPositions();

    Vec2 drawPosition;
    Vec2 previousDrawPosition;
    
    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (unsigned int i = 0; i < particleCount; i++)
    {
        drawPosition.set(camera.mapCoordinate(&positionArray[i]));
        previousDrawPosition.set(camera.mapPreviousCoordinate(&previousPositionArray[i]));
        if (((drawPosition.x < 0 or drawPosition.x > displayWidth - 1) or (drawPosition.y < 0 or drawPosition.y > displayHeight - 1)) and ((previousDrawPosition.x < 0 or previousDrawPosition.x > displayWidth - 1) or (previousDrawPosition.y < 0 or previousDrawPosition.y > displayHeight - 1)) or (subtractVec2(drawPosition, previousDrawPosition).magnitude() > 10000.0))
            continue;
        
        //SDL_RenderDrawLineF(renderer, drawPosition.x, drawPosition.y, previousDrawPosition.x, previousDrawPosition.y);
    }

    //SDL_SetRenderDrawColor(renderer, 191, 127, 15, 255);
    for (unsigned int i = 0; i < attractorCount; i++)
    {
        drawPosition.set(camera.mapCoordinate(&attractorArray[i].position));
        previousDrawPosition.set(camera.mapPreviousCoordinate(&previousAttractorArray[i].position));
        if (((drawPosition.x < 0 or drawPosition.x > displayWidth - 1) or (drawPosition.y < 0 or drawPosition.y > displayHeight - 1)) and ((previousDrawPosition.x < 0 or previousDrawPosition.x > displayWidth - 1) or (previousDrawPosition.y < 0 or previousDrawPosition.y > displayHeight - 1)) or (subtractVec2(drawPosition, previousDrawPosition).magnitude() > 1000.0))
            continue;
        
        //SDL_RenderDrawLineF(renderer, drawPosition.x, drawPosition.y, previousDrawPosition.x, previousDrawPosition.y);
    }
    */
    {
        glUseProgram(gProgramID);

        glEnableVertexAttribArray(gVertexPos2DLocation);

        glBindBuffer(GL_ARRAY_BUFFER, gVBO);
        glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
        glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

        glDisableVertexAttribArray(gVertexPos2DLocation);

        glUseProgram(NULL);
    }

    SDL_GL_SwapWindow(window);
}
