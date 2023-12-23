#version 410 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;

void main()
{
    FragColor = vec4(1.0); // set all 4 vector values to 1.0
}