uniform sampler2D	heightMap;

uniform mat4		modelviewMatrix;
uniform mat4		transformMatrix;
uniform mat4		projectionMatrix;

varying vec3		normal;
varying vec4		vertex;

void main()
{
	normal = vec4( transformMatrix * vec4( gl_Normal, 0 ) ).xyz;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	//vec4 offset = texture2D( heightMap, gl_TexCoord[0].st );
	//vertex = gl_Vertex + vec4( 0.0, offset.y, 0.0, 1.0 );
	vertex = gl_Vertex;
	gl_Position = projectionMatrix * modelviewMatrix * transformMatrix * vertex;
}