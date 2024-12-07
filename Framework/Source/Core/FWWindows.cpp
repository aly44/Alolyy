//
// Copyright (c) 2016-2024 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Core/CoreHeaders.h"

#include "Core/FWWindows.h"
#include "Core/GameCore.h"
#include "GL/GLExtensions.h"
#include "GL/WGLExtensions.h"
#include "GL/MyGLContext.h"
#include "Utility.h"

namespace fw {

    // Initialize opengl window on windows, huge chunks taken from nehe
    //    http://nehe.gamedev.net/tutorial/creating_an_opengl_window_%28win32%29/13001/
    // Update to GL 3+ using this info:
    //    https://mariuszbartosik.com/opengl-4-x-initialization-in-windows-without-a-framework/

    // Public methods

    FWWindows::FWWindows(int width, int height)
        : FWCore( width, height )
    {
        init();
    }

    FWWindows::~FWWindows()
    {
        shutdown();
    
        delete m_pMyGLContext;
    }

    int FWWindows::run(GameCore& game)
    {
        m_pRunningGame = &game;

        // Main loop.
        MSG message;
        bool done = false;

        double lastTime = getTimeSinceGameStarted();

        while( !done )
        {
            if( PeekMessage( &message, nullptr, 0, 0, PM_REMOVE ) )
            {
                if( message.message == WM_QUIT )
                {
                    done = true;
                }
                else
                {
                    TranslateMessage( &message );
                    DispatchMessage( &message );
                }
            }
            else
            {
                double currentTime = getTimeSinceGameStarted();
                float deltaTime = (float)(currentTime - lastTime);
                lastTime = currentTime;

                game.update( deltaTime );
                game.draw();

                // Backup the state of the keyboard and mouse.
                for( int i=0; i<256; i++ )
                    m_oldKeyStates[i] = m_keyStates[i];

                for( int i=0; i<3; i++ )
                    m_oldMouseButtonStates[i] = m_mouseButtonStates[i];
            }
        }

        // Truncate wParam in 64-bit mode to an int.
        return static_cast<int>( message.wParam );
    }

    bool FWWindows::isKeyDown(int value)
    {
        assert( value >= 0 && value < 256 );
        return m_keyStates[value];
    }

    bool FWWindows::isMouseButtonDown(int id)
    {
        assert( id >= 0 && id < 3 );
        return m_mouseButtonStates[id];
    }

    void FWWindows::getMouseCoordinates(int* pMouseX, int* pMouseY)
    {
	    assert( pMouseX != nullptr );
	    assert( pMouseY != nullptr );
	
        POINT p;
        if( GetCursorPos( &p ) )
        {
            if( ScreenToClient( m_hWnd, &p ) )
            {
                *pMouseX = p.x;
                *pMouseY = p.y;
            }
        }
    }

    void FWWindows::swapBuffers()
    {
        ::SwapBuffers( m_hDeviceContext );
    }

    void FWWindows::setWindowSize(int width, int height)
    {
        int maxWidth = GetSystemMetrics( SM_CXFULLSCREEN );
        int maxHeight = GetSystemMetrics( SM_CYFULLSCREEN );

        float aspect = static_cast<float>( width ) / height;

        if( width > maxWidth )
        {
            width = maxWidth;
            height = static_cast<int>(maxWidth / aspect);
        }

        if( height > maxHeight )
        {
            width = static_cast<int>(maxHeight * aspect);
            height = maxHeight;
        }

        DWORD dwStyle = static_cast<DWORD>( GetWindowLongPtr( m_hWnd, GWL_STYLE ) );
        DWORD dwExStyle = static_cast<DWORD>( GetWindowLongPtr( m_hWnd, GWL_EXSTYLE ) );
        HMENU menu = GetMenu( m_hWnd );

        // Calculate the full size of the window needed to match our client area of width/height.
        RECT WindowRect = { 0, 0, width, height };
        AdjustWindowRectEx( &WindowRect, dwStyle, menu ? TRUE : FALSE, dwExStyle );

        int windowWidth = WindowRect.right - WindowRect.left;
        int windowHeight = WindowRect.bottom - WindowRect.top;

        SetWindowPos( m_hWnd, 0, 0, 0, windowWidth, windowHeight, SWP_NOZORDER | SWP_NOMOVE );

        resizeWindow( width, height );
    }

    //========================
    // Protected methods.
    //========================

    bool FWWindows::init()
    {
        // Create Our OpenGL Window.
        if( !createGLWindow( "OpenGL Window", m_windowWidth, m_windowHeight, 32, 0, 24, 8, 1, m_fullscreenMode ) )
        {
            PostQuitMessage( 0 );
            return false;
        }

        // Initialize OpenGL Extensions, must be done after OpenGL Context is created.
        openGL_InitExtensions();
        WGL_InitExtensions();

        return true;
    }

    void FWWindows::shutdown()
    {
        killGLWindow( true );
        PostQuitMessage(0);
    }

    void FWWindows::resizeWindow(int width, int height)
    {
        if( height <= 0 ) height = 1;
        if( width <= 0 ) width = 1;

        m_windowWidth = width;
        m_windowHeight = height;
    }

    bool FWWindows::createGLWindow(const char* title, int width, int height, unsigned char colorBits, unsigned char alphaBits, unsigned char zBits, unsigned char stencilBits, unsigned char multisampleSize, bool fullscreenflag)
    {
        DWORD dwExStyle;
        DWORD dwStyle;

        RECT WindowRect;
        WindowRect.left = (long)0;
        WindowRect.right = (long)width;
        WindowRect.top = (long)0;
        WindowRect.bottom = (long)height;

        m_fullscreenMode = fullscreenflag;

        m_hInstance = GetModuleHandle( nullptr );       // Grab an instance for our window.

        // Define and register the window class.
        {
            WNDCLASSEX wc;
            ZeroMemory( &wc, sizeof(wc) );
            wc.cbSize = sizeof( wc );

            wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  // Redraw on move, and own DC for window.
            wc.lpfnWndProc = (WNDPROC)FWWindows::wndProc;   // wndProc handles messages.
            wc.cbClsExtra = 0;                              // No extra window data.
            wc.cbWndExtra = 0;                              // No extra window data.
            wc.hInstance = m_hInstance;                     // Set the instance.
            wc.hIcon = LoadIcon( 0, IDI_WINLOGO );          // Load the default icon.
            wc.hCursor = LoadCursor( 0, IDC_ARROW );        // Load the arrow pointer.
            wc.hbrBackground = 0;                           // No background required.
            wc.lpszMenuName = nullptr;                      // We don't want a menu.
            wc.lpszClassName = "MyWindowClass";             // Set the class name.

            // Attempt to register the Window Class.
            if( !RegisterClassEx( &wc ) )
            {
                return failAndCleanup( "Failed To Register The Window Class." );
            }
        }

        if( m_fullscreenMode )
        {
            DEVMODE dmScreenSettings;                                   // Device mode.
            memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) ); // Makes sure memory's cleared.
            dmScreenSettings.dmSize = sizeof( dmScreenSettings );       // Size of the devmode structure.
            dmScreenSettings.dmPelsWidth  = width;                      // Selected screen width.
            dmScreenSettings.dmPelsHeight = height;                     // Selected screen height.
            dmScreenSettings.dmBitsPerPel = colorBits;                  // Selected bits per pixel.
            dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

            // Try to set selected mode and get results.  NOTE: CDS_FULLSCREEN gets rid of start bar.
            if( ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
            {
                // If the mode fails, offer two options.  Quit or run in a window.
                if( MessageBox( 0, "The requested fullscreen mode is not supported by\nyour video card.\nTry a different resolution.\nUse Windowed Mode Instead?", "", MB_YESNO|MB_ICONEXCLAMATION ) == IDYES )
                {
                    m_fullscreenMode = false;
                }
                else
                {
                    //return failAndCleanup( "Program Will Now Close." );
                    return false;
                }
            }
        }

        if( m_fullscreenMode )
        {
            dwExStyle = WS_EX_APPWINDOW;
            dwStyle = WS_POPUP;
            ShowCursor( false );
        }
        else
        {
            dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
            dwStyle = WS_OVERLAPPEDWINDOW;
        }

        AdjustWindowRectEx( &WindowRect, dwStyle, false, dwExStyle ); // Adjust window to true requested size.

        // Create our window.
        {
            m_hWnd = CreateWindowEx( dwExStyle,           // Extended style for the window.
                     "MyWindowClass",                     // Class name.
                     title,                               // Window title.
                     WS_CLIPSIBLINGS | WS_CLIPCHILDREN |  // Required window style.
                     dwStyle,                             // Selected window style.
                     0, 0,                                // Window position.
                     WindowRect.right-WindowRect.left,    // Calculate adjusted window width.
                     WindowRect.bottom-WindowRect.top,    // Calculate adjusted window height.
                     nullptr,                             // No parent window.
                     nullptr,                             // No menu.
                     m_hInstance,                         // Instance.
                     this );                              // Pass a pointer to this FWWindows object to WM_NCCREATE.
        
            if( m_hWnd == nullptr )
            {
                return failAndCleanup( "Window Creation Error." );
            }
        }

        // Get the device context.
        {
            m_hDeviceContext = GetDC( m_hWnd );
            if( m_hDeviceContext == nullptr )
                return failAndCleanup( "Failed to get the device context." );
        }

        // Create an OpenGL rendering context.
        m_pMyGLContext = new MyGLContext();
        if( m_pMyGLContext->create( m_hInstance, m_hDeviceContext, 4, 5, true, colorBits, alphaBits, zBits, stencilBits, multisampleSize ) == false )
            return failAndCleanup( "Failed to create WGL context." );

        ShowWindow( m_hWnd, SW_SHOW );   // Show the window.
        SetForegroundWindow( m_hWnd );   // Slightly higher priority.
        SetFocus( m_hWnd );              // Sets keyboard focus to the window.
        resizeWindow( width, height );   // Set up our screen.

        return true;
    }

    bool FWWindows::failAndCleanup(const char* pMessage)
    {
        killGLWindow( true );
        MessageBox( 0, pMessage, "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return false;
    }

    void FWWindows::killGLWindow(bool destroyInstance)
    {
        if( m_fullscreenMode )
        {
            ChangeDisplaySettings( nullptr, 0 );
            ShowCursor( true );
        }

        if( m_hDeviceContext && !ReleaseDC( m_hWnd, m_hDeviceContext ) )
        {
            MessageBox( 0, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        }

        if( m_hWnd && !DestroyWindow( m_hWnd ) )
        {
            MessageBox( 0, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        }

        if( destroyInstance && m_hInstance != nullptr )
        {
            if( !UnregisterClass( "MyWindowClass", m_hInstance ) )
            {
                MessageBox( 0, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
            }
            m_hInstance = 0;
        }

        m_hDeviceContext = 0;
        m_hWnd = 0;
    }

    // This is a static method.
    LRESULT CALLBACK FWWindows::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! WARNING !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! READ THIS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // This is a static method, which means it's essentially a global function.
        // It has no "this".
        // That means you can't access variables and methods directly.
        // As a member of the FWWindows class it has full access to private
        //     variables and methods of any instance of the class.
        // To access the single instance of FWWindows that we created
        //     you have to use the pFWWindows pointer initialized just below.
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        // Get a pointer to the FWWindows object associated with this window.
        FWWindows* pFWWindows = (FWWindows*)GetWindowLongPtr( hWnd, GWLP_USERDATA );

        switch( uMsg )
        {
        case WM_NCCREATE:
            {
                // Set the user data for this hWnd to the FWWindows* we passed in, used on first line of this method above.
                CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>( lParam );
                FWWindows* pFWWindows = static_cast<FWWindows*>( pCreateStruct->lpCreateParams );
                SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pFWWindows ) );

                if( pFWWindows )
                {
                    pFWWindows->m_hWnd = hWnd;
                }
            }
            return 1;

        case WM_DESTROY:
            {
                if( pFWWindows )
                {
                    pFWWindows->m_hWnd = nullptr;
                }
            }
            return 0;

        case WM_ACTIVATE:
            {
                if( !HIWORD(wParam) )
                {
                    pFWWindows->m_windowIsActive = true;
                }
                else
                {
                    pFWWindows->m_windowIsActive = false;
                } 
            }
            return 0;

        case WM_SYSCOMMAND:
            {
                switch( wParam )
                {
                // Don't let screensaver or monitor power save mode kick in.
                case SC_SCREENSAVE:
                case SC_MONITORPOWER:
                    return 0;
                }
            }
            break;

        case WM_CLOSE:
            {
                PostQuitMessage( 0 );
            }
            return 0;

        case WM_KEYDOWN:
            {
                bool keyWasPressedLastTimeMessageArrived = lParam & (1 << 30);

                if( keyWasPressedLastTimeMessageArrived == false )
                {
                    if( wParam == VK_ESCAPE && pFWWindows->m_escapeKeyWillQuit )
                        PostQuitMessage( 0 );

                    pFWWindows->m_keyStates[wParam] = true;

                    if( pFWWindows->m_pRunningGame )
                        pFWWindows->m_pRunningGame->onKeyPressed( (int)wParam );
                }
            }
            return 0;

        case WM_KEYUP:
            {
                pFWWindows->m_keyStates[wParam] = false;

                if( pFWWindows->m_pRunningGame )
                    pFWWindows->m_pRunningGame->onKeyReleased( (int)wParam );
            }
            return 0;

        case WM_MOUSEMOVE:
            {
                int x = GET_X_LPARAM( lParam );
                int y = GET_Y_LPARAM( lParam );

                if( pFWWindows->m_pRunningGame )
                    pFWWindows->m_pRunningGame->onMouseMove( (float)x, (float)y );
            }
            return 0;

        case WM_LBUTTONDOWN:
            {
                pFWWindows->m_mouseButtonStates[0] = true;

                int x = GET_X_LPARAM( lParam );
                int y = GET_Y_LPARAM( lParam );

                if( pFWWindows->m_pRunningGame )
                    pFWWindows->m_pRunningGame->onMouseButtonDown( 0, (float)x, (float)y );
            }
            return 0;

        case WM_LBUTTONUP:
            {
                pFWWindows->m_mouseButtonStates[0] = false;

                int x = GET_X_LPARAM( lParam );
                int y = GET_Y_LPARAM( lParam );
            }
            return 0;

        case WM_SIZE:
            {
                pFWWindows->resizeWindow( LOWORD(lParam), HIWORD(lParam) );
            }
            return 0;
        }

        // Pass all unhandled messages to DefWindowProc.
        return DefWindowProc( hWnd, uMsg, wParam, lParam );
    }

} // namespace fw
