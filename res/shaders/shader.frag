#version 410 core  

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse; 
    vec3 specular;
};
           
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
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // Ambient lighting
    vec3 ambient = light.ambient * material.ambient;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // Specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);    
    vec3 reflectDir = reflect(-lightDir, norm); 

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    // calculating resulting color
    vec3 result = ambient + diffuse + specular;

    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue) * vec4(result, 1.0f); // * vec4(ourColor, 1.0)   
    // FragColor = vec4(result, 1.0);
}