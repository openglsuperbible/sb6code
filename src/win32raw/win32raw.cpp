/*
 * Copyright © 2012-2013 Graham Sellers
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#define WIN32_LEAN_AND_MEAN 1
#define WIN32_EXTRA_LEAN 1
#include <windows.h>

#include <GL/GL.h>
#pragma comment (lib, "opengl32.lib")

static struct
{
    int window_width;
    int window_height;
} globals;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_SIZE:
            globals.window_width = LOWORD(lParam);
            globals.window_height = HIWORD(lParam);
            break;

        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

static void DrawScene()
{
    glViewport(0, 0, globals.window_width, globals.window_height);

    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

int CALLBACK WinMain(
  _In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPSTR lpCmdLine,
  _In_  int nCmdShow
)
{
    MSG msg;
    WNDCLASS cls;

    ::ZeroMemory(&cls, sizeof(cls));

    cls.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    cls.lpfnWndProc = &WindowProc;
    cls.hInstance = ::GetModuleHandle(NULL);
    cls.lpszClassName = TEXT("OPENGL");

    ::RegisterClass(&cls);

    HWND hWnd = ::CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
                                 TEXT("OPENGL"),
                                 TEXT("OpenGL Window"),
                                 WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                 0, 0,
                                 800, 600,
                                 NULL,
                                 NULL,
                                 hInstance,
                                 NULL);

    HDC dc = ::GetDC(hWnd);

    PIXELFORMATDESCRIPTOR pfd;

    ::ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize               = sizeof(pfd);
    pfd.nVersion            = 1;
    pfd.dwFlags             = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER;
    pfd.iPixelType          = PFD_TYPE_RGBA;
    pfd.cColorBits          = 24;
    pfd.cRedBits            = 8;
    pfd.cGreenBits          = 8;
    pfd.cBlueBits           = 8;
    pfd.cDepthBits          = 32;

    int iPixelFormat = ::ChoosePixelFormat(dc, &pfd);
    ::SetPixelFormat(dc, iPixelFormat, &pfd);

    HGLRC rc = wglCreateContext(dc);
    wglMakeCurrent(dc, rc);

    for (;;)
    {
        if (::PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        DrawScene();
        ::SwapBuffers(dc);
    }

    wglMakeCurrent(dc, NULL);
    wglDeleteContext(rc);

    ::ReleaseDC(hWnd, dc);

    ::DestroyWindow(hWnd);

    ::UnregisterClass(TEXT("OPENGL"), ::GetModuleHandle(NULL));

    return 0;
}
