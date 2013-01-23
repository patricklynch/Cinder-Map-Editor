uniform mat4		modelViewMatrix;
uniform mat4		transformMatrix;
uniform mat4		projectionMatrix;

varying vec3		worldPos;
varying vec3		normal;

void main(void)
{
	gl_TexCoord[0]	= gl_MultiTexCoord0;
	gl_TexCoord[1]	= gl_MultiTexCoord1;
	gl_TexCoord[2]	= gl_MultiTexCoord2;
	gl_TexCoord[3]	= gl_MultiTexCoord3;
	
	vec4 pos		= transformMatrix * gl_Vertex;
	gl_Position		= projectionMatrix * modelViewMatrix * pos;
	worldPos		= pos.xyz;
}
