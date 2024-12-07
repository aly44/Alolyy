#pragma once

#include "Math/vector.h"

namespace fw
{
    class ShaderProgram;
    class Texture;

    struct VertexFormat
    {
        vec2 pos;
        unsigned char r, g, b, a;
        vec2 uv;
    };

    class Mesh
    {
    public:
        Mesh(std::vector<VertexFormat> verts, int primitiveType);
        virtual ~Mesh();

        void build(std::vector<VertexFormat> verts, int primitiveType);

        void draw(ShaderProgram* shader, vec2 pos, Texture* texture);

    protected:
        GLuint m_VBO = 0;
        int m_numVerts = 0;
        int m_primitiveType = GL_POINTS;
    };
}
