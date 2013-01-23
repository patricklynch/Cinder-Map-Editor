uniform sampler2D	texture;
uniform sampler2D	tileMap;
uniform vec4		color;
uniform float		numTextures;
uniform vec2		offset;



uniform vec3	eyePos;
uniform vec3	lightPos;
uniform vec4	ambientColor;
uniform vec4	specularColor;
uniform float	shininess;
varying vec3	normal;
varying vec4	vertex;

vec4 _lightingAmbiSpecDiff( in vec4 _diffuseColor,
							in vec4 _specularColor,
							in vec4 _ambientColor,
							in float _shininess,
							in vec3 _lightPos,
							in vec4 _lightColor,
							in vec3 _eyePos,
							in vec3 _vertex,
							in vec3 _normal ) {
    vec3 N = normalize( _normal );
    vec3 L = normalize( _lightPos - _vertex );
    vec3 E = normalize( _eyePos - _vertex );
    vec3 H = normalize( L + E );
	float df = max( 0.0, dot( N, L ) );
	float sf = pow( max( 0.0, dot( N, H ) ), 30.0 );
	return _ambientColor * _diffuseColor + _diffuseColor * df + _specularColor * sf;
}



void main()
{
	float unit = 1.0 / numTextures;
	vec2 st = gl_TexCoord[0].st + offset;
	vec4 sample = texture2D( texture, st * unit ) * color;
	gl_FragColor = sample;
	
}

