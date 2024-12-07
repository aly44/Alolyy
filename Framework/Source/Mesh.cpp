#include "Core/CoreHeaders.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"

namespace fw
{
    Mesh::Mesh(std::vector<VertexFormat> verts, int primitiveType)
    {
        build( verts, primitiveType );
    }

    Mesh::~Mesh()
    {
    }

    void Mesh::build(std::vector<VertexFormat> verts, int primitiveType)
    {
        glDeleteBuffers( 1, &m_VBO );

        glGenBuffers( 1, &m_VBO );
        glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof(VertexFormat) * verts.size(), verts.data(), GL_STATIC_DRAW );

        m_primitiveType = primitiveType;
        m_numVerts = (int)verts.size();
    }

    void Mesh::draw(ShaderProgram* shader, vec2 pos, Texture* texture)
    {
        glUseProgram( shader->getProgram() );

        // Setup uniforms.
        GLint positionLoc = glGetUniformLocation( shader->getProgram(), "u_objectTranslation" );
        glUniform2f( positionLoc, pos.x, pos.y );

        if( texture )
        {
            int textureUnitNumber = 8;
            glActiveTexture( GL_TEXTURE0 + textureUnitNumber );
            glBindTexture( GL_TEXTURE_2D, texture->getTextureID() );
            GLint colorTexture = glGetUniformLocation( shader->getProgram(), "u_texture" );
            glUniform1i( colorTexture, textureUnitNumber );
        }

        // Enable VBO.
        glBindBuffer( GL_ARRAY_BUFFER, m_VBO );

        // Setup attributes.
        GLint aposLoc = glGetAttribLocation( shader->getProgram(), "a_position" );
        if( aposLoc != -1 )
        {
            glEnableVertexAttribArray( aposLoc );
            glVertexAttribPointer( aposLoc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0 );
        }

        GLint acolorLoc = glGetAttribLocation( shader->getProgram(), "a_colour" );
        if( acolorLoc != -1 )
        {
            glEnableVertexAttribArray( acolorLoc );
            glVertexAttribPointer( acolorLoc, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexFormat), (void*)8 );
        }

        GLint aUVLoc = glGetAttribLocation( shader->getProgram(), "a_UV" );
        if( aUVLoc != -1 )
        {
            glEnableVertexAttribArray( aUVLoc );
            glVertexAttribPointer( aUVLoc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)12 );
        }

        // Draw the primitive.
        glDrawArrays( m_primitiveType, 0, m_numVerts );

        // Disable attributes.
        if( aposLoc == -1 )
            glDisableVertexAttribArray( aposLoc );

        if( acolorLoc == -1 )
            glDisableVertexAttribArray( acolorLoc );

        if( aUVLoc == -1 )
            glDisableVertexAttribArray( aUVLoc );
    }
}
