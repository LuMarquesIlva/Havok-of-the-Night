#pragma once

#include "VectorFuncs.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_keyboard.h>
#include "Utility.hpp"

class Input
{
    public:
        struct States {
            int numkeys = 0;

            const bool* KeyboardState = SDL_GetKeyboardState(&numkeys);
            Internal_Vector MouseStates = Internal_Vector(false, false, false, false, false);

            bool IsKeyDown(SDL_Scancode Key) {

                // Iterate trought the keyboard list
                for (int i = 0; i < numkeys; i++) {

                    if (this->KeyboardState[i] == true) {
                        // Convert scancode integer to keycode
                        SDL_Keycode Keycode = SDL_SCANCODE_TO_KEYCODE(i);

                        if (Keycode == Key) {
                            return true;
                        }
                        return false;
                    };
                };

                return false;
            };
        };

    class Mouse {
        public:
            int x = 0;
            int y = 0;

            Input::States *_States;

            void ChangeMouseValue(bool value, int index) {
                _States->MouseStates.Set(index, value);
            };

            void FillMouseListWithValue(bool value) {
                for (int i = 0; i < (int)_States->MouseStates.GetSize(); i++) {
                    _States->MouseStates.Set(i, value);
                };
            };

            bool IsMouseButtonDown(int index) const {
                _States->MouseStates.Set(index, true);
                bool deb_bool = _States->MouseStates.GetBool(index);
                SDL_Log("Mouse button %d value changed at check and returning %d", index, deb_bool);
                if (deb_bool == true) {
                    SDL_Log("Mouse button %d value changed at check and returning true", index);
                    return true;
                };

                return false;
            };

            void UpdateMouse(SDL_Event *events) {
                if (events->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                    switch (events->button.button) {
                        case 0:
                            this->ChangeMouseValue(true, 0);
                            if (debug_var == true) {
                                SDL_Log("Mouse button 0 value change");
                            }
                            break;
                        case 1:
                            this->ChangeMouseValue(true, 1);
                            if (debug_var == true) {
                                SDL_Log("Mouse button 1 value change");
                            }
                            break;
                        case 2:
                            this->ChangeMouseValue(true, 2);
                            if (debug_var == true) {
                                SDL_Log("Mouse button 2 value change");
                            }
                            break;
                        case 3:
                            this->ChangeMouseValue(true, 3);
                            if (debug_var == true) {
                                SDL_Log("Mouse button 3 value change");
                            }
                            break;
                        case 4:
                            this->ChangeMouseValue(true, 4);
                            if (debug_var == true) {
                                SDL_Log("Mouse button 4 value change");
                            }
                            break;
                        default:
                            break;
                    }
                } else if (events->type == SDL_EVENT_MOUSE_BUTTON_UP) {
                    switch (events->button.button) {
                        case 0:
                            this->ChangeMouseValue(false, 0);
                            if (debug_var == true) {
                                SDL_Log("Mouse button 0 value change");
                            }
                            break;
                        case 1:
                            this->ChangeMouseValue(false, 1);
                            if (debug_var == true) {
                                SDL_Log("Mouse button 1 value change");
                            }
                            break;
                        case 2:
                            this->ChangeMouseValue(false, 2);
                            if (debug_var == true) {
                                SDL_Log("Mouse button 2 value change");
                            }
                            break;
                        case 3:
                            this->ChangeMouseValue(false, 3);
                            if (debug_var == true) {
                                SDL_Log("Mouse button 3 value change");
                            }
                            break;
                        case 4:
                            this->ChangeMouseValue(false, 4);
                            if (debug_var == true) {
                                SDL_Log("Mouse button 4 value change");
                            }
                            break;
                        default:
                            break;
                    }
                };
            };
    };
};
