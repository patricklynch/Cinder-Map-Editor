uniform sampler2D	texture;
uniform sampler2D	tileMap;
uniform vec4		color;
uniform float		numTextures;
uniform vec2		offset;

#include "shaders/includes/light_ambi_diff_spec.frag"

void main()
{
	float unit = 1.0 / numTextures;
	vec2 st = gl_TexCoord[0].st + offset;
	vec4 sample = texture2D( texture, st * unit ) * color;
	gl_FragColor = _lightingAmbiDiffSpecShort( sample );
	
}

