uniform sampler2D	mainTexture;
uniform vec4	diffuseColor;

void main()
{
	gl_FragColor = texture2D( mainTexture, gl_TexCoord[0].st ) * diffuseColor;
}