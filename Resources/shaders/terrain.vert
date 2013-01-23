uniform sampler2D	heightMap;
uniform mat4		projectionMatrix;
uniform mat4		modelviewMatrix;

varying vec3		normal;

void main()
{
	normal = gl_Normal;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	//vec4 offset = texture2D( heightMap, gl_TexCoord[0].st );
	//offset = vec4( 0, 0, 0, 0 );
	gl_Position = ftransform();// + projectionMatrix * modelviewMatrix * (gl_Vertex + offset);
}