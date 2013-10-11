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
#include <vmath.h>
#include <sb6ktx.h>
#include <shader.h>

#include <string>
static void print_shader_log(GLuint shader)
{
    std::string str;
    GLint len;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    str.resize(len);
    glGetShaderInfoLog(shader, len, NULL, &str[0]);

#ifdef _WIN32
    OutputDebugStringA(str.c_str());
#endif
}

class noise_app : public sb6::application
{
public:
    noise_app()
        : prog_noise(0),
          paused(0)
    {

    }

    void init()
    {
        static const char title[] = "OpenGL SuperBible - Noise";

        sb6::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    void startup(void)
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        load_shaders();
    }

    void shutdown(void)
    {
        glDeleteProgram(prog_noise);
        glDeleteVertexArrays(1, &vao);
    }

    void render(double currentTime)
    {
        static const GLfloat black[] = { 0.0f, 0.25f, 0.0f, 1.0f };
        glViewport(0, 0, info.windowWidth, info.windowHeight);

        static double last_time = 0.0;
        static double total_time = 0.0;

        if (!paused)
            total_time += (currentTime - last_time);
        last_time = currentTime;

        float t = (float)total_time;

        glClearBufferfv(GL_COLOR, 0, black);

        vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -2.0f) *
                                vmath::rotate((float)t * 5.0f, 0.0f, 0.0f, 1.0f) *
                                vmath::rotate((float)t * 30.0f, 1.0f, 0.0f, 0.0f);
        vmath::mat4 proj_matrix = vmath::perspective(50.0f, (float)info.windowWidth / (float)info.windowHeight, 0.1f, 1000.0f);
        vmath::mat4 mvp = proj_matrix * mv_matrix;

        glUseProgram(prog_noise);
        glUniform1f(loc_time, t * 0.0002);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    void onKey(int key, int action)
    {
        if (!action)
            return;

        switch (key)
        {
            case 'P': paused = !paused;
                break;
            case 'R':
                    load_shaders();
                break;
        }
    }

    void load_shaders()
    {
        GLuint vs;
        GLuint fs;

        if (prog_noise)
            glDeleteProgram(prog_noise);

        prog_noise = glCreateProgram();

        vs = sb6::shader::load("media/shaders/noise/noise.vs.glsl", GL_VERTEX_SHADER);
        fs = sb6::shader::load("media/shaders/noise/noise.fs.glsl", GL_FRAGMENT_SHADER);

        glAttachShader(prog_noise, vs);
        glAttachShader(prog_noise, fs);

        glLinkProgram(prog_noise);

        loc_time = glGetUniformLocation(prog_noise, "time");

        glDeleteShader(vs);
        glDeleteShader(fs);
    }

private:
    GLuint      prog_noise;
    GLuint      vao;
    int         loc_time;
    bool        paused;
};

DECLARE_MAIN(noise_app);
