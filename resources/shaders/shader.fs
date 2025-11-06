#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture_diffuse;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 lightColor = vec3(1.0);
    vec3 objectColor = texture(texture_diffuse, TexCoord).rgb;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = 0.5 * spec * lightColor;

    vec3 result = (diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
