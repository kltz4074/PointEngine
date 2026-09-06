#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;

    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;

    vec4 viewPos = view * worldPos;

    viewPos = view * worldPos;
    vec4 pos = projection * viewPos;

    vec3 ndc = pos.xyz / pos.w;

    float resolution = 90.0;

    ndc.xy = round(ndc.xy * resolution) / resolution;

    gl_Position = vec4(ndc * pos.w, pos.w);
}
