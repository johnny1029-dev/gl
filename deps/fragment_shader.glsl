#version 330
in vec3 color;
in vec2 tex;
out vec4 fragment;

uniform sampler2D vTexture;

void main()
{
    fragment = texture(vTexture, tex);
}