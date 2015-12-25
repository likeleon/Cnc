uniform sampler2D DiffuseTexture;
void main()
{
	gl_FragColor = texture2D(DiffuseTexture,gl_TexCoord[0].st);
	gl_FragColor = vec4(1,0,0,1);
}