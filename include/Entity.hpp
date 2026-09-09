#pragma once

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_video.h>

#include "Core.hpp"
#include "VectorFuncs.hpp"

Core core;

class Object {
public:
    float ID = 0;
    float x;
    float y;


    Vector2 position;

    // Object Class Constructor: Uses the values to create an Object type, a sort of container
    // for SDL_FRect to make it easy to add values a properties
    Object(float _ID, float x, float y) : ID(_ID), x(x), y(y) {

        this->position.x = x;
        this->position.y = y;

        this->position = Vector2(this->position.x, this->position.y);

    };

    void Move(float dx, float dy) {
        this->position.x += dx; // Update the x position inside the Vector2
        this->position.y += dy; // Update the y position inside the Vector2

        this->position = Vector2(this->position.x, this->position.y); // Update the position with the new individual values

    }

    void SetPosition(float x, float y) {
        this->position.x = x;
        this->position.y = y;
        this->position = Vector2(this->position.x, this->position.y);

    }

    void SetPosition_Relative(float dx, float dy) {
        int WinSize_x;
        int WinSize_y;
        SDL_GetWindowSize(core.GetWindow(), &WinSize_x, &WinSize_y);

        this->position.x += dx / WinSize_x;
        this->position.y += dy / WinSize_y;
        this->position = Vector2(this->position.x, this->position.y);

        /*
        if (typeid(*this).name() == "Rect") {
            this->Rect.x = this->position.x;
            this->Rect.y = this->position.y;
        }
        */
    }

};

class Rect : public Object {
public:
    SDL_FRect _Rect;

    Rect(float _ID, float x, float y, float w, float h) : Object(_ID, x, y) {

        SDL_FRect _Rect = {this->position.x, this->position.y, w, h};

        this->_Rect = _Rect;
    }

    const SDL_FRect *GetRect() {
        return &this->_Rect;
    }

    const SDL_FRect *GetRect() const {
        return &this->_Rect;
    }

    const Vector2 *GetPosition() const {
        return &this->position;
    }

    void Move(float dx, float dy) {
        this->position.x += dx; // Update the x position inside the Vector2
        this->position.y += dy; // Update the y position inside the Vector2

        this->position = Vector2(this->position.x, this->position.y); // Update the position with the new individual values

        this->_Rect.x = this->position.x; // Update the x position inside the SDL_FRect
        this->_Rect.y = this->position.y; // Update the y position inside the SDL_FRect
    }

    void SetPosition(float x, float y) {
        this->position.x = x;
        this->position.y = y;
        this->position = Vector2(this->position.x, this->position.y);

        this->_Rect.x = this->position.x;
        this->_Rect.y = this->position.y;
    }

    void Draw() {
        SDL_RenderFillRect(core.GetRenderer(), this->GetRect());
    }
};

class Line : public Object {
public:
    float x2;
    float y2;

    Line(float _ID, float x1, float y1, float x2, float y2) : Object(_ID, x1, y1) {
        this->x2 = x2;
        this->y2 = y2;
        SDL_RenderLine(core.GetRenderer(), x, y, x2, y2);
    }

    void Draw() {
        SDL_RenderLine(core.GetRenderer(), x, y, x2, y2);
    }

    void SetEndPosition(float x2, float y2) {
        this->x2 = x2;
        this->y2 = y2;
    }

    void SetStartPosition(float x1, float y1) {
        this->x = x1;
        this->y = y1;
    }

};
