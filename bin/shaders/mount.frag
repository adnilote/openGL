#version 330

in vec2 TexCoord;

out vec4 outColor;

uniform sampler2D mountTexture;

void main() {
    outColor =texture(mountTexture,TexCoord); 
// vec4(0,1,0,0);
}
