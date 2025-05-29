#pragma once
#include <SDL3/SDL.h>

constexpr SDL_Keycode KEYSET_QWERTY[0x10]{
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_4,
    SDLK_Q,
    SDLK_W,
    SDLK_E,
    SDLK_R,
    SDLK_A,
    SDLK_S,
    SDLK_D,
    SDLK_F,
    SDLK_Z,
    SDLK_X,
    SDLK_C,
    SDLK_V,
};

uint8_t key_lookup(SDL_Keycode key, const SDL_Keycode* _keyset = &KEYSET_QWERTY[0]) {
    for (int i = 0; i < 0x10; ++i) {
        if (key == *(_keyset + i)) {
            return i;
        }
    }
    return 0xFF;
}
