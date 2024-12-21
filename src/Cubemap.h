#pragma once

#include <vector>
#include <string>

class Cubemap 
{
private:
    unsigned int m_RendererID;
    unsigned int m_textureID;
    int width, height, nrChannels;
    std::vector<std::string> m_faces;

public:
    Cubemap(const std::vector<std::string>& faces);
    ~Cubemap();

    void Bind();
    void Unbind();
    void Delete();

private:
    unsigned int loadCubemap();

};
