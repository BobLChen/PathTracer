#pragma once

#include <string>

#include "glad/glad.h"

namespace GLSLPT
{
    class Shader
    {
    public:
        Shader(const std::string& filePath, GLuint shaderType);
        GLuint Object() const;
	private:
		GLuint m_Object;
    };
}