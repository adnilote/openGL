#version 330

//layout(location = 0) in vec3 point3;
layout(location = 0) in vec4 point;
layout(location = 1) in vec2 texCoord;

out vec2 TexCoord;
//in vec4 point;

uniform mat4 camera;

void main() {
	mat4 scaleMatrix = mat4(1.0);
	scaleMatrix[0][0] = 0.5;
	scaleMatrix[1][1] = 0.1;
	scaleMatrix[2][2] = 0.5;

	mat4 move = mat4(1.0);
//	move[2][3] = 15;
//	move[1][3] = 15;
	move[0][3] = 15;

//	vec4 point = vec4(point3, 1.0);
 
   	gl_Position =  camera *(scaleMatrix* point+vec4(0.1,0,0.05,0));
   	TexCoord = texCoord;
}
