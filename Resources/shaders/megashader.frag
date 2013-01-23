uniform sampler2DShadow depthTexture;

uniform sampler2D	mainTexture;
uniform sampler2D	normalTexture;
uniform sampler2D	selfillumTexture;

uniform vec4		selfIllumColor;
uniform vec4		ambientColor;
uniform vec4		specularColor;
uniform vec4		diffuseColor;
uniform vec4		fogColor;
uniform vec4		rimColor;
uniform float		rimPower;

uniform float		fogBlend;
uniform	float		fogStart;

uniform vec3		eyePos;

varying vec4		worldPos, eyeCoord;
varying vec3		vNormal;

uniform float		shadowIntensity;
uniform vec3		mainLightPos;
uniform vec4		mainLightColor;
uniform vec3		lightPos[4];
uniform vec4		lightColor[4];

varying vec4		shadowMatrix;

void getShadow( vec3 lightPos, vec4 baseColor, vec4 specularColor, vec4 lightColor, vec3 E, vec4 worldPos, vec3 N, out vec4 diffuse, out vec4 specular )
{
	vec3 lightDir = lightPos - worldPos.xyz;
	vec3 L = normalize(lightDir);
	vec3 H = normalize(L + E);
	float df = max(0.0, dot(N, L));
	diffuse = baseColor * df * lightColor;
	float sf = pow( max(0.0, dot(N, H)), 30.0 );
	specular = specularColor * sf * lightColor;
}

void main(void)
{
	vec3 normal = texture2D( normalTexture, gl_TexCoord[0].st).xyz;
	normal = vNormal + normalize( normal * 2.0 - vec3(1,1,1) );
	
	vec3 vViewDir = eyePos - worldPos.xyz;
    vec3 N = normalize( normal );
    vec3 E = normalize(vViewDir);
	vec4 ambient = ambientColor;
	vec4 textured = texture2D( mainTexture, gl_TexCoord[0].st );
	vec4 selfIllum = texture2D( selfillumTexture, gl_TexCoord[0].st ) * selfIllumColor;
	
	vec4 baseColor = textured * diffuseColor;
	
	vec4 diffuse;
	vec4 specular;
	vec4 df;
	vec4 sp;
	
	getShadow( mainLightPos, baseColor, specularColor, mainLightColor, E, worldPos, N, df, sp );
	float mainLightDiffuse = df.r;
	specular += sp;
	diffuse += df;
	
	for( int k = 0; k < 4; k++) {
		getShadow( lightPos[k], baseColor, specularColor, lightColor[k], E, worldPos, N, df, sp );
		specular += sp;
		diffuse += df;
	}
	
	// Shadow mapping
	float shadow;
	vec3 coord = 0.5 * ( shadowMatrix.xyz / shadowMatrix.w + 1.0 );
	// Check if the coord is outside the shadowMap's area, in which case we want no shadow
	if (coord.x < 0.0)		shadow = 1.0;
	else if (coord.x > 1.0) shadow = 1.0;
	else if (coord.y < 0.0) shadow = 1.0;
	else if (coord.y > 1.0) shadow = 1.0;
	else {
		for( int ix = -1; ix <= 1; ix+=2) {
			for( int iy = -1; iy <= 1; iy+=2) {
				float ox = float(ix);
				float oy = float(iy);
				vec3 offset = vec3( 0.0007 * ox, 0.0007 * oy, 0.0 );
				shadow += shadow2D( depthTexture, coord + offset ).r / 4.0;
			}
		}
	}
	// Soften shadow edges
	if ( (shadow-1.0) * shadow * mainLightDiffuse != 0.0 ) {
		shadow = shadow / 8.0;
		for( int i = 1; i < 3; i++ )
		{
			float oi = float(i);
			for( int ix = -1; ix <= 1; ix+=2) {
				for( int iy = -1; iy <= 1; iy+=2) {
					float ox = float(ix);
					float oy = float(iy);
					vec3 offset = vec3( 0.0005 * ox * oi, 0.0005 * oy * oi, 0.0 );
					shadow += shadow2D( depthTexture, coord + offset ).r / 8.0;
				}
			}
		}
	}
	
	// Rim lighting
	float rf = 0.0;
	if (rimPower > 0.0) {
		rf = 1.0 - dot( normalize( vViewDir ), normalize( vNormal ) );
		rf = pow( rf, 1.0/rimPower);
	}
	
	// Final lit object
	vec4 objectColor =	ambient +
	//rimColor * rf * rimColor.a +
	( (1.0 - shadowIntensity) + shadowIntensity * shadow) * diffuse +
	specular * shadow;
	
	// Fog
	float fogMix = clamp( (length( vViewDir ) - fogStart) / fogBlend, 0.0, 1.0 );
	vec3 finalColor = mix( objectColor.xyz, fogColor.xyz, fogMix );
	gl_FragColor = vec4( finalColor, objectColor.a ) + selfIllum;
	
	gl_FragColor = objectColor;
	
}
