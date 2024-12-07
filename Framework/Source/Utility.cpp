#include "Core/CoreHeaders.h"
#include "Utility.h"

namespace fw {

    double getHighPrecisionTime()
    {
        unsigned __int64 freq;
        unsigned __int64 time;

        QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
        QueryPerformanceCounter((LARGE_INTEGER*)&time);

        double timeseconds = (double)time / freq;

        return timeseconds;
    }

    double getTimeSinceGameStarted()
    {
        static double timeGameStarted = getHighPrecisionTime();
        double timePassed = getHighPrecisionTime() - timeGameStarted;
        return timePassed;
    }

    void outputMessage(const char* message, ...)
    {
        static const int c_MaxMessage = 1024;
        char szBuff[c_MaxMessage];
        va_list arg;
        va_start(arg, message);
        vsnprintf_s( szBuff, sizeof(szBuff), _TRUNCATE, message, arg );
        va_end(arg);

        szBuff[c_MaxMessage-1] = 0; // vsnprintf_s might do this, but docs are unclear.
        OutputDebugString( szBuff );
    }

    char* loadCompleteFile(const char* filename, long* length)
    {
        char* filecontents = 0;

        FILE* filehandle;
        errno_t error = fopen_s( &filehandle, filename, "rb" );

        if( filehandle )
        {
            fseek( filehandle, 0, SEEK_END );
            long size = ftell( filehandle );
            rewind( filehandle );

            filecontents = new char[size+1];
            fread( filecontents, size, 1, filehandle );
            filecontents[size] = 0;

            if( length )
                *length = size;

            fclose( filehandle );
        }

        return filecontents;
    }

} // namespace fw
