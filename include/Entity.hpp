#pragma once

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_video.h>

#include "Core.hpp"
#include "VectorFuncs.hpp"

class Object {
    public:
        Vector2 position = Vector2();
        float ID = 0;
        float x = this->position._x;
        float y = this->position._y;

        // Object Class Constructor: Uses the values to create an Object type, a sort of container
        // for SDL_FRect to make it easy to add values a properties
        Object(float _ID, float x, float y) : ID(_ID), x(this->position._x), y(this->position._y) {

            this->ID = _ID;
            this->position = Vector2(x, y);

        };

        void Move(float dx, float dy) {
            this->position._x += dx; // Update the x position inside the Vector2
            this->position._y += dy; // Update the y position inside the Vector2

        }

        void SetPosition(float x, float y) {
            this->position._x = x;
            this->position._y = y;

        }

        void SetPosition_Relative(float dx, float dy) {
            int WinSize_x;
            int WinSize_y;
            SDL_GetWindowSize(Core::GetWindow(), &WinSize_x, &WinSize_y);

            this->position._x += dx / WinSize_x;
            this->position._y += dy / WinSize_y;

            /*
            if (typeid(*this).name() == "Rect") {
                this->Rect.x = this->position._x;
                this->Rect.y = this->position._y;
            }
            */
        }

};

class Rect : public Object {
public:
    struct Properties {
        Vector3 *Color = new Vector3();

        void operator=(const Vector3& rhs) {
            *this->Color = rhs;
        }

        float operator[](int index) const {
            return (index == 0) ? this->Color->x : (index == 1) ? this->Color->y : this->Color->z;
        }
    } props;

    SDL_FRect _Rect;

    Rect(float _ID, float x, float y, float w, float h) : Object(_ID, x, y) {

        SDL_FRect _Rect = {this->position._x, this->position._y, w, h};

        this->_Rect = _Rect;
    }

    Rect(float _ID, float x, float y, float w, float h, Vector3 color) : Object(_ID, x, y) {

        SDL_FRect _Rect = {this->position._x, this->position._y, w, h};

        this->_Rect = _Rect;

        if (color != Vector3()) {
            this->props.Color = new Vector3(color);
        } else {
            this->props.Color = new Vector3();
        }
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

    void Move(float *dx, float *dy) {
        this->position._x += *dx; // Update the x position inside the Vector2
        this->position._y += *dy; // Update the y position inside the Vector2

        this->_Rect.x = this->position._x; // Update the x position inside the SDL_FRect
        this->_Rect.y = this->position._y; // Update the y position inside the SDL_FRect
    }

    void SetPosition(float x, float y) {
        this->position._x = x;
        this->position._y = y;

        this->_Rect.x = this->position._x;
        this->_Rect.y = this->position._y;
    }

    void SetColor(float r, float g, float b) {
        this->props.Color = new Vector3(r, g, b);
    }

    Vector3 *GetColor() {
        return this->props.Color;
    }

    float GetColor(int index) {
        return (index == 0) ? this->props.Color->x : (index == 1) ? this->props.Color->y : this->props.Color->z;
    }

    void Draw() {
        #ifndef _USE_VULKAN
            SDL_SetRenderDrawColorFloat(Core::GetRenderer(), this->GetColor(0), this->GetColor(1), this->GetColor(2), SDL_ALPHA_OPAQUE_FLOAT);
            SDL_RenderFillRect(Core::GetRenderer(), this->GetRect());
        #endif

        #ifdef _USE_VULKAN
            SDL_RenderEvent();
        #endif
    }
};

class Line : public Object {
public:
    Vector2 PositionStart;
    Vector2 PositionEnd;

    struct Properties {
        Vector3 Color = Vector3(0.6f, 0.8f, 0.8f);

        void operator=(const Vector3& rhs) {
            this->Color = rhs;
        }
    } props;

    Line(float _ID, float x1, float y1, float x2, float y2) : Object(_ID, x1, y1) {
        this->PositionStart = Vector2(x1, y1);
        this->PositionEnd = Vector2(x2, y2);
        #ifndef _USE_VULKAN
        SDL_RenderLine(Core::GetRenderer(), PositionStart._x, PositionStart._y, PositionEnd._x, PositionEnd._y);
        #endif
    }

    Line(float _ID, float x1, float y1, float x2, float y2, Vector3 color) : Object(_ID, x1, y1) {
        this->PositionStart = Vector2(x1, y1);
        this->PositionEnd = Vector2(x2, y2);

        if (color != Vector3()) {
            this->props.Color = color;
        } else {
            this->props.Color = Vector3();
        }
        #ifndef _USE_VULKAN
        SDL_RenderLine(Core::GetRenderer(), PositionStart._x, PositionStart._y, PositionEnd._x, PositionEnd._y);
        #endif
    }

    void Draw() {
        #ifndef _USE_VULKAN
            SDL_SetRenderDrawColorFloat(Core::GetRenderer(), this->props.Color[0], this->props.Color[1], this->props.Color[2], SDL_ALPHA_OPAQUE_FLOAT);
            SDL_RenderLine(Core::GetRenderer(), PositionStart._x, PositionStart._y, PositionEnd._x, PositionEnd._y);
        #endif
    }

    void SetEndPosition(float x2, float y2) {
        this->PositionEnd = Vector2(x2, y2);
    }

    void SetStartPosition(float x1, float y1) {
        this->PositionStart = Vector2(x1, y1);
    }

    void Move(float dx1, float dy1, float dx2, float dy2) {
        this->PositionStart._x += dx1;
        this->PositionStart._y += dy1;
        this->PositionEnd._x += dx2;
        this->PositionEnd._y += dy2;
    }

    void SetColor(float r, float g, float b) {
        this->props.Color = Vector3(r, g, b);
    }

    Vector3 GetColor() {
        return this->props.Color;
    }

};
