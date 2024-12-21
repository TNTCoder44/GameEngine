#include "Cubemap.h"

#include <iostream>

#include <glad/glad.h>
#include <stb/stb_image.h>

Cubemap::Cubemap(const std::vector<std::string>& faces)
{
    m_faces = faces;

    m_RendererID = loadCubemap();
}

Cubemap::~Cubemap()
{
}

void Cubemap::Bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
}

void Cubemap::Unbind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::Delete()
{
    glDeleteTextures(1, &m_RendererID);
}

unsigned int Cubemap::loadCubemap()
{
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < m_faces.size(); i++)
    {
        unsigned char *data = stbi_load(m_faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << m_faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return m_textureID;
}
