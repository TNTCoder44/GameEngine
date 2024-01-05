#version 330 core

out vec4 FragColor;

struct Light 
{
	vec3 position;
	vec3 color;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Light light;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
	// Ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * texture(texture_diffuse1, TexCoords).xyz;

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light.color * texture(texture_diffuse1, TexCoords).xyz;

	// Specular
	float specularStrength = 1.0;
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 viewDir = normalize(viewPos - FragPos);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * texture(texture_specular1, TexCoords).xyz;

	// calculate color
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}
