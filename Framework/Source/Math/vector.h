#pragma once
#include <cmath> // For sqrtf()

namespace fw {

    class ivec2
    {
    public:
        int x, y;

        ivec2() : x(0), y(0) {}
        ivec2(int nx, int ny) : x(nx), y(ny) {}

        ivec2 operator+(const ivec2& other) const { return ivec2(x + other.x, y + other.y); }
        ivec2 operator-(const ivec2& other) const { return ivec2(x - other.x, y - other.y); }

        bool operator==(const ivec2& other) const { return x == other.x && y == other.y; }
        bool operator!=(const ivec2& other) const { return !(*this == other); }

    };


    class vec2
    {
    public:
        vec2() {}
        vec2(float nxy) : x(nxy), y(nxy) {}
        vec2(float nx, float ny) : x(nx), y(ny) {}
        vec2(int nx, int ny) : x((float)nx), y((float)ny) {}

        vec2 operator+(vec2 o) { return vec2(x + o.x, y + o.y); }
        vec2 operator-(vec2 o) { return vec2(x - o.x, y - o.y); }
        vec2 operator*(vec2 o) { return vec2(x * o.x, y * o.y); }
        vec2 operator/(vec2 o) { return vec2(x / o.x, y / o.y); }

        vec2 operator+(float o) { return vec2(x + o, y + o); }
        vec2 operator-(float o) { return vec2(x - o, y - o); }
        vec2 operator*(float o) { return vec2(x * o, y * o); }
        vec2 operator/(float o) { return vec2(x / o, y / o); }

        void operator+=(vec2 o) { x += o.x; y += o.y; }

        // Length methods
        float length() const { return sqrtf(x * x + y * y); }
        float LengthSquared() const { return x * x + y * y; }
        float Length() const { return length(); }

        // Dot product
        float dot(vec2 o) { return x * o.x + y * o.y; }

        // Normalization
        void normalize()
        {
            float len = length();
            if (len == 0)
                return;
            x /= len;
            y /= len;
        }

        vec2 Normalized() const
        {
            vec2 result = *this;
            result.normalize();
            return result;
        }

        void Normalize() { normalize(); } // Alias for compatibility

        // Comparison operators
        bool operator==(const vec2& other) const { return (x == other.x && y == other.y); }
        bool operator!=(const vec2& other) const { return !(*this == other); }

    public:
        float x = 0;
        float y = 0;
    };

} // namespace fw
