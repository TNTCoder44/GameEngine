#pragma once

class FrameBuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_ColorAttachment;
    unsigned int m_DepthAttachment; // not used right now
    unsigned int m_Width, m_Height;
    unsigned int m_rbo;
    unsigned int m_textureColorbuffer;

    void CreateTexture();
    void CreateRenderbuffer();

public:
    FrameBuffer(const unsigned int&, const unsigned int&, const int&);
    ~FrameBuffer();

    void Bind() const;
    void Unbind() const;
    void Delete() const;

    void BindTexture() const;
};

