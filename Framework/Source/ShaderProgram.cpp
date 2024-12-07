#include "Core/CoreHeaders.h"

#include "ShaderProgram.h"
#include "Utility.h"

namespace fw {

    ShaderProgram::ShaderProgram()
    {
    }

    ShaderProgram::ShaderProgram(const char* vertFilename, const char* fragFilename)
    {
        init( vertFilename, fragFilename );
    }

    ShaderProgram::~ShaderProgram()
    {
        cleanup();
    }

    void ShaderProgram::cleanup()
    {
        if( m_vertShaderString )
            delete[] m_vertShaderString;
        if( m_fragShaderString )
            delete[] m_fragShaderString;

        glDetachShader( m_program, m_vertShader );
        glDetachShader( m_program, m_fragShader );

        if( m_vertShader )
            glDeleteShader( m_vertShader );
        if( m_fragShader )
            glDeleteShader( m_fragShader );
        if( m_program )
            glDeleteProgram( m_program );

        m_vertShaderString = nullptr;
        m_fragShaderString = nullptr;

        m_vertShader = 0;
        m_fragShader = 0;
        m_program = 0;
    }

    void ShaderProgram::compileShader(GLuint& shaderHandle, const char* shaderString)
    {
        const char* strings[] = { shaderString };
        glShaderSource( shaderHandle, 1, strings, nullptr );

        glCompileShader( shaderHandle );

        //GLenum errorcode = glGetError();

        int compiled = 0;
        glGetShaderiv( shaderHandle, GL_COMPILE_STATUS, &compiled );
        if( compiled == 0 )
        {
            int infoLen = 0;
            glGetShaderiv( shaderHandle, GL_INFO_LOG_LENGTH, &infoLen );

            char* infoBuffer = new char[static_cast<int64_t>(infoLen)+1];
            glGetShaderInfoLog( shaderHandle, infoLen+1, nullptr, infoBuffer );
            outputMessage( infoBuffer );

            // If you hit this assert, your shader failed to compile.
            // Read the message inside the infoBuffer variable or check the output window for details.
            assert( false );
            delete[] infoBuffer;

            glDeleteShader( shaderHandle );
            shaderHandle = 0;
        }
    }

    bool ShaderProgram::init(const char* vertFilename, const char* fragFilename)
    {
        m_vertShaderString = loadCompleteFile( vertFilename, nullptr );
        m_fragShaderString = loadCompleteFile( fragFilename, nullptr );

        assert( m_vertShaderString != nullptr && m_fragShaderString != nullptr );
        if( m_vertShaderString == nullptr || m_fragShaderString == nullptr )
            return false;

        return reload();
    }

    bool ShaderProgram::reload()
    {
        assert( m_vertShaderString != nullptr );
        assert( m_fragShaderString != nullptr );

        m_vertShader = glCreateShader( GL_VERTEX_SHADER );
        m_fragShader = glCreateShader( GL_FRAGMENT_SHADER );

        compileShader( m_vertShader, m_vertShaderString );
        compileShader( m_fragShader, m_fragShaderString );

        if( m_vertShader == 0 || m_fragShader == 0 )
        {
            cleanup();
            return false;
        }

        m_program = glCreateProgram();
        glAttachShader( m_program, m_vertShader );
        glAttachShader( m_program, m_fragShader );

        glLinkProgram( m_program );

        int linked = 0;
        glGetProgramiv( m_program, GL_LINK_STATUS, &linked );
        if( linked == 0 )
        {
            int infoLen = 0;
            glGetProgramiv( m_program, GL_INFO_LOG_LENGTH, &infoLen );

            char* infoBuffer = new char[infoLen+1];
            glGetProgramInfoLog( m_program, infoLen+1, nullptr, infoBuffer );
            outputMessage( infoBuffer );

            // If you hit this assert, your shader failed to link.
            // Read the message inside the infoBuffer variable or check the output window for details.
            assert( false );
            delete[] infoBuffer;

            cleanup();
            return false;
        }

        return true;
    }

} // namespace fw
