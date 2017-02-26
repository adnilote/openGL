#version 330

layout(location = 0) in vec4 point;
//layout(location = 1) in vec2 color;
layout(location = 1) in vec2 texCoord;

out vec2 TexCoord;
//in vec4 point;

uniform mat4 camera;

void main() {
    gl_Position = camera * point;
    TexCoord = texCoord;
}
