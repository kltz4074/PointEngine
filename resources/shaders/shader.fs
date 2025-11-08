#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture_diffuse;

uniform vec3 lightPos;
uniform vec3 viewPos;

// Новые юниформы для цветов и силы
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);   // белый свет
uniform float lightIntensity = 1.0;               // light intensity
uniform float ambientStrength = 0.2;             // 20% ambient (настраивай под себя)


void main()
{
    // --- Ambient ---
    vec3 ambient = ambientStrength * lightColor;

    // --- Diffuse ---
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // --- Specular ---
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = 0.5 * spec * lightColor;  // 0.5 — сила блика

    // --- Итоговый цвет ---
    vec3 objectColor = texture(texture_diffuse, TexCoord).rgb;

    // применяем яркость
    vec3 result = lightIntensity * (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0);
}