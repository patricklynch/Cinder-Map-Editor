uniform mat4	projectionMatrix;
uniform mat4	modelviewMatrix;
uniform mat4	transformMatrix;

varying vec3	normal;
varying vec4	vertex;

void main()
{
	normal = gl_Normal;
	vertex = gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;
	gl_Position = projectionMatrix * modelviewMatrix * transformMatrix * gl_Vertex;
}