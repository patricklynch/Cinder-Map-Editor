uniform mat4		modelViewMatrix;
uniform mat4		transformMatrix;
uniform mat4		projectionMatrix;

void main(void)
{
	gl_TexCoord[0]	= gl_MultiTexCoord0;
	gl_Position		= projectionMatrix * modelViewMatrix * transformMatrix * gl_Vertex;
}
