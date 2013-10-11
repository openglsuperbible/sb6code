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

static const char * vs_source[] =
{
    "#version 420 core                                                              \n"
    "                                                                               \n"
    "void main(void)                                                                \n"
    "{                                                                              \n"
    "    const vec4 vertices[] = vec4[](vec4(-1.0, -1.0, 0.5, 1.0),                 \n"
    "                                   vec4( 1.0, -1.0, 0.5, 1.0),                 \n"
    "                                   vec4(-1.0,  1.0, 0.5, 1.0),                 \n"
    "                                   vec4( 1.0,  1.0, 0.5, 1.0));                \n"
    "                                                                               \n"
    "    gl_Position = vertices[gl_VertexID];                                       \n"
    "}                                                                              \n"
};

static const char * fs_source[] =
{
    "#version 430 core                                                              \n"
    "                                                                               \n"
    "uniform sampler2D s;                                                           \n"
    "                                                                               \n"
    "uniform float exposure;\n"
    "\n"
    "out vec4 color;                                                                \n"
    "                                                                               \n"
    "void main(void)                                                                \n"
    "{                                                                              \n"
    "    vec4 c = texture(s, 2.0 * gl_FragCoord.xy / textureSize(s, 0));                  \n"
    "    c.xyz = vec3(1.0) - exp(-c.xyz * exposure);                                \n"
    "    color = c;                                                                 \n"
    "}                                                                              \n"
};

class hdrexposure_app : public sb6::application
{
public:
    hdrexposure_app()
        : exposure(1.0f)
    {

    }

    void init()
    {
        static const char title[] = "OpenGL SuperBible - HDR Exposure";

        sb6::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    void startup(void)
    {
        // Generate a name for the texture
        glGenTextures(1, &texture);

        // Load texture from file
        sb6::ktx::file::load("media/textures/treelights_2k.ktx", texture);

        // Now bind it to the context using the GL_TEXTURE_2D binding point
        glBindTexture(GL_TEXTURE_2D, texture);

        program = glCreateProgram();
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, fs_source, NULL);
        glCompileShader(fs);

        print_shader_log(fs);

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, vs_source, NULL);
        glCompileShader(vs);

        print_shader_log(vs);

        glAttachShader(program, vs);
        glAttachShader(program, fs);

        glLinkProgram(program);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
    }

    void shutdown(void)
    {
        glDeleteProgram(program);
        glDeleteVertexArrays(1, &vao);
        glDeleteTextures(1, &texture);
    }

    void render(double t)
    {
        static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, green);

        glUseProgram(program);
        glViewport(0, 0, info.windowWidth, info.windowHeight);
        glUniform1f(0, exposure);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    void onKey(int key, int action)
    {
        if (!action)
            return;

        switch (key)
        {
            case GLFW_KEY_KP_ADD:
                    exposure *= 1.1f;
                break;
            case GLFW_KEY_KP_SUBTRACT:
                    exposure /= 1.1f;
                break;
        }
    }

private:
    GLuint      texture;
    GLuint      program;
    GLuint      vao;
    float       exposure;
};

DECLARE_MAIN(hdrexposure_app);
