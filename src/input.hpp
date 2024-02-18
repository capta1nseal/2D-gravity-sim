#ifndef _GRAVITYINPUT_
#define _GRAVITYINPUT_

#include <array>

#include <SDL2/SDL_scancode.h>

class Input
{
public:
    Input();

    void setKeyboardState(const Uint8 *newKeyboardState);

    void updateInputs();

    bool upPressed();
    bool rightPressed();
    bool downPressed();
    bool leftPressed();
    bool zoomInPressed();
    bool zoomOutPressed();

private:
    static const int inputArraySize = 6;
    std::array<bool, inputArraySize> inputArray;
    const Uint8 *keyboardState;
};

#endif
