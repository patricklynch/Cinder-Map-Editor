uniform sampler2D	texture;
uniform sampler2D	tileMap;
uniform vec4		color;
uniform float		numTextures;
uniform vec2		offset;

varying vec4		vertex;

#include "shaders/includes/light_ambi_diff_spec.frag"
#include "shaders/includes/discard_outside_bounds.frag"

const vec3 clipMin = vec3( -1, -1, -1 );
const vec3 clipMax = vec3(  1,  1,  1 );

void main()
{
	_discardOutsideBounds( vertex.xyz, clipMin, clipMax );
	
	float unit = 1.0 / numTextures;
	vec2 st = gl_TexCoord[0].st + offset;
	vec4 sample = texture2D( texture, st * unit ) * color;
	gl_FragColor = _lightingAmbiDiffSpecShort( sample );
	
}

