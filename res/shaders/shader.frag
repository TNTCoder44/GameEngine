#version 410 core  
           
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;

// lightning
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue) * vec4(ourColor, 1.0f);    
    FragColor = vec4(lightColor * objectColor, 1.0f);
}