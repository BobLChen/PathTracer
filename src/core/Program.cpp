#include <stdexcept>

#include "Program.h"

namespace GLSLPT
{
    Program::Program(const std::vector<Shader>& shaders)
    {
        m_Object = glCreateProgram();
		for (unsigned i = 0; i < shaders.size(); i++) {
			glAttachShader(m_Object, shaders[i].Object());
		}
        
        glLinkProgram(m_Object);
		printf("Linking program %d\n", int(m_Object));
		for (unsigned i = 0; i < shaders.size(); i++) {
			glDetachShader(m_Object, shaders[i].Object());
		}
        
        GLint success = 0;
        glGetProgramiv(m_Object, GL_LINK_STATUS, &success);
        if (success == GL_FALSE)
        {
            std::string msg("Error while linking program\n");
            GLint logSize = 0;
            glGetProgramiv(m_Object, GL_INFO_LOG_LENGTH, &logSize);
            char* info = new char[logSize + 1];
            glGetShaderInfoLog(m_Object, logSize, NULL, info);
            msg += info;
            delete[] info;
            glDeleteProgram(m_Object);
            m_Object = 0;
            throw std::runtime_error(msg);
        }
    }

    Program::~Program()
    {
        glDeleteProgram(m_Object);
    }

    void Program::Active()
    {
        glUseProgram(m_Object);
    }

    void Program::Deactive()
    {
        glUseProgram(0);
    }

    GLuint Program::Object()
    {
        return m_Object;
    }
}