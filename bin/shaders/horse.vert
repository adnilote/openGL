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

	mat4 move = mat4(1.0);
	move[2][3] = 1.5;
	move[1][3] = 1.5;
	move[0][3] = 1.5;
mat4 rotateMatrix = mat4(1.0);
rotateMatrix[0][0]=cos(2.35);	
rotateMatrix[2][2]=cos(2.35);
rotateMatrix[0][2]=-sin(2.35);
rotateMatrix[2][0]=sin(2.35);
    


	vec4 point = vec4(point3, 1.0);
 
   	gl_Position =  camera *(rotateMatrix * scaleMatrix * point+vec4(0.2,0,0.5,0));
   	TexCoord = texCoord;
}


