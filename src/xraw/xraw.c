// GLXBasics.c
// Use GLX to setup OpenGL windows
// Draw eyeballs
// OpenGL SuperBible, 5th Edition
// Nick Haemel

#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
// #include <glxew.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265

// Store all system info in one place
typedef struct RenderContextRec
{
    GLXContext ctx;
    Display *dpy;
    Window win;
    int nWinWidth;
    int nWinHeight;
    int nMousePosX;
    int nMousePosY;

} RenderContext;

void EarlyInitGLXfnPointers()
{
/*
    glGenVertexArraysAPPLE = (void(*)(GLsizei, const GLuint*))glXGetProcAddressARB((GLubyte*)"glGenVertexArrays");
    glBindVertexArrayAPPLE = (void(*)(const GLuint))glXGetProcAddressARB((GLubyte*)"glBindVertexArray");
    glDeleteVertexArraysAPPLE = (void(*)(GLsizei, const GLuint*))glXGetProcAddressARB((GLubyte*)"glGenVertexArrays");
 glXCreateContextAttribsARB = (GLXContext(*)(Display* dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list))glXGetProcAddressARB((GLubyte*)"glXCreateContextAttribsARB");
 glXChooseFBConfig = (GLXFBConfig*(*)(Display *dpy, int screen, const int *attrib_list, int *nelements))glXGetProcAddressARB((GLubyte*)"glXChooseFBConfig");
 glXGetVisualFromFBConfig = (XVisualInfo*(*)(Display *dpy, GLXFBConfig config))glXGetProcAddressARB((GLubyte*)"glXGetVisualFromFBConfig");
*/
}

void CreateWindow(RenderContext *rcx)
{
    XSetWindowAttributes winAttribs;
    GLint winmask;
    GLint nMajorVer = 0;
    GLint nMinorVer = 0;
    XVisualInfo *visualInfo;
    GLXFBConfig *fbConfigs;
    int numConfigs = 0;
    static int fbAttribs[] = {
                    GLX_RENDER_TYPE,   GLX_RGBA_BIT,
                    GLX_X_RENDERABLE,  True,
                    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                    GLX_DOUBLEBUFFER,  True,
                    GLX_RED_SIZE,      8,
                    GLX_BLUE_SIZE,     8,
                    GLX_GREEN_SIZE,    8,
                    0 };

    EarlyInitGLXfnPointers();

    // Tell X we are going to use the display
    rcx->dpy = XOpenDisplay(NULL);

    // Get Version info
    glXQueryVersion(rcx->dpy, &nMajorVer, &nMinorVer);
    printf("Supported GLX version - %d.%d\n", nMajorVer, nMinorVer);   

    if(nMajorVer == 1 && nMinorVer < 2)
    {
        printf("ERROR: GLX 1.2 or greater is necessary\n");
        XCloseDisplay(rcx->dpy);
        exit(0);
    }
    // Get a new fb config that meets our attrib requirements
    fbConfigs = glXChooseFBConfig(rcx->dpy, DefaultScreen(rcx->dpy), fbAttribs, &numConfigs);
    visualInfo = glXGetVisualFromFBConfig(rcx->dpy, fbConfigs[0]);

    // Now create an X window
    winAttribs.event_mask = ExposureMask | VisibilityChangeMask | 
                            KeyPressMask | PointerMotionMask    |
                            StructureNotifyMask ;

    winAttribs.border_pixel = 0;
    winAttribs.bit_gravity = StaticGravity;
    winAttribs.colormap = XCreateColormap(rcx->dpy, 
                                          RootWindow(rcx->dpy, visualInfo->screen), 
                                          visualInfo->visual, AllocNone);
    winmask = CWBorderPixel | CWBitGravity | CWEventMask| CWColormap;

    rcx->win = XCreateWindow(rcx->dpy, DefaultRootWindow(rcx->dpy), 20, 20,
                 rcx->nWinWidth, rcx->nWinHeight, 0, 
                             visualInfo->depth, InputOutput,
                 visualInfo->visual, winmask, &winAttribs);

    XMapWindow(rcx->dpy, rcx->win);

    // Also create a new GL context for rendering
    GLint attribs[] = {
      GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
      GLX_CONTEXT_MINOR_VERSION_ARB, 2,
      GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
      0 };
    rcx->ctx = glXCreateContextAttribsARB(rcx->dpy, fbConfigs[0], 0, True, attribs);
    glXMakeCurrent(rcx->dpy, rcx->win, rcx->ctx);
#if 0
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
#endif 

    const GLubyte *s = glGetString(GL_VERSION);
    printf("GL Version = %s\n", s);
}

void SetupGLState(RenderContext *rcx)
{
    float aspectRatio = rcx->nWinHeight ? (float)rcx->nWinWidth/(float)rcx->nWinHeight : 1.0f;
    float fYTop     = 0.05f;
    float fYBottom  = - fYTop;
    float fXLeft    = fYTop     * aspectRatio;
    float fXRight   = fYBottom  * aspectRatio;

    glViewport(0,0,rcx->nWinWidth,rcx->nWinHeight);
    glScissor(0,0,rcx->nWinWidth,rcx->nWinHeight);

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Clear matrix stack
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
        
    // Set the frustrum
    glFrustum(fXLeft, fXRight, fYBottom, fYTop, 0.1f, 100.f);
}

void DrawCircle()
{
    float fx = 0.0;
    float fy = 0.0;
    
    int nDegrees = 0;

    // Use a triangle fan with 36 tris for the circle
    glBegin(GL_TRIANGLE_FAN);
      glVertex2f(0.0, 0.0);
      for(nDegrees = 0; nDegrees < 360; nDegrees+=10)
      {
          fx = sin((float)nDegrees*PI/180);
          fy = cos((float)nDegrees*PI/180);
          glVertex2f(fx, fy);
      }
      glVertex2f(0.0f, 1.0f);
    glEnd();
}


void Draw(RenderContext *rcx)
{
    float fRightX = 0.0;
    float fRightY = 0.0;
    float fLeftX = 0.0;
    float fLeftY = 0.0;

    int nLtEyePosX = (rcx->nWinWidth)/2 - (0.3 * ((rcx->nWinWidth)/2));
    int nLtEyePosY = (rcx->nWinHeight)/2;
    int nRtEyePosX = (rcx->nWinWidth)/2 + (0.3 * ((rcx->nWinWidth)/2));
    int nRtEyePosY = (rcx->nWinHeight)/2;

    double fLtVecMag = 100;
    double fRtVecMag = 100;

    // Use the eyeball width as the minimum
    double fMinLength =  (0.1 * ((rcx->nWinWidth)/2));

    // Calculate the length of the vector from the eyeball
    // to the mouse pointer
    fLtVecMag = sqrt( pow((double)(rcx->nMousePosX - nLtEyePosX), 2.0) + 
                      pow((double)(rcx->nMousePosY - nLtEyePosY), 2.0));

    fRtVecMag =  sqrt( pow((double)(rcx->nMousePosX - nRtEyePosX), 2.0) + 
                      pow((double)(rcx->nMousePosY - nRtEyePosY), 2.0));

    // Clamp the minimum magnatude
    if (fRtVecMag < fMinLength)
      fRtVecMag = fMinLength;
    if (fLtVecMag < fMinLength)
      fLtVecMag = fMinLength;

    // Normalize the vector components
    fLeftX = (float)(rcx->nMousePosX - nLtEyePosX) / fLtVecMag;
    fLeftY = -1.0 * (float)(rcx->nMousePosY - nLtEyePosY) / fLtVecMag;
    fRightX = (float)(rcx->nMousePosX - nRtEyePosX) / fRtVecMag;
    fRightY = -1.0 * ((float)(rcx->nMousePosY - nRtEyePosY) / fRtVecMag);

    glClear(GL_COLOR_BUFFER_BIT);

     // Clear matrix stack
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw left eyeball
    glColor3f(1.0, 1.0, 1.0);
    glScalef(0.20, 0.20, 1.0);
    glTranslatef(-1.5, 0.0, 0.0);
    DrawCircle();

    // Draw black
    glColor3f(0.0, 0.0, 0.0);
    glScalef(0.40, 0.40, 1.0);
    glTranslatef(fLeftX, fLeftY, 0.0);
    DrawCircle();

    // Draw right eyeball
    glColor3f(1.0, 1.0, 1.0);
    glLoadIdentity();
    glScalef(0.20, 0.20, 1.0);
    glTranslatef(1.5, 0.0, 0.0);
    DrawCircle();

    // Draw black
    glColor3f(0.0, 0.0, 0.0);
    glScalef(0.40, 0.40, 1.0);
    glTranslatef(fRightX, fRightY, 0.0);
    DrawCircle();

    // Clear matrix stack
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw Nose
    glColor3f(0.5, 0.0, 0.7);
    glScalef(0.20, 0.20, 1.0);
    glTranslatef(0.0, -1.5, 0.0);

    glBegin(GL_TRIANGLES);
      glVertex2f(0.0, 1.0);
      glVertex2f(-0.5, -1.0);
      glVertex2f(0.5, -1.0);
    glEnd();

    // Display rendering
    glXSwapBuffers(rcx->dpy, rcx->win);    
}

void Cleanup(RenderContext *rcx)
{
    // Unbind the context before deleting
    glXMakeCurrent(rcx->dpy, None, NULL);

    glXDestroyContext(rcx->dpy, rcx->ctx);
    rcx->ctx = NULL;

    XDestroyWindow(rcx->dpy, rcx->win);
    rcx->win = (Window)NULL;

    XCloseDisplay(rcx->dpy);
    rcx->dpy = 0;
}

int main(int argc, char* argv[])
{
    Bool bWinMapped = False;
    RenderContext rcx;

    // Set initial window size
    rcx.nWinWidth = 400;
    rcx.nWinHeight = 200;

    // Set initial mouse position
    rcx.nMousePosX = 0;
    rcx.nMousePosY = 0;

    // Setup X window and GLX context
    CreateWindow(&rcx);
    SetupGLState(&rcx);

    // Draw the first frame before checking for messages
    Draw(&rcx);

    // Execute loop the whole time the app runs
    for(;;)
    {
        XEvent newEvent;
        XWindowAttributes winData;

        // Watch for new X events
        XNextEvent(rcx.dpy, &newEvent);

        switch(newEvent.type)
        {
        case UnmapNotify:
        bWinMapped = False;
        break;
        case MapNotify :
        bWinMapped = True;
        case ConfigureNotify:
        XGetWindowAttributes(rcx.dpy, rcx.win, &winData);
        rcx.nWinHeight = winData.height;
        rcx.nWinWidth = winData.width;
        SetupGLState(&rcx);
        break;
        case MotionNotify:
        rcx.nMousePosX = newEvent.xmotion.x;
        rcx.nMousePosY = newEvent.xmotion.y;
        Draw(&rcx);
        break;
        case KeyPress:
        case DestroyNotify:
        Cleanup(&rcx);
        exit(0);
        break;
        }

        if(bWinMapped)
        {
            Draw(&rcx);
        }
    }

    Cleanup(&rcx);
    
    return 0;
}
