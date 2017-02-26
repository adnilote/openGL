#version 330

in vec2 TexCoord;

out vec4 outColor;

uniform sampler2D GrassTex;
uniform sampler2D DirtTex;

void main() {
	vec4 d= texture(DirtTex, TexCoord);
	vec4 g= texture(GrassTex, TexCoord);

    outColor = mix(g,d, 0.5);
}