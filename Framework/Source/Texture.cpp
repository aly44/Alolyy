#include "Core/CoreHeaders.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../Libraries/stb/stb_image.h"

#include "Texture.h"

namespace fw {

    Texture::Texture()
    {
        struct color { unsigned char r, g, b, a; };

        color colorArray[2*2] =
        {
            { 255, 0, 0, 255 },
            { 0, 255, 0, 255 },
            { 0, 0, 255, 255 },
            { 128, 128, 128, 255 },
        };

        int width = 2;
        int height = 2;

        Create( colorArray, 2, 2 );
    }

    Texture::Texture(const char* filename)
    {
        stbi_set_flip_vertically_on_load( true );

        int w, h, numChannels;
        unsigned char* data = stbi_load( filename, &w, &h, &numChannels, 4 );
        assert( data );

        Create( data, w, h );

        stbi_image_free( data );
    }

    Texture::~Texture()
    {
    }

    void Texture::Create(void* pixels, int w, int h)
    {
        // Create a texture buffer
        glGenTextures( 1, &m_textureID );

        // Set texture unit 0 to be the current active one
        int textureUnitIndex = 0;
        glActiveTexture( GL_TEXTURE0 + textureUnitIndex );

        // Bind the texture to the GL_TEXTURE_2D target of the active TIU
        glBindTexture( GL_TEXTURE_2D, m_textureID );

        // Fill the texture buffer with pixel information from "buffer"
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

        glBindTexture( GL_TEXTURE_2D, 0 );
    }

} // namespace fw
