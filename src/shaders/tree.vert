#version 330

layout(location = 0) in vec3 point3;
layout(location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 camera;
void main() {

	mat4 scaleMatrix = mat4(1.0);
	scaleMatrix[0][0] = 0.005;
	scaleMatrix[1][1] = 0.005;
	scaleMatrix[2][2] = 0.005;

	vec4 point = vec4(point3, 1.0);
	
   	gl_Position =  camera* ( scaleMatrix*point+ vec4(0.016,0,0.059,0) );
   	TexCoord = texCoord;
}

