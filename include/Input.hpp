#pragma once

#include "VectorFuncs.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_keyboard.h>
#include "Utility.hpp"

class Input
{
    private:
        bool inputDebug = false;
    public:
        class States {
            public:

            int numkeys = 0;

            const bool* KeyboardState = SDL_GetKeyboardState(&numkeys);

            Internal_Vector KeyboardKeys = Internal_Vector(0, 0);
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

            Mouse() {
                _States = new Input::States();
            };

            Input::States *_States;

            void ChangeMouseValue(int index, bool value) {
                _States->MouseStates.Set(index, value);
            };

            void FillMouseListWithValue(bool value) {
                for (int i = 0; i < (int)_States->MouseStates.GetSize(); i++) {
                    _States->MouseStates.Set(i, value);
                };
            };

            bool IsMouseButtonDown(int index) const {
                //_States->MouseStates.Set(index, true);
                bool deb_bool = _States->MouseStates.GetBool(index); // Gets the bool value at index
                if (deb_bool == true) {
                    return true;
                };
                return false;
            };

            void UpdateMouse(SDL_Event *events) {
                if (events->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                    switch (events->button.button) {
                        case 1:
                            this->ChangeMouseValue(0, true); // Sets mouse button 1 value to true at index 0
                            if (debug_var == true) {
                                SDL_Log("Mouse button 1 value change");
                            }
                            break;
                        case 2:
                            this->ChangeMouseValue(1, true);
                            if (debug_var == true) {
                                SDL_Log("Mouse button 2 value change");
                            }
                            break;
                        case 3:
                            this->ChangeMouseValue(2, true);
                            if (debug_var == true) {
                                SDL_Log("Mouse button 3 value change");
                            }
                            break;
                        case 4:
                            this->ChangeMouseValue(3, true);
                            if (debug_var == true) {
                                SDL_Log("Mouse button 4 value change");
                            }
                            break;
                        case 5:
                            this->ChangeMouseValue(4, true);
                            if (debug_var == true) {
                                SDL_Log("Mouse button 5 value change");
                            }
                            break;
                        default:
                            break;
                    }
                } else if (events->type == SDL_EVENT_MOUSE_BUTTON_UP) {
                    switch (events->button.button) {
                        case 1:
                            this->ChangeMouseValue(0, false);
                            if (debug_var == true) {
                                SDL_Log("Mouse button 1 value change");
                            }
                            break;
                        case 2:
                            this->ChangeMouseValue(1, false);
                            if (debug_var == true) {
                                SDL_Log("Mouse button 2 value change");
                            }
                            break;
                        case 3:
                            this->ChangeMouseValue(2, false);
                            if (debug_var == true) {
                                SDL_Log("Mouse button 3 value change");
                            }
                            break;
                        case 4:
                            this->ChangeMouseValue(3, false);
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

    class Keyboard {
        public:
            Keyboard() {
                KeyboardKeyState = new Input::States();
            }

            ~Keyboard() {
                delete KeyboardKeyState;
            }

            void Update(SDL_Event* events) {
                if (KeyboardKeyState != nullptr) {
                    if (events->type == SDL_EVENT_KEY_DOWN) {
                        if (KeyboardKeyState->KeyboardKeys.GetSize() == 0) {
                            KeyboardKeyState->KeyboardKeys.Append(events->key.scancode);
                        } else {
                            KeyboardKeyState->KeyboardKeys.Set(KeyboardKeyState->KeyboardKeys.GetSize() - 1, events->key.scancode);
                        }
                    }
                    if (events->type == SDL_EVENT_KEY_UP) {
                        if (!KeyboardKeyState->IsKeyDown(events->key.scancode)) {
                            KeyboardKeyState->KeyboardKeys.Set(KeyboardKeyState->KeyboardKeys.GetSize() - 1, 0);
                        }
                    }
                    SDL_Log("%d", events->key.scancode);
                }
            }

            Input::States* KeyboardKeyState;
    };
};
