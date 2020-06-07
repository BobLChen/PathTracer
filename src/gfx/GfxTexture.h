#pragma once

#include <string>

#include "glad/glad.h"

class GfxTexture
{
public:
    GfxTexture(GLuint target, GLint internalformat, GLenum format, GLenum type, int width, int height, int depth, void* data = nullptr);
    
    virtual ~GfxTexture();
    
    inline GLuint GetTexture()
    {
        return m_Object;
    }
    
    inline GLuint GetTarget()
    {
        return m_Target;
    }
    
    inline GLint GetInternalFormat()
    {
        return m_InternalFormat;
    }
    
    inline GLenum GetFormat()
    {
        return m_Format;
    }
    
    inline GLenum GetType()
    {
        return m_Type;
    }
    
    inline int GetWidth()
    {
        return m_Width;
    }
    
    inline int GetHeight()
    {
        return m_Height;
    }
    
    inline int GetDepth()
    {
        return m_Depth;
    }
    
    inline void Active()
    {
        glBindTexture(m_Target, m_Object);
    }
    
    inline void Deactive()
    {
        glBindTexture(m_Target, 0);
    }
    
    void SubImage2D(GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, const void* data);
    
    void Filter(GLuint minFilter, GLuint magFilter);
    
    void Wrap(GLuint s, GLuint t, GLuint r);
    
private:
    
    GLuint  m_Object;
    GLuint  m_Target;
    GLint   m_InternalFormat;
    GLenum  m_Format;
    GLenum  m_Type;
    
    int     m_Width;
    int     m_Height;
    int     m_Depth;
};

