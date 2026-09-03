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

            std::array<bool, 5> MouseStates {false, false, false, false, false};
            const bool* KeyboardState = SDL_GetKeyboardState(&numkeys);

            void UpdateMouse(SDL_Event *events);

            void ChangeMouseValue(bool value, int index) {
               this->MouseStates.at(index) = value;
            };

            void FillMouseListWithValue(bool value) {
                for (int i = 0; i < (int)MouseStates.size(); i++) {
                    this->MouseStates.at(i) = value;
                };
            };

            bool IsMouseButtonDown(int index) {
                if (index != 0) {
                    if (this->MouseStates.at(index-1) == true) {
                        return true;
                    };
                } else {
                    if (this->MouseStates.at(index) == true) {
                        return true;
                    };
                };

                return false;
            };

            void ChangePressedValue(int index) {

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

};
