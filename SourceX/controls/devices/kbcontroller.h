#pragma once

// Keyboard keys acting like gamepad buttons
#ifndef HAS_KBCTRL
#define HAS_KBCTRL 0
#endif

#if HAS_KBCTRL == 1
#include <SDL.h>
#include "controls/controller_buttons.h"

namespace dvl {

ControllerButton KbCtrlToControllerButton(const SDL_Event &event);

bool IsKbCtrlButtonPressed(ControllerButton button);

bool ProcessKbCtrlAxisMotion(const SDL_Event &event);

} // namespace dvl
#endif
