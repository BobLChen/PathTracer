#pragma once

#include "glad/glad.h"

namespace GLSLPT
{
    class Program;

    class Quad
    {
    public:
        Quad();

        void Draw(Program* program);
    private:
        GLuint vao;
		GLuint vbo;
    };
}