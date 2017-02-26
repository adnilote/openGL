#version 330

layout(location = 0) in vec3 point3;
layout(location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 camera;
void main() {

	mat4 scaleMatrix = mat4(1.0);
	scaleMatrix[0][0] = 1.5;
	scaleMatrix[1][1] = 1;
	scaleMatrix[2][2] = 1.5;

	mat4 move = mat4(1.0);
	move[0][3] = 15;

	vec4 point = vec4(point3, 1.0);
	//vec3 movesky = camera.position;
 
   	gl_Position =  camera* ( scaleMatrix*point- vec4(0,1.01,0,0) );
   	TexCoord = texCoord;
}
