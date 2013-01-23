uniform mat4		shadowTransMatrix;

uniform mat4		modelViewMatrix;
uniform mat4		transformMatrix;
uniform mat4		projectionMatrix;

varying vec4		worldPos, eyeCoord;
varying vec3		vNormal;
varying vec4		shadowMatrix;

void main(void)
{
	// For shadow mapping
	worldPos = transformMatrix * gl_Vertex;
	eyeCoord = modelViewMatrix * worldPos;
	shadowMatrix = shadowTransMatrix * eyeCoord;
	
	vNormal = vec4( transformMatrix * vec4( gl_Normal, 0) ).xyz;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;

	gl_Position = projectionMatrix * eyeCoord;
}
