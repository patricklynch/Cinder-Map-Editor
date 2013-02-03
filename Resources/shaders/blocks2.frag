uniform sampler2D	texture1;
uniform sampler2D	texture2;
uniform sampler2D	textureMask;

uniform vec4		color;
uniform bool		invertMask;
varying vec4		vertex;

uniform vec2		testOffset;

#include "shaders/includes/light_ambi_diff_spec.frag"

void main()
{
	vec4 texColor1 = texture2D( texture1, gl_TexCoord[0].st );
	vec4 texColor2 = texture2D( texture2, gl_TexCoord[0].st );
	vec4 mask = texture2D( textureMask, gl_TexCoord[0].st );
	
	vec4 finalTexture = mix( texColor1, texColor2, mask.r );
	
	gl_FragColor = _lightingAmbiDiffSpecShort( finalTexture );	
}