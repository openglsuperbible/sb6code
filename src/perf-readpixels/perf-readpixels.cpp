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

#include <sb6.h>

#include <shader.h>
#include <object.h>
#include <vmath.h>

#ifndef _WIN32
#include <unistd.h>
#endif

class perf_readpixels_app : public sb6::application
{
public:
    perf_readpixels_app()
        : render_program(0),
          mode(MODE_READ_OFF),
          paused(false)
    {
    }

protected:
    void init()
    {
        static const char title[] = "OpenGL SuperBible - ReadPixels Test";

        sb6::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    void startup();

    void render(double currentTime);

    void onKey(int key, int action);

    void load_shaders();

    GLuint              render_program;
    GLuint              pixel_buffers[5];

    sb6::object         object;

    enum MODE
    {
        MODE_READ_OFF,
        MODE_READ_TO_SYSMEM,
        MODE_READ_PBO_SAME_FRAME,
        MODE_READ_PBO_FRAME1,
        MODE_READ_PBO_FRAME2,
        MODE_READ_PBO_FRAME3,
        MODE_READ_PBO_FRAME4
    } mode;
    bool                paused;

    struct
    {
        GLint proj_matrix;
        GLint mv_matrix;
    } uniforms;
};

void perf_readpixels_app::startup()
{
    object.load("media/objects/dragon.sbm");

    load_shaders();

    glGenBuffers(5, &pixel_buffers[0]);

    int i;

    for (i = 0; i < 5; i++)
    {
        glBindBuffer(GL_PIXEL_PACK_BUFFER, pixel_buffers[i]);
        glBufferData(GL_PIXEL_PACK_BUFFER, 30 * 30 * 4, NULL, GL_STREAM_COPY);
    }
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void perf_readpixels_app::render(double currentTime)
{
    static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    static const GLfloat one = 1.0f;
    float f = (float)currentTime;
    unsigned char pixel[4 * 30 * 30];
    static unsigned int frame_index = 0;

#if _MSC_VER
    if (paused)
    {
        Sleep(200);
    }
#else
    if (paused)
    {
        usleep(200000);
    }
#endif

    glClearBufferfv(GL_COLOR, 0, black);
    glClearBufferfv(GL_DEPTH, 0, &one);

    glUseProgram(render_program);

    vmath::mat4 proj_matrix = vmath::perspective(50.0f,
                                                    (float)info.windowWidth / (float)info.windowHeight,
                                                    0.1f,
                                                    1000.0f);

    vmath::mat4 mv_matrix = vmath::translate(0.0f, -5.0f, -100.0f) *
                            vmath::rotate(f * 5.0f, 0.0f, 1.0f, 0.0f) *
                            vmath::mat4::identity();

    glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, proj_matrix);
    glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, mv_matrix);

    object.render(8 * 8);

    switch (mode)
    {
        case MODE_READ_OFF:
            break;
        case MODE_READ_TO_SYSMEM:
            glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
            glReadPixels(info.windowWidth / 2,
                         info.windowHeight / 2,
                         30, 30,
                         GL_RGBA, GL_UNSIGNED_BYTE,
                         pixel);
            break;
        case MODE_READ_PBO_SAME_FRAME:
        case MODE_READ_PBO_FRAME1:
        case MODE_READ_PBO_FRAME2:
        case MODE_READ_PBO_FRAME3:
        case MODE_READ_PBO_FRAME4:
            glBindBuffer(GL_PIXEL_PACK_BUFFER, pixel_buffers[frame_index % 5]);
            glReadPixels(info.windowWidth / 2,
                         info.windowHeight / 2,
                         30, 30,
                         GL_RGBA, GL_UNSIGNED_BYTE,
                         0);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, pixel_buffers[(frame_index - (mode - MODE_READ_PBO_SAME_FRAME)) % 5]);
            glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, 30 * 30 * 4, GL_MAP_READ_BIT);
            glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
            break;
    };

    frame_index++;
}

void perf_readpixels_app::onKey(int key, int action)
{
    if (!action)
        return;

    switch (key)
    {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
            mode = (MODE)(key - '1');
            break;
        case 'P':
            paused = !paused;
            break;
        case 'R':
            load_shaders();
            break;
    };
}

void perf_readpixels_app::load_shaders()
{
    if (render_program)
        glDeleteProgram(render_program);

    GLuint shaders[] =
    {
        sb6::shader::load("media/shaders/perftest/render.vs.glsl", GL_VERTEX_SHADER),
        sb6::shader::load("media/shaders/perftest/render.fs.glsl", GL_FRAGMENT_SHADER)
    };

    render_program = sb6::program::link_from_shaders(shaders, 2, true);

    uniforms.proj_matrix = glGetUniformLocation(render_program, "proj_matrix");
    uniforms.mv_matrix = glGetUniformLocation(render_program, "mv_matrix");
}

DECLARE_MAIN(perf_readpixels_app)
