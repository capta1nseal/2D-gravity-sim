#include "input.hpp"

#include <array>

#include <SDL2/SDL_scancode.h>


Input::Input()
{
    for (int i = 0; i < inputArraySize; i++)
        inputArray[i] = false;
}

void Input::setKeyboardState(const Uint8 *newKeyboardState)
{
    keyboardState = newKeyboardState;
}

void Input::updateInputs()
{
    for (int i = 0; i < inputArraySize; i++)
        inputArray[i] = false;
    if (keyboardState[SDL_SCANCODE_UP])
        inputArray[0] = true;
    if (keyboardState[SDL_SCANCODE_RIGHT])
        inputArray[1] = true;
    if (keyboardState[SDL_SCANCODE_DOWN])
        inputArray[2] = true;
    if (keyboardState[SDL_SCANCODE_LEFT])
        inputArray[3] = true;
    if (keyboardState[45])
        inputArray[4] = true;
    if (keyboardState[56])
        inputArray[5] = true;
}

bool Input::upPressed() { return inputArray[0]; }
bool Input::rightPressed() { return inputArray[1]; }
bool Input::downPressed() { return inputArray[2]; }
bool Input::leftPressed() { return inputArray[3]; }
bool Input::zoomInPressed() { return inputArray[4]; }
bool Input::zoomOutPressed() { return inputArray[5]; }

