#include "include/Input.hpp"

void Input::States::UpdateMouse(SDL_Event *event) {

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        switch (event->button.button) {
            case 1:
                this->ChangeMouseValue(true, 0);
                SDL_Log("MOUSE 1 %i", this->MouseStates.at(0));
                break;
            case 2:
                this->ChangeMouseValue(true, 1);
                SDL_Log("MOUSE 2 %i", this->MouseStates.at(1));
                break;
            case 3:
                this->ChangeMouseValue(true, 2);
                SDL_Log("MOUSE 3 %i", this->MouseStates.at(2));
                break;
            default:
                break;
        }
    } else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        switch (event->button.button) {
            case 1:
                this->ChangeMouseValue(false, 0);
                SDL_Log("MOUSE 1 %i", this->MouseStates.at(0));
                break;
            case 2:
                this->ChangeMouseValue(false, 1);
                SDL_Log("MOUSE 2 %i", this->MouseStates.at(1));
                break;
            case 3:
                this->ChangeMouseValue(false, 2);
                SDL_Log("MOUSE 3 %i", this->MouseStates.at(2));
                break;
            default:
                break;
        }
    };
}
