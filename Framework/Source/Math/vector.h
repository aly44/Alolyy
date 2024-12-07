#pragma once

namespace fw {

    class ivec2
    {
    public:
        int x, y;
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

        float length() { return sqrtf( x*x + y*y ); }
        float dot(vec2 o) { return x*o.x + y*o.y; }

        void normalize()
        {
            float len = length();
            if( len == 0 )
                return;
            x /= len;
            y /= len;
        }

    public:
        float x = 0;
        float y = 0;
    };

} // namespace fw