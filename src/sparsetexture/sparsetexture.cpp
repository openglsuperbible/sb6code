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
#include <vmath.h>

class sparsetexture_app : public sb6::application
{
public:
    sparsetexture_app()
        : paused(false)
    {

    }

    void startup();
    void render(double currentTime);
    void shutdown();

protected:
    void load_shaders();

    PFNGLTEXSTORAGESPARSEAMDPROC    glTexStorageSparseAMD;

    GLuint      texture;
    GLuint      program;
    GLuint      vao;

    struct
    {
        GLint   mv_matrix;
        GLint   vp_matrix;
    } uniforms;

    bool        paused;

    unsigned char * texture_data;

    static const unsigned char bit_reversal_table[256];
};

const unsigned char sparsetexture_app::bit_reversal_table[256] =
{
    0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
    0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
    0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
    0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
    0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
    0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
    0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
    0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
    0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
    0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
    0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
    0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
    0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
    0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
    0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
    0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
    0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
    0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
    0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
    0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
    0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
    0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
    0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
    0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
    0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
    0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
    0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
    0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
    0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
    0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
    0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
    0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff
};

void sparsetexture_app::startup()
{
    glTexStorageSparseAMD = (PFNGLTEXSTORAGESPARSEAMDPROC)sb6GetProcAddress("glTexStorageSparseAMD");

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexStorageSparseAMD(GL_TEXTURE_2D, GL_RGBA8, 8192, 8192, 1, 1, GL_TEXTURE_STORAGE_SPARSE_BIT_AMD);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenVertexArrays(1, &vao);

    load_shaders();

    texture_data = new unsigned char [512 * 512 * 4];

    int x, y;

    for (y = 0; y < 512; y++)
    {
        for (x = 0; x < 512; x++)
        {
            texture_data[y * 2048 + x * 4] = y ^ x;
            texture_data[y * 2048 + x * 4 + 1] = (y >> 1) ^ (x << 1);
            texture_data[y * 2048 + x * 4 + 2] = (y >> 2) ^ (x << 2);
            texture_data[y * 2048 + x * 4 + 3] = 0;
        }
    }
}

void sparsetexture_app::render(double currentTime)
{
    static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    static double last_time = 0.0;
    static double total_time = 0.0;

    if (!paused)
        total_time += (currentTime - last_time);
    last_time = currentTime;

    const float f = (float)total_time;

    unsigned int t = (unsigned int)(f * 45.67f);

    vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -2.0f) *
                            vmath::rotate(f * 40.0f, 0.0f, 1.0f, 0.0f);

    vmath::mat4 vp_matrix = vmath::perspective(50.0f,
                                               (float)info.windowWidth / (float)info.windowHeight,
                                               0.1f, 1000.0f);

    glClearBufferfv(GL_COLOR, 0, black);

    glBindVertexArray(vao);
    glUseProgram(program);

    glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, mv_matrix);
    glUniformMatrix4fv(uniforms.vp_matrix, 1, GL_FALSE, vp_matrix);

    glBindTexture(GL_TEXTURE_2D, texture);

    int r = (t & 0x100);
    t = bit_reversal_table[(t & 0xFF) ^ 173];

    int x = t & 0xF;
    int y = (t >> 4) & 0xF;

    x ^= 0xA;
    y ^= 0xA;

    if (r)
    {
        glTexSubImage2D(GL_TEXTURE_2D, 0, x * 512, y * 512, 512, 512, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }
    else
    {
        glTexSubImage2D(GL_TEXTURE_2D, 0, x * 512, y * 512, 512, 512, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
    }

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void sparsetexture_app::shutdown()
{
    delete [] texture_data;

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);
}

void sparsetexture_app::load_shaders()
{
    GLuint shaders[2];

    shaders[0] = sb6::shader::load("media/shaders/sparsetexture/render.vs.glsl", GL_VERTEX_SHADER);
    shaders[1] = sb6::shader::load("media/shaders/sparsetexture/render.fs.glsl", GL_FRAGMENT_SHADER);

    program = sb6::program::link_from_shaders(shaders, 2, true);

    uniforms.mv_matrix = glGetUniformLocation(program, "mv_matrix");
    uniforms.vp_matrix = glGetUniformLocation(program, "vp_matrix");
}

DECLARE_MAIN(sparsetexture_app)
