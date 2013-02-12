uniform sampler2D	textureD;
uniform sampler2D	textureR;
uniform sampler2D	textureG;
uniform sampler2D	textureB;
uniform sampler2D	textureEdge;
uniform sampler2D	textureMask;
uniform sampler2D	texturePaintMask;
uniform mat4		textureMatrix;
//uniform mat4		textureMatrix2;

uniform float		numTextures;
uniform vec2		offset;

uniform vec4		color;
varying vec4		vertex;

uniform bool		edge;

#include "shaders/includes/light_ambi_diff_spec.frag"

void main()
{
	float unit					= 1.0 / numTextures;
	
	if ( edge ) {
		vec2 st					= gl_TexCoord[0].st;
		st.y					= 1.0 - st.y;
		vec4 texColorEdge		= texture2D( textureEdge, st );
		vec4 mask				= texture2D( textureMask, st );
		vec4 finalColor			= mix( texColorEdge, vec4(0,0,0,0), mask.r );
		gl_FragColor			= _lightingAmbiDiffSpecShort( finalColor );
	}
	else {
		vec2 st					= gl_TexCoord[0].st;
		st						= vec2( textureMatrix * vec4( st.x, st.y, 0, 1 ) );
		vec4 texColorD			= texture2D( textureD, st );
		vec4 texColorR			= texture2D( textureR, st );
		vec4 texColorG			= texture2D( textureG, st );
		vec4 texColorB			= texture2D( textureB, st );

		st						= gl_TexCoord[0].st;
		st						= vec2( textureMatrix * vec4( st.x, st.y, 0, 1 ) );
		st						= vec2( st.x, 1.0-st.y );
		st						+= offset;
		st						= st * unit;
		vec4 paintMask			= texture2D( texturePaintMask, st );
		
		vec4 finalColor			= texColorD;
		
		finalColor				= mix( finalColor, texColorR, paintMask.r );
		finalColor				= mix( finalColor, texColorG, paintMask.g );
		finalColor				= mix( finalColor, texColorB, paintMask.b );

		gl_FragColor			= _lightingAmbiDiffSpecShort( finalColor );
	}
}