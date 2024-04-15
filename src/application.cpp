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
    initializeCamera();
    initializeSimulation();
}
GravitySimApplication::~GravitySimApplication()
{
    destroyGraphics();
}


void GravitySimApplication::run()
{
    std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::duration<double, std::chrono::_V2::steady_clock::period>> start;
        
    std::chrono::duration<double> delta(0.0);

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

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

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

    { // initializing openGL
        gProgramID = glCreateProgram();

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

        glClearColor(0.f, 0.f, 0.f, 0.1f);

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
    camera.setPosition(Vec2(0.0, 0.0));
}

void GravitySimApplication::initializeSimulation()
{
    // The following 4 lines create a cyclic spinning pattern
    // simulation.addAttractor(Vec2(0.0, -500.0), Vec2(200.0, 0.0), 50.0);
    // simulation.addAttractor(Vec2(0.0, 500.0), Vec2(-200.0, 0.0), 50.0);
    // simulation.addAttractor(Vec2(500.0, 0.0), Vec2(0.0, 200.0), 50.0);
    // simulation.addAttractor(Vec2(-500.0, 0.0), Vec2(0.0, -200.0), 50.0);

    // The following lines model the solar system
    // constants
    double solarMass = 1.9891e30;
    double massBasis = 1e24;
    double astronomicalUnit = 149597870700.0;
    double distanceBasis = 1e9;

    // commonly re-used variables
    double distance;
    double velocity;
    double mass;

    // sun
    simulation.addAttractor(Vec2(0.0, 0.0), Vec2(0.0, 0.0), solarMass);
    // mercury
    distance = astronomicalUnit * 0.38;
    velocity = 47400;
    mass = massBasis * 0.330;
    simulation.addAttractor(Vec2(distance, 0.0), Vec2(0.0, -velocity), mass);
    // venus
    distance = astronomicalUnit * 0.72;
    velocity = 35000;
    mass = massBasis * 4.87;
    simulation.addAttractor(Vec2(distance, 0.0), Vec2(0.0, -velocity), mass);
    // earth
    distance = astronomicalUnit;
    velocity = 29800;
    mass = massBasis * 5.97;
    simulation.addAttractor(Vec2(distance, 0.0), Vec2(0.0, -velocity), mass);
    // moon
    distance += distanceBasis * 0.3844;
    velocity += 1022.0;
    mass = massBasis * 0.07346;
    simulation.addAttractor(Vec2(distance, 0.0), Vec2(0.0, -velocity), mass);
    // mars
    distance = astronomicalUnit * 1.52;
    velocity = 24100;
    mass = massBasis * 0.642;
    simulation.addAttractor(Vec2(distance, 0.0), Vec2(0.0, -velocity), mass);
    // jupiter
    distance = astronomicalUnit * 5.20;
    velocity = 13100;
    mass = massBasis * 1898;
    simulation.addAttractor(Vec2(distance, 0.0), Vec2(0.0, -velocity), mass);
    // saturn
    distance = astronomicalUnit * 9.58;
    velocity = 9700;
    mass = massBasis * 568;
    simulation.addAttractor(Vec2(distance, 0.0), Vec2(0.0, -velocity), mass);
    // uranus
    distance = astronomicalUnit * 19.14;
    velocity = 6800;
    mass = massBasis * 86.8;
    simulation.addAttractor(Vec2(distance, 0.0), Vec2(0.0, -velocity), mass);
    // neptune
    distance = astronomicalUnit * 30.20;
    velocity = 5400;
    mass = massBasis * 102;
    simulation.addAttractor(Vec2(distance, 0.0), Vec2(0.0, -velocity), mass);


    camera.startFollowing(simulation.getClosestPositionPointer(Vec2(0.0, 0.0)));
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
            case SDL_SCANCODE_F:
                if (camera.isFollowing())
                {
                    camera.stopFollowing();
                }
                else
                {
                    Vec2* positionToFollow = simulation.getClosestPositionPointer(worldMousePosition);
                    camera.startFollowing(positionToFollow);
                }
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                simulation.addAttractor(worldMousePosition, 100.0);
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
    glClear(GL_COLOR_BUFFER_BIT);


    unsigned int particleCount;
    std::vector<Vec2> positionArray, previousPositionArray;
    unsigned int attractorCount;
    std::vector<Attractor> attractorArray, previousAttractorArray;
    
    simulation.getFrameData(particleCount, positionArray, previousPositionArray, attractorCount, attractorArray, previousAttractorArray);

    Vec2 drawPosition;
    Vec2 previousDrawPosition;

    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);

    glBufferData(GL_ARRAY_BUFFER, (particleCount + attractorCount) * 2 * 2 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);

    GLfloat *vertexBufferData = reinterpret_cast<GLfloat*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

    for (unsigned int i = 0; i < particleCount; i++)
    {
        drawPosition.set(camera.mapCoordinate(&positionArray[i]));
        previousDrawPosition.set(camera.mapPreviousCoordinate(previousPositionArray[i]));

        vertexBufferData[i * 4] = static_cast<GLfloat>(drawPosition.x);
        vertexBufferData[i * 4 + 1] = static_cast<GLfloat>(drawPosition.y);
        vertexBufferData[i * 4 + 2] = static_cast<GLfloat>(previousDrawPosition.x);
        vertexBufferData[i * 4 + 3] = static_cast<GLfloat>(previousDrawPosition.y);
    }

    unsigned int attractorBaseIndex = particleCount * 4;

    for (unsigned int i = 0; i < attractorCount; i++)
    {
        drawPosition.set(camera.mapCoordinate(attractorArray[i].position));
        previousDrawPosition.set(camera.mapPreviousCoordinate(previousAttractorArray[i].position));

        vertexBufferData[attractorBaseIndex + i * 4] = static_cast<GLfloat>(drawPosition.x);
        vertexBufferData[attractorBaseIndex + i * 4 + 1] = static_cast<GLfloat>(drawPosition.y);
        vertexBufferData[attractorBaseIndex + i * 4 + 2] = static_cast<GLfloat>(previousDrawPosition.x);
        vertexBufferData[attractorBaseIndex + i * 4 + 3] = static_cast<GLfloat>(previousDrawPosition.y);
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);

    glGenBuffers(1, &gIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (particleCount + attractorCount) * 2 * sizeof(GLuint), nullptr, GL_STATIC_DRAW);

    GLuint *indexBufferData = reinterpret_cast<GLuint*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));

    for (unsigned int i = 0; i < (particleCount + attractorCount) * 2; i++)
    {
        indexBufferData[i] = i;
    }

    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    
    gVertexPos2DLocation = glGetAttribLocation(gProgramID, "LVertexPos2D");

    glUseProgram(gProgramID);

    glEnableVertexAttribArray(gVertexPos2DLocation);

    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);

    glLineWidth(1.5);
    glDrawArrays(GL_LINES, 0, particleCount * 2);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(1.5);
    glDrawArrays(GL_POINTS, 0, particleCount * 2);

    glLineWidth(15.0);
    glDrawArrays(GL_LINES, particleCount * 2, attractorCount * 2);
    glPointSize(15.0);
    glDrawArrays(GL_POINTS, particleCount * 2, attractorCount * 2);

    glDisableVertexAttribArray(gVertexPos2DLocation);

    glUseProgram(NULL);


    SDL_GL_SwapWindow(window);
}
