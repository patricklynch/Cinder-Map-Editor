uniform mat4		projectionMatrix;
uniform mat4		modelviewMatrix;
uniform mat4		transformMatrix;

varying vec3		normal;

void main()
{
	normal = gl_Normal;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = projectionMatrix * modelviewMatrix * transformMatrix * gl_Vertex;
}