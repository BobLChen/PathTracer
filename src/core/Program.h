#pragma once

#include "Shader.h"

#include <vector>

namespace GLSLPT
{
    class Program
    {
    public:
        Program(const std::vector<Shader>& shaders);
        ~Program();

        void Active();
        void Deactive();
        GLuint Object();

	private:
		GLuint m_Object;
    };
}
