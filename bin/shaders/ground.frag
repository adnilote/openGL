#version 330

in vec2 TexCoord;

out vec4 outColor;

uniform sampler2D myTexture;

void main() {
    outColor =texture(myTexture,TexCoord); // vec4(0,1,0,0);//
}
