/*
 * Copyright � 2012-2013 Graham Sellers
 *
 * This code is part of the OpenGL SuperBible, 6th Edition.
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

#ifndef __SB6_H__
#define __SB6_H__

#ifdef WIN32
    #pragma once
    #define _CRT_SECURE_NO_WARNINGS 1

    #define WIN32_LEAN_AND_MEAN 1
    #include <Windows.h>

    #ifdef _DEBUG
        #ifdef _WIN64
            #pragma comment (lib, "GLFW_d64.lib")
            #ifndef IN_SB6_LIB
                #pragma comment (lib, "sb6_d64.lib")
            #endif
        #else
            #pragma comment (lib, "GLFW_d32.lib")
            #ifndef IN_SB6_LIB
                #pragma comment (lib, "sb6_d32.lib")
            #endif
        #endif
    #else
        #ifdef _WIN64
            #pragma comment (lib, "GLFW_r64.lib")
            #ifndef IN_SB6_LIB
                #pragma comment (lib, "sb6_r64.lib")
            #endif
        #else
            #pragma comment (lib, "GLFW_r32.lib")
            #ifndef IN_SB6_LIB
                #pragma comment (lib, "sb6_r32.lib")
            #endif
        #endif
    #endif

    #pragma comment (lib, "OpenGL32.lib")
#endif

#include "GL/gl3w.h"

#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1
#include "GLFW/glfw3.h"

#include "sb6ext.h"

#include <stdio.h>
#include <string.h>

namespace sb6
{

class application
{
public:
    application() {}
    virtual ~application() {}
    virtual void run(sb6::application* the_app)
    {
        bool running = true;
        app = the_app;

        if (!glfwInit())
        {
            fprintf(stderr, "Failed to initialize GLFW\n");
            return;
        }

        init();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.minorVersion);

#ifdef _DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif /* _DEBUG */
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_SAMPLES, info.samples);
        glfwWindowHint(GLFW_STEREO, info.flags.stereo ? GL_TRUE : GL_FALSE);

	if (info.flags.fullscreen)
	{
            if (info.windowWidth == 0 || info.windowHeight == 0)
            {        
                const GLFWvidmode* mode;
                mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
                info.windowWidth = mode->width;
				info.windowHeight = mode->height;
	    }
	    currwindow = glfwCreateWindow(info.windowWidth, info.windowHeight, "Hello OpenGL", glfwGetPrimaryMonitor(), NULL);
            if (!currwindow)
            {
                fprintf(stderr, "Failed to open window\n");
		glfwTerminate();
                return;
            }
	    glfwSwapInterval((int)info.flags.vsync);
        }
        else
        {
       	    currwindow = glfwCreateWindow(info.windowWidth, info.windowHeight, "Hello OpenGL", NULL, NULL);
            if (!currwindow)
            {
                fprintf(stderr, "Failed to open window\n");
				glfwTerminate();
                return;
            } 
	}
	glfwMakeContextCurrent(currwindow);

        glfwSetWindowTitle(currwindow, info.title);
        glfwSetWindowSizeCallback(currwindow, glfw_onResize);
        glfwSetKeyCallback(currwindow, glfw_onKey);
        glfwSetMouseButtonCallback(currwindow, glfw_onMouseButton);
        glfwSetScrollCallback(currwindow, glfw_onMouseWheel);
        (info.flags.cursor ? glfwSetInputMode(currwindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL) : glfwSetInputMode(currwindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN));

        info.flags.stereo = (glfwGetWindowAttrib(currwindow, GLFW_STEREO) ? 1 : 0);

        gl3wInit();

#ifdef _DEBUG
        fprintf(stderr, "VENDOR: %s\n", (char *)glGetString(GL_VENDOR));
        fprintf(stderr, "VERSION: %s\n", (char *)glGetString(GL_VERSION));
        fprintf(stderr, "RENDERER: %s\n", (char *)glGetString(GL_RENDERER));
#endif

        if (info.flags.debug)
        {
            if (gl3wIsSupported(4, 3))
            {
                glDebugMessageCallback(debug_callback, this);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            }
            else if (sb6IsExtensionSupported("GL_ARB_debug_output"))
            {
                glDebugMessageCallbackARB(debug_callback, this);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
            }
        }

        startup();

        do
        {
            render(glfwGetTime());

            glfwSwapBuffers(currwindow);
			glfwPollEvents();
            running &= (glfwGetKey(currwindow, GLFW_KEY_ESCAPE   ) == GLFW_RELEASE);
        } while(!glfwWindowShouldClose(currwindow) && running);

        shutdown();

        glfwTerminate();
    }

    virtual void init()
    {
        strcpy(info.title, "SuperBible6 Example");
        info.windowWidth = 800;
        info.windowHeight = 600;
#ifdef __APPLE__
        info.majorVersion = 3;
        info.minorVersion = 2;
#else
        info.majorVersion = 4;
        info.minorVersion = 3;
#endif
        info.samples = 0;
        info.flags.all = 0;
        info.flags.cursor = 1;
#ifdef _DEBUG
        info.flags.debug = 1;
#endif
    }

    virtual void startup()
    {

    }

    virtual void render(double currentTime)
    {

    }

    virtual void shutdown()
    {

    }

    virtual void onResize(int w, int h)
    {
        info.windowWidth = w;
        info.windowHeight = h;
    }

    virtual void onKey(int key, int action)
    {

    }

    virtual void onMouseButton(int button, int action)
    {

    }

    virtual void onMouseMove(int x, int y)
    {

    }

    virtual void onMouseWheel(int pos)
    {

    }

    virtual void onDebugMessage(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message)
    {
#ifdef _WIN32
        OutputDebugStringA(message);
        OutputDebugStringA("\n");
#endif /* _WIN32 */
    }

    static void getMousePosition(int& x, int& y)
    {
    	//TODO
    }

public:
    struct APPINFO
    {
        char title[128];
        int windowWidth;
        int windowHeight;
        int majorVersion;
        int minorVersion;
        int samples;
        union
        {
            struct
            {
                unsigned int    fullscreen  : 1;
                unsigned int    vsync       : 1;
                unsigned int    cursor      : 1;
                unsigned int    stereo      : 1;
                unsigned int    debug       : 1;
            };
            unsigned int        all;
        } flags;
    };

protected:
    APPINFO     info;
    static      sb6::application * app;
    GLFWwindow* currwindow;

    static void glfw_onResize(GLFWwindow* win, int w, int h)
    {
        app->onResize(w, h);
    }

    static void glfw_onKey(GLFWwindow* win, int key, int scancode, int action, int mods)
    {
        app->onKey(key, action);
    }

    static void glfw_onMouseButton(GLFWwindow* win, int button, int action, int mods)
    {
        app->onMouseButton(button, action);
    }

    static void glfw_onMouseMove(int x, int y)
    {
        app->onMouseMove(x, y);
    }

    static void glfw_onMouseWheel(GLFWwindow* win, double pos1, double pos2)
    {
        app->onMouseWheel(pos1, pos2);
    }

    void setVsync(bool enable)
    {
        info.flags.vsync = enable ? 1 : 0;
        glfwSwapInterval((int)info.flags.vsync);
    }

    static void APIENTRY debug_callback(GLenum source,
                                        GLenum type,
                                        GLuint id,
                                        GLenum severity,
                                        GLsizei length,
                                        const GLchar* message,
                                        GLvoid* userParam)
    {
        reinterpret_cast<application *>(userParam)->onDebugMessage(source, type, id, severity, length, message);
    }
};

};

#if defined _WIN32
#define DECLARE_MAIN(a)                             \
sb6::application *app = 0;                          \
int CALLBACK WinMain(HINSTANCE hInstance,           \
                     HINSTANCE hPrevInstance,       \
                     LPSTR lpCmdLine,               \
                     int nCmdShow)                  \
{                                                   \
    a *app = new a;                                 \
    app->run(app);                                  \
    delete app;                                     \
    return 0;                                       \
}
#elif defined _LINUX || defined __APPLE__
#define DECLARE_MAIN(a)                             \
int main(int argc, const char ** argv)              \
{                                                   \
    a *app = new a;                                 \
    app->run(app);                                  \
    delete app;                                     \
    return 0;                                       \
}
#else
#error Undefined platform!
#endif

#endif /* __SB6_H__ */

