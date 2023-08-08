#ifndef _GRAVITYINPUT_
#define _GRAVITYINPUT_

#include <iostream>
#include <array>

#include <SDL2/SDL_scancode.h>

class Input
{
public:
    Input()
    {
        for (int i = 0; i < inputArraySize; i++)
            inputArray[i] = false;
    }

    void setKeyboardState(const Uint8 *newKeyboardState)
    {
        keyboardState = newKeyboardState;
    }

    void updateInputs()
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

    bool upPressed() { return inputArray[0]; }
    bool rightPressed() { return inputArray[1]; }
    bool downPressed() { return inputArray[2]; }
    bool leftPressed() { return inputArray[3]; }
    bool zoomInPressed() { return inputArray[4]; }
    bool zoomOutPressed() { return inputArray[5]; }

private:
    static const int inputArraySize = 6;
    std::array<bool, inputArraySize> inputArray;
    const Uint8 *keyboardState;
};

#endif
