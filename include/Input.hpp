#pragma once

#include "VectorFuncs.hpp"
#include <array>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_log.h>

class Input
{
    public:
        struct States {
            int numkeys = 0;

            const bool* KeyboardState = SDL_GetKeyboardState(&numkeys);
            std::array<bool, 5> MouseStates {false, false, false, false, false};

            void ChangePressedValue(int index) {

            }

            bool SwitchMouseState(int index, bool value) {
                MouseStates.at(index) = value;

                if (MouseStates.at(index) == value) {
                    return true;
                }
                return false;
            }

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

            bool Debug = false;

            Input::States *_States;

            void ChangeMouseValue(bool value, int index) {
                _States->SwitchMouseState(index, value);
            };

            void FillMouseListWithValue(bool value) {
                for (int i = 0; i < (int)_States->MouseStates.size(); i++) {
                    _States->SwitchMouseState(i, value);
                };
            };

            bool IsMouseButtonDown(int index) {
                if (index != 0) {
                    if (_States->SwitchMouseState(index-1, true)) {
                        return true;
                    };
                } else {
                    if (_States->SwitchMouseState(index, true)) {
                        return true;
                    };
                };

                return false;
            };

            void UpdateMouse(SDL_Event *events) {
                if (events->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                    switch (events->button.button) {
                        case 1:
                            this->ChangeMouseValue(true, 0);
                            if (Debug == true) {
                                SDL_Log("MOUSE 1 %i", _States->MouseStates.at(0));
                            };
                            break;
                        case 2:
                            this->ChangeMouseValue(true, 1);
                            if (Debug == true) {
                                SDL_Log("MOUSE 2 %i", _States->MouseStates.at(1));
                            };
                            break;
                        case 3:
                            this->ChangeMouseValue(true, 2);
                            if (Debug == true) {
                                SDL_Log("MOUSE 3 %i", _States->MouseStates.at(2));
                            };
                            break;
                        default:
                            break;
                    }
                } else if (events->type == SDL_EVENT_MOUSE_BUTTON_UP) {
                    switch (events->button.button) {
                        case 1:
                            this->ChangeMouseValue(false, 0);
                            if (Debug == true) {
                                SDL_Log("MOUSE 1 %i", _States->MouseStates.at(0));
                            };
                            break;
                        case 2:
                            this->ChangeMouseValue(false, 1);
                            if (Debug == true) {
                                SDL_Log("MOUSE 2 %i", _States->MouseStates.at(1));
                            };
                            break;
                        case 3:
                            this->ChangeMouseValue(false, 2);
                            if (Debug == true) {
                                SDL_Log("MOUSE 3 %i", _States->MouseStates.at(2));
                            };
                            break;
                        default:
                            break;
                    }
                };
            };
    };
};
