/**
 Pixel Lighting with diffuse, ambient and specular components
 */

uniform vec4	lightColor;
uniform vec4	ambientColor;
uniform vec4	specularColor;
uniform vec3	eyePos;
uniform vec3	lightPos;
uniform float	shininess;

varying vec3	normal;
varying vec4	worldPos;

// For other uses, call the fully parameterized function
vec4 _lightingAmbiDiffSpec(in vec4 _diffuseColor,
						   in vec4 _specularColor,
						   in vec4 _ambientColor,
						   in float _shininess,
						   in vec3 _lightPos,
						   in vec4 _lightColor,
						   in vec3 _eyePos,
						   in vec4 _vertex,
						   in vec3 _normal )
{
    vec3 N = normalize( _normal );
    vec3 L = normalize( _lightPos - _vertex.xyz );
    vec3 E = normalize( _eyePos - _vertex.xyz );
    vec3 H = normalize( L + E );
	float df = max( 0.0, dot( N, L ) );
	float sf = pow( max( 0.0, dot( N, H ) ), _shininess );
	return _ambientColor * _diffuseColor + _diffuseColor * df + _specularColor * sf;
}

// By defining the uniforms and varyings above, you can simply call this
vec4 _lightingAmbiDiffSpecShort( in vec4 _diffuseColor )
{
    return _lightingAmbiDiffSpec(_diffuseColor,
								 specularColor,
								 ambientColor,
								 shininess,
								 lightPos,
								 lightColor,
								 eyePos,
								 worldPos,
								 normal );
}