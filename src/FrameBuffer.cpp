#include "FrameBuffer.h"
#include <iostream>
#include <glad/glad.h>

FrameBuffer::FrameBuffer(const unsigned int& width, const unsigned int& height, const int& colorAttachment)
{
    glGenFramebuffers(1, &m_RendererID);    
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    
    m_Width = width;
    m_Height = height;
    m_ColorAttachment = colorAttachment;
    m_rbo = 0;
    m_textureColorbuffer = 0;

    CreateTexture();
    CreateRenderbuffer();
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::CreateTexture()
{
    glGenTextures(1, &m_textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, m_ColorAttachment, GL_TEXTURE_2D, m_textureColorbuffer, 0);
}

void FrameBuffer::CreateRenderbuffer()
{
    unsigned int m_rbo;
    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER Framebuffer is not complete!" << std::endl;
    }

    Unbind();
}

void FrameBuffer::Bind() const
{
    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
}


void FrameBuffer::Unbind() const
{
    glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Delete() const
{
    glDeleteFramebuffers(1, &m_RendererID);
    glDeleteRenderbuffers(1, &m_rbo);
}

void FrameBuffer::BindTexture() const
{
    glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer);
}

