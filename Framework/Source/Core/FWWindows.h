//
// Copyright (c) 2016-2024 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "Core/FWCore.h"

namespace fw {

    class GameCore;
    class MyGLContext;

    class FWWindows : public FWCore
    {
    public:
        FWWindows(int width, int height);
        virtual ~FWWindows();

        virtual int run(GameCore& game) override;

        virtual bool isKeyDown(int value) override;
        virtual bool isMouseButtonDown(int id) override;
        virtual void getMouseCoordinates(int* pMouseX, int* pMouseY) override;

        virtual void swapBuffers() override;

        void setWindowSize(int width, int height);

    protected:
        bool init();
        void shutdown();

        void resizeWindow(int width, int height);
        bool createGLWindow(const char* title, int width, int height, unsigned char colorBits, unsigned char alphaBits, unsigned char zBits, unsigned char stencilBits, unsigned char multisampleSize, bool fullscreenflag);
        bool failAndCleanup(const char* pMessage);
        void killGLWindow(bool destroyInstance);

        static LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    protected:
        HWND m_hWnd = nullptr;
        HGLRC m_hRenderingContext = nullptr;
        HDC m_hDeviceContext = nullptr;
        HINSTANCE m_hInstance = nullptr;
        MyGLContext* m_pMyGLContext = nullptr;

        GameCore* m_pRunningGame = nullptr;

        bool m_keyStates[256] = {};
        bool m_mouseButtonStates[3] = {};
        bool m_oldKeyStates[256] = {};
        bool m_oldMouseButtonStates[3] = {};

        bool m_windowIsActive = false;
        bool m_fullscreenMode = false;
    };

} // namespace fw
