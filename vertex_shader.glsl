#version 330
in vec3 vCol;
in vec3 vPos;
in vec2 vTex;
out vec3 color;
out vec2 tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(vPos, 1.0);
    color = vCol;
    tex = vTex;
}