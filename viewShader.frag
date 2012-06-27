uniform sampler1D my_coltex;
uniform sampler3D my_tex;
uniform vec3 dimensions;
uniform float planeDistance;
uniform float maxSize;
varying vec3 camera;
varying vec3 vertex;

void main()
{
	vec3 t0 = vertex;
	vec3 te = camera;
	vec3 eyeDirection = normalize(t0-te);
	vec3 centerPoint = vec3((dimensions.x/2.0), (dimensions.y/2.0), (dimensions.z/2.0));
	vec3 normal = normalize(centerPoint-te);
	//te += eyeDirection * (maxSize*2.0*0.5);
	vec3 planeOrig = te + (normal*maxSize*planeDistance*2.0);
	//vec3 planeOrig = vec3((dimensions.x/2.0), (dimensions.y/2.0), (dimensions.z/2.0));
	vec3 planeToRay = planeOrig - te;
	float t = dot(normal, planeToRay)/(dot(normal, eyeDirection));
	te += eyeDirection * t;//(maxSize*planeDistance*2.0);
	vec3 texPos =vec3((te.x/dimensions.x),(te.y/dimensions.y),(te.z/dimensions.z));
	float pointIso = texture3D(my_tex, texPos).x;
	vec3 pointPos = te.xyz;
	if(texPos.y > 1.0)
		discard;
	if(texPos.y < 0.0)
		discard;
	if(texPos.x > 1.0)
		discard;	
	if(texPos.x < 0.0)
		discard;	
	if(texPos.z > 1.0)
		discard;
	if(texPos.z < 0.0)
		discard;
	
	vec4 intersection = vec4(te.xyz, 1);
	vec4 iproj = gl_ModelViewProjectionMatrix * intersection;
	iproj.z /= iproj.w;
	gl_FragDepth = (iproj.z+1.0)/2.0;
	vec4 tempColor = texture1D(my_coltex, pointIso);
	gl_FragColor = tempColor;
	
	if(tempColor.a == 0.0)
		discard;
	//gl_FragColor = vec4(1,0,0,1);
	return;
	/*	if(pointIso == isoValue)
		{
			//t0 is actual position
			intersection = (t0.xyz, 1);
			iproj = gl_ModelViewProjectionMatrix * intersection;
			iproj.z /= iproj.w;
			gl_FragDepth = (iproj.z + 1.0)/2.0;
			LightIntensity = getLightIntensity(getNormal(texPos));
			gl_FragColor.xyz = texture1D(my_coltex, isoValue).xyz * LightIntensity;
			gl_FragColor.w = texture1D(my_coltex, isoValue).w;
			//gl_FragColor = vec4(LightIntensity, LightIntensity, LightIntensity, 1);
			return;
		}
		else if(pointIso > isoValue && lastPointIso < isoValue)
		{
			
			vec3 hitPos = mix(lastTexPos, texPos, 0.5);
			intersection = (t0.xyz, 1);
			//iproj = gl_ModelViewProjectionMatrix * intersection;
			iproj = gl_ModelViewProjectionMatrix * intersection;
			iproj.z /= iproj.w;
			gl_FragDepth = (iproj.z+1.0)/2.0;
			LightIntensity = getLightIntensity(getNormal(hitPos));
			pointIso = texture3D(my_tex, hitPos).x;
			gl_FragColor.xyz = texture1D(my_coltex, isoValue).xyz * LightIntensity;
			gl_FragColor.w = texture1D(my_coltex, isoValue).w;
			//gl_FragColor = vec4((iproj.z+1.0)/2.0, (iproj.z+1.0)/2.0, (iproj.z+1.0)/2.0, 1);
			//gl_FragColor = vec4(hitPos.x, hitPos.y, hitPos.z, 1);
			return;
		}
		lastTexPos = texPos;
	}*/
	//discard;
}