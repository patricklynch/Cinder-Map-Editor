varying vec3 vVertex;
uniform float cameraDepth;

void main()
{
	float depth = vVertex.z / cameraDepth;
	gl_FragColor = vec4( depth, depth, depth, 1);
}