#version 330 core
out vec4 FragColor;

in vec3 someColor;

void main()
{
    FragColor = vec4(someColor, 1.0f);
}