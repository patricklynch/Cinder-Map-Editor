uniform mat4		modelviewMatrix;
uniform mat4		transformMatrix;
uniform mat4		projectionMatrix;

varying vec3		normal;
varying vec4		vertex;

void main()
{
	normal = vec4( transformMatrix * vec4( gl_Normal, 0 ) ).xyz;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	vertex = gl_Vertex;
	gl_Position = projectionMatrix * modelviewMatrix * transformMatrix * vertex;
}