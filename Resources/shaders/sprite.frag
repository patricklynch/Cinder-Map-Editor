uniform sampler2D	mainTexture;
uniform sampler2D	normalTexture;
uniform mat4		transformMatrix;

uniform vec4		diffuseColor;
uniform vec4		ambientColor;
uniform vec4		specularColor;

uniform float		normalFactor;

uniform vec3		lightPos;
uniform vec3		eyePos;

varying vec3		worldPos;

void main()
{
	vec3 viewDir		= eyePos - worldPos;
	vec3 lightDir		= lightPos - worldPos;
	vec3 normal			= texture2D( normalTexture, gl_TexCoord[0].st ).xyz;
	normal				= normal * 2.0 - 1.0;
	normal				*= normalFactor;
	normal				= vec4( transformMatrix * vec4( normal, 0 ) ).xyz;
	
    vec3 N				= normalize( normal );
    vec3 L				= normalize( lightDir );
    vec3 E				= normalize( viewDir );
    vec3 H				= normalize( L + E );
	
	float df			= max( 0.0, dot(N, L) );
	float sf			= pow( max( 0.0, dot(N, H) ), 80.0 );
	
	
	vec4 textureColor	= texture2D( mainTexture, gl_TexCoord[0].st ) * diffuseColor;
	
	vec4 diffuse		= vec4( textureColor.xyz * df, textureColor.a );
	vec4 specular		= vec4( specularColor.xyz * sf, textureColor.a );
	vec4 ambient		= vec4( ambientColor.xyz, textureColor.a );
	
	gl_FragColor		= ambient * diffuseColor + diffuse;
	
	
}
