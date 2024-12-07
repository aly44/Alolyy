#pragma once

namespace fw
{
    class ShaderProgram;

    class Texture
    {
    public:
        Texture();
        Texture(const char* filename);
        virtual ~Texture();

        void Create(void* pixels, int w, int h);

        GLuint getTextureID() { return m_textureID; }

    protected:
        GLuint m_textureID = 0;
    };
}
