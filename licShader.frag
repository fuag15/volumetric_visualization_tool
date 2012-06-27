uniform sampler3D my_tex;
uniform sampler3D my_noisetex;
uniform sampler3D my_vectex;
uniform sampler1D my_coltex;

int i;
vec3 v;
vec3 stp;
vec4 color;
vec3 multAmount;
vec3 intensity;

void main()
{
// 	vec4 my_intensity = texture3D(my_tex, gl_TexCoord[0].xyz);
// 	vec4 my_colorlookup = texture1D(my_coltex, my_intensity.x);
// 	if(my_colorlookup.a == 0.0)
// 	{
// 		discard;
// 	}
// 	else
// 	{
// 		gl_FragColor = my_colorlookup;
// 	}
// 	
 	stp = gl_TexCoord[0].stp;
	multAmount = texture3D(my_noisetex, stp);
	intensity = texture3D(my_tex, stp);
	color = multAmount.x * texture1D(my_coltex, intensity.x);
	v = texture3D(my_vectex, stp);
	v *= 2.0/40.0;
	for(i=0; i < 20; i++)
	{
		stp -= v;
		stp = clamp(stp,0.,1.);
		multAmount = texture3D(my_noisetex, stp);
		intensity = texture3D(my_tex, stp);
		color += multAmount.x * texture1D(my_coltex, intensity.x);
		v = texture3D(my_vectex, stp);
		v *= 2.0/40.0;
	}
// 	
	stp = gl_TexCoord[0].stp;
	v = texture3D(my_vectex, stp);
// 	
	for(i=0; i< 20; i++)
	{
		stp += v;
		stp = clamp(stp,0.,1.);
		multAmount = texture3D(my_noisetex, stp);
		intensity = texture3D(my_tex, stp);
		color += multAmount.x * texture1D(my_coltex, intensity.x);
		v = texture3D(my_vectex, stp);
		v *= 2.0/40.0;
	}
	color.xyz = color.xyz/40.0;
	gl_FragColor = color;
	
	//gl_FragColor = texture3D(my_noisetex, gl_TexCoord[0].xyz);
	
}
