#version 330
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 proj;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = proj * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
