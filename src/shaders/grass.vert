#version 330

/*layout(location=0)*/ in vec4 point;
/*layout(location=1)*/ //in vec2 UV;

//in double rand;
out vec2 TexCoord;

//in vec2 position;
//in vec4 variance;

uniform mat4 camera;
uniform samplerBuffer tboSampler;
uniform samplerBuffer tboSampler2;


void main() {
    
float h = 0.1+0.02 * sin( pow(gl_InstanceID, 2));
  //  vec2 bladePos = position;
    vec2 bladePos = texelFetch(tboSampler, gl_InstanceID).xy;
    vec4 var      = texelFetch(tboSampler2, gl_InstanceID);

    mat4 scaleMatrix = mat4(1.0);
    scaleMatrix[0][0] = 0.01;
    scaleMatrix[1][1] = h*(0.6f-var.y*5.0);
	scaleMatrix[2][2] = h*0.3f;

    mat4 positionMatrix = mat4(1.0);
    positionMatrix[3][0] = bladePos.x;
    positionMatrix[3][2] = bladePos.y;
	
float rot = cos(gl_InstanceID*gl_InstanceID)*3.14/4-3.14;
mat4 rotateMatrix = mat4(1.0);
rotateMatrix[1][1]=rotateMatrix[3][3]=1;
rotateMatrix[0][0]=cos(rot);	
rotateMatrix[2][2]=cos(rot);
rotateMatrix[0][1]=sin(rot);
rotateMatrix[2][0]=-sin(rot);
    

	gl_Position = camera * (positionMatrix * rotateMatrix * scaleMatrix   * point + var *h  * point.y* point.y* point.y);

	TexCoord = point.xy;
}

