#version 330

in vec2 TexCoord;

out vec4 outColor;

uniform sampler2D cloverTexture;

void main() {	
    outColor =texture(cloverTexture, TexCoord); // vec4(1,0,0,0);// //
}
