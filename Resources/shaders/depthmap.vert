uniform mat4 transformMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

void main()
{
	gl_Position = projectionMatrix * modelViewMatrix * transformMatrix * vec4( gl_Vertex );
}
