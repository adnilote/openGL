#version 330

layout(location = 0) in vec3 point3;
layout(location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 camera;
void main() {

	mat4 scaleMatrix = mat4(1.0);
	scaleMatrix[0][0] = 0.003;
	scaleMatrix[1][1] = 0.0035;
	scaleMatrix[2][2] = 0.004;

mat4 rotateMatrix = mat4(1.0);
rotateMatrix[0][0]=cos(2);	
rotateMatrix[2][2]=cos(2);
rotateMatrix[0][2]=-sin(2);
rotateMatrix[2][0]=sin(2);
    

	vec4 point = vec4(point3, 1.0);
	
   	gl_Position =  camera* (rotateMatrix* scaleMatrix*point+ vec4(0.3,0,0.059,0) );
   	TexCoord = texCoord;
}


