#include <iostream>
#include <fstream>
#include <sstream>

#include "Shader.h"
#include "ShaderIncludes.h"

namespace GLSLPT
{
    Shader::Shader(const std::string& filePath, GLuint shaderType)
    {
		std::string source = GLSLPT::ShaderInclude::Load(filePath);

        m_Object = glCreateShader(shaderType);
		printf("Compiling Shader %s -> %d\n", filePath.c_str(), int(m_Object));

        const GLchar *src = (const GLchar *)source.c_str();
        glShaderSource(m_Object, 1, &src, 0);
        glCompileShader(m_Object);

        GLint success = 0;
        glGetShaderiv(m_Object, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE)
        {
			std::string msg;
            GLint logSize = 0;
            glGetShaderiv(m_Object, GL_INFO_LOG_LENGTH, &logSize);
            char *info = new char[logSize + 1];
            glGetShaderInfoLog(m_Object, logSize, NULL, info);
			msg += filePath;
			msg += "\n";
            msg += info;
            delete[] info;
            glDeleteShader(m_Object);
			m_Object = 0;
			printf("Shader compilation error %s\n", msg.c_str());
            throw std::runtime_error(msg);
        }
    }
	
    GLuint Shader::Object() const
    {
        return m_Object;
    }
}