//
// Copyright (c) 2020-2024 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

namespace fw {

class MyGLContext
{
protected:
    // WGLExtensions need to be acquired once per process, done in acquireWGLExtensions().
    // NOTE: This is not threadsafe.
    static bool m_WGLExtensionsAcquired;

    HDC m_hDeviceContext;
    HGLRC m_hRenderingContext;

protected:
    void destroyRenderingContext(HDC hDeviceContext);

public:
    MyGLContext();
    ~MyGLContext();

    bool acquireWGLExtensions(HINSTANCE hInstance);
    bool failAndCleanup(const char* pMessage);

    bool create(HINSTANCE hInstance, HDC hDeviceContext, int majorVersion, int minorVersion, bool compatibilityMode, unsigned char colorBits, unsigned char alphaBits, unsigned char zBits, unsigned char stencilBits, unsigned char multisampleSize);
    void destroy();
};

} // namespace fw
