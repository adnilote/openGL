#version 330

in vec3 TexCoord;

out vec4 outColor;

uniform samplerCube skyTexture;

void main() {
    outColor =texture(skyTexture,TexCoord); 
// vec4(0,1,0,0);
}

