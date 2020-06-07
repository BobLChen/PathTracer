#include "GfxTexture.h"

GfxTexture::GfxTexture(GLuint target, GLint internalformat, GLenum format, GLenum type, int width, int height, int depth, void* data)
    : m_Object(0)
    , m_Target(target)
    , m_InternalFormat(internalformat)
    , m_Format(format)
    , m_Type(type)
    , m_Width(width)
    , m_Height(height)
    , m_Depth(depth)
{
    glGenTextures(1, &m_Object);
    glBindTexture(m_Target, m_Object);
    
    if (m_Target == GL_TEXTURE_2D)
    {
        glTexImage2D(m_Target, 0, m_InternalFormat, m_Width, m_Height, 0, m_Format, m_Type, data);
    }
    else if (m_Target == GL_TEXTURE_2D_ARRAY)
    {
        glTexImage3D(m_Target, 0, m_InternalFormat, m_Width, m_Height, m_Depth, 0, m_Format, m_Type, data);
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glBindTexture(m_Target, 0);
}

GfxTexture::~GfxTexture()
{
    if (m_Object != 0)
    {
        glDeleteTextures(1, &m_Object);
        m_Object = 0;
    }
}

void GfxTexture::Filter(GLuint minFilter, GLuint magFilter)
{
    glBindTexture(m_Target, m_Object);
    glTexParameteri(m_Target, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(m_Target, GL_TEXTURE_MIN_FILTER, minFilter);
    glBindTexture(m_Target, 0);
}

void GfxTexture::Wrap(GLuint s, GLuint t, GLuint r)
{
    glBindTexture(m_Target, m_Object);
    glTexParameteri(m_Target, GL_TEXTURE_WRAP_S, s);
    glTexParameteri(m_Target, GL_TEXTURE_WRAP_T, t);
    glTexParameteri(m_Target, GL_TEXTURE_WRAP_R, r);
    glBindTexture(m_Target, 0);
}

void GfxTexture::SubImage2D(GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, const void* data)
{
    glBindTexture(m_Target, m_Object);
    glTexSubImage2D(m_Target, level, xoffset, yoffset, width, height, m_Format, m_Type, data);
    glBindTexture(m_Target, 0);
}
