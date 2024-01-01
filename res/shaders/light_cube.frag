#version 410 core

#if __VERSION__ >= 430
    #version 430 core
#endif

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0); // set all 4 vector values to 1.0
}