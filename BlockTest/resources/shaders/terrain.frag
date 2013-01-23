uniform sampler2D	texture;
uniform sampler2D	tileMap;

uniform vec4		color;
varying vec3		normal;
uniform float		numTiles;
uniform float		numTextures;

void main()
{
	float unit = numTiles / numTextures;
	vec2 st = gl_TexCoord[0].st;
	vec2 offset = st * unit;
	
	vec2 loc = texture2D( tileMap, gl_TexCoord[0].st ).xy * numTextures;
	loc = vec2( floor(loc.x), floor(loc.y) );
	
	offset.x -= unit * ( floor( st.x * numTiles ) - loc.x );
	offset.y -= unit * ( floor( st.y * numTiles ) - loc.y );
	offset.x = fract( offset.x );
	offset.y = fract( offset.y );
	
	st = st + offset;
	vec4 sample = texture2D( texture, offset ) * color;
	gl_FragColor = sample;
	
}

