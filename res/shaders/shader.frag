#version 410 core  

#if __VERSION__ >= 430
    #version 430 core
#endif

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse; 
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
           
out vec4 FragColor;

// lighting
in vec3 Normal;
in vec3 FragPos;

// Texture
in vec3 ourColor;
in vec2 TexCoord;

// texture
uniform float mixValue;

// lighting & texture
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
  	
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuseMap = vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * diff * diffuseMap;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specularMap = vec3(texture(material.specular, TexCoord));
    vec3 specular = light.specular * spec * specularMap;

    // emission
    vec2 myTexCoords = TexCoord;
    vec3 emissionMap = vec3(texture(material.emission, myTexCoords));
    vec3 emission = emissionMap;

    // emission mask
    // vec3 emissionMask = step(vec3(1.0f), vec3(1.0f) - specularMap);
    // emission = emission * emissionMask;

    // spotlight
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    diffuse *= intensity;
    specular *= intensity;

    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
    	        light.quadratic * (distance * distance));

    // calculating resulting color vector
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation; 

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f); // * vec4(ourColor, 1.0);
}