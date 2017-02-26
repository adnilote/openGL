#version 330

in vec2 TexCoord;

out vec4 outColor;

uniform sampler2D Texture;

void main() {
    outColor =texture(Texture,TexCoord); 
// vec4(0,1,0,0);
}




