#version 410 core  
           
out vec4 FragColor;

// lighting
in vec3 Normal;

in vec3 FragPos;

// Texture
in vec3 ourColor;
in vec2 TexCoord;

// texture
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;

// lighting
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;

void main()
{
    // Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting


    // calculating resulting color
    vec3 result = (ambient + diffuse) * objectColor;

    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue) * vec4(ourColor, 1.0f);    
    FragColor = vec4(result, 1.0f);
}