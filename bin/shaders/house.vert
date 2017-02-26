#version 330

layout(location = 0) in vec3 point3;
layout(location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 camera;
void main() {

	mat4 scaleMatrix = mat4(1.0);
	scaleMatrix[0][0] = 0.4;
	scaleMatrix[1][1] = 0.4;
	scaleMatrix[2][2] = 0.4;
mat4 rotateMatrix = mat4(1.0);
rotateMatrix[0][0]=cos(3.0);	
rotateMatrix[2][2]=cos(3.0);
rotateMatrix[2][0]=sin(3.0);
rotateMatrix[0][2]=-sin(3.0);
    
	vec4 point = vec4(point3, 1.0);
	
   	gl_Position =  camera* ( rotateMatrix * scaleMatrix*point+ vec4(0.85,0.09,0.6,0) );
   	TexCoord = texCoord;
}


