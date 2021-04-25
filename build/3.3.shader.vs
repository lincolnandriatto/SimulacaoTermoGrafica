#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

out vec3 ourColor;
out vec3 Normal;
out vec3 FragPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;
void main()
{
    gl_Position = model*transform*projection * view  * vec4(aPos, 1.0f);
    ourColor = aColor;
    FragPos = vec3(model * vec4(aPos, 1.0));
   // Normal = aNormal;
   Normal = mat3(transpose(inverse(model))) * aNormal;
}
