#version 330

layout(location = 0) in vec4 point4;

out vec3 TexCoord;

uniform mat4 camera;
//uniform samplerBuffer skymove;

void main() {

   // vec3 movesky = texelFetch(skymove, gl_InstanceID).xyz;

	mat4 scaleMatrix = mat4(1.0);
	scaleMatrix[0][0] = 15;
	scaleMatrix[1][1] = 15;
	scaleMatrix[2][2] = 15;

	//mat4 move = mat4(1.0);
	//move[0][3] = 15;

	//vec3 movesky = camera.position;
 
   	gl_Position =  camera* (scaleMatrix * point4 +
vec4(0,0.5,0,0) );
//+vec4(movesky.x, movesky.y, movesky.z, 0) );
	TexCoord = point4.xyz;
}