#ifndef INSTANCE2_HPP
#define INSTANCE2_HPP

#include <iostream>
#include <string>
#include "raylib.h"

using namespace std;

class Instance2 {
    private:
        string Name;
        string Type;
        Vector2 Position;
        Vector2 Dimensions;
        Color InstanceColor;
        int Sides = 0;
    public:
        void SetPosition(float x, float y) {
            Position = {x, y};
            cout << Position.x << ", " << Position.y << endl;
        }

        void SetDimensions(float width, float height) {
            Dimensions = {width, height};
            cout << Dimensions.x << ", " << Dimensions.y << endl;
        }

        void SetColor(const Color& color) {
            InstanceColor = color;
            cout << "Color values: (" << (int)InstanceColor.r << ", " << (int)InstanceColor.g << ", " << (int)InstanceColor.b << ", " << (int)InstanceColor.a << ")" << endl;
        }

        void SetType(const string& type) {
            Type = type;
            cout << "Type set to: " << Type << endl;
        }

        string GetName() const { return Name; }
        string GetType() const { return Type; }
        Vector2 GetPosition() const { return Position; }
        Vector2 GetDimensions() const { return Dimensions; }
        Color GetColor() const { return InstanceColor; }
        int GetSides() const { return Sides; }

        Instance2(const string& name, const string& type, const Vector2& position, const Vector2& dimensions, const Color& color, int sides) : Name(name), Type(type), Position(position), Dimensions(dimensions), InstanceColor(color), Sides(sides) {}
        virtual ~Instance2() = default;

        virtual void DrawInstance() = 0;
};

class Rectangle2 : public Instance2 {
    public:
        Rectangle2(const string& name, const Vector2& position, const Vector2& dimensions, const Color& color, int sides = 4) : Instance2(name, "Rectangle", position, dimensions, color, sides) {}

        void DrawInstance() override {
            DrawRectangleV(GetPosition(), GetDimensions(), GetColor());
        }
};

class Circle2 : public Instance2 {
    public:
        Circle2(const string& name, const Vector2& position, float radius, const Color& color, int sides = 90) : Instance2(name, "Circle", position, {radius * 2, radius * 2}, color, sides) {}

        void DrawInstance() override {
            DrawCircleV(GetPosition(), GetDimensions().x / 2, GetColor());
        }
};

class RightTriangle2 : public Instance2 {
    public:
        RightTriangle2(const string& name, const Vector2& position, const Vector2& dimensions, const Color& color, int sides = 3) : Instance2(name, "RightTriangle", position, dimensions, color, sides) {}

        void DrawInstance() override {
            Vector2 p1 = GetPosition();
            Vector2 p2 = { p1.x + GetDimensions().x, p1.y };
            Vector2 p3 = { p1.x + GetDimensions().x / 2, p1.y - GetDimensions().y };
            DrawTriangle(p1, p2, p3, GetColor());
        }
};

class EquilateralTriangle2 : public Instance2 {
    public:
        EquilateralTriangle2(const string& name, const Vector2& position, float sideLength, const Color& color, int sides = 3) : Instance2(name, "EquilateralTriangle", position, {sideLength, sideLength}, color, sides) {}

        void DrawInstance() override {
            Vector2 p1 = GetPosition();
            Vector2 p2 = { p1.x + GetDimensions().x / 2, p1.y - GetDimensions().y };
            Vector2 p3 = { p1.x - GetDimensions().x / 2, p1.y - GetDimensions().y };
            DrawTriangle(p1, p2, p3, GetColor());
        }
};

class IsocelesTriangle2 : public Instance2 {
    public:
        IsocelesTriangle2(const string& name, const Vector2& position, float baseWidth, float height, const Color& color, int sides = 3) : Instance2(name, "IsocelesTriangle", position, {baseWidth, height}, color, sides) {}

        void DrawInstance() override {
            Vector2 p1 = GetPosition();
            Vector2 p2 = { p1.x + GetDimensions().x, p1.y - GetDimensions().y };
            Vector2 p3 = { p1.x - GetDimensions().x, p1.y - GetDimensions().y };
            DrawTriangle(p1, p2, p3, GetColor());
        }
};

class Polygon2 : public Instance2 {
    public:
        Polygon2(const string& name, const Vector2& position, int sides, float length, const Color& color, int _sides) : Instance2(name, "Polygon", position, {length, length}, color, _sides) {}

        void DrawInstance() override {
            DrawPoly(GetPosition(), GetSides(), GetPosition().x / 2, 0, GetColor());
        }
};

#endif