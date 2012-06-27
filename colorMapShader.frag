uniform sampler3D my_tex;
uniform sampler1D my_coltex;

void main()
{
	vec4 my_intensity = texture3D(my_tex, gl_TexCoord[0].xyz);
	vec4 my_colorlookup = texture1D(my_coltex, my_intensity.x);
	if(my_colorlookup.a == 0.0)
	{
		discard;
	}
	else
	{
		gl_FragColor = my_colorlookup;
	}
	
}
