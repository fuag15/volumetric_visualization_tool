uniform sampler1D my_coltex;
uniform sampler3D my_tex;
uniform vec3 dimensions;
uniform float isoValue;
uniform float maxSize;
uniform float detail;
uniform float drawIso;
varying vec3 camera;
varying vec3 vertex;
uniform float alphaCorrect;
uniform int viewPlane;


// Lighting Stuff
vec3 LightPosition = vec3(0, 0, 0);
const float SpecularContribution = 0.2;
const float DiffuseContribution = 1.0 - SpecularContribution;
const float cellSize = 1.0 / 256.0;


vec3 ec;

vec3 getNormal(vec3 at)
{
    vec3 n = vec3(texture3D(my_tex, at - vec3(cellSize, 0.0, 0.0)).x - texture3D(my_tex, at + vec3(cellSize, 0.0, 0.0)).x,
                  texture3D(my_tex, at - vec3(0.0, cellSize, 0.0)).y - texture3D(my_tex, at + vec3(0.0, cellSize, 0.0)).y,
                  texture3D(my_tex, at - vec3(0.0, 0.0, cellSize)).z - texture3D(my_tex, at + vec3(0.0, 0.0, cellSize)).z
                 );
    
    return normalize(n);
}

float getLightIntensity(vec3 norm)
{
    vec3 lightVec = normalize(LightPosition - ec);
    vec3 viewVec = normalize(-ec);

    vec3 reflectVec = reflect(-lightVec, norm);
    float diffuse = max(dot(lightVec, norm), 0.3);
    float spec = 0.0;
    float ambient = 0.1;
    
    if (diffuse > 0.0 || ambient > 0.0)
    {
        spec = max(dot(reflectVec, viewVec), 0.2);
        spec = pow(spec, 16.0);            
    }
    
    return SpecularContribution*spec + DiffuseContribution*diffuse + ambient;
    //return norm;
}

void main()
{
	float LightIntensity = 1.0;
	float stepSize = 0.5;
	stepSize = detail;
	int firstRun = 0;
	//gl_FragColor = vec4(0,0,0,0);
	vec3 t0 = vertex;
	vec3 te = camera;
	vec3 eyeDirection = normalize(t0-te);
	if(viewPlane == 1)
	{
		vec3 origT0 = t0;
		//vec3 eyeDirection = normalize(t0-te);
		vec3 centerPoint = vec3((dimensions.x/2.0), (dimensions.y/2.0), (dimensions.z/2.0));
		vec3 normal = normalize(centerPoint-te);
		te += eyeDirection * (maxSize*2.0*0.4);
		vec3 test = vec3(te.x/dimensions.x, te.y/dimensions.y, te.z/dimensions.z);
		t0 = te;
		if(test.y > 1.0)
			t0 = origT0;
		if(test.y < 0.0)
			t0 = origT0;
		if(test.x > 1.0)
			t0 = origT0;
		if(test.x < 0.0)
			t0 = origT0;
		if(test.z > 1.0)
			t0 = origT0;
		if(test.z < 0.0)
			t0 = origT0;
		
		
		//vec3 planeOrig = te + (normal*maxSize*0.5);
		//vec3 planeOrig = vec3((dimensions.x/2.0), (dimensions.y/2.0), (dimensions.z/2.0));
		//vec3 planeToRay = planeOrig - te;
		//float t = dot(normal, planeToRay)/(dot(normal, eyeDirection));
		//te += eyeDirection * t;//(maxSize*planeDistance*2.0);
		//t0 = te;
	}
	vec4 currentColor;
	vec4 finalColor = vec4(0,0,0,0);
	vec4 colorTemp;
	
	float alphaCorrectV = alphaCorrect * 1000.0;
	
	ec = eyeDirection;
	te = te + eyeDirection* stepSize;
	int num_iterations = 800;
	num_iterations = (maxSize * 2.0)/stepSize;
	vec4 loccol = vec4(0.0,0.0,0.0,0.0);
	vec4 intersection;
	vec4 iproj;
	vec3 right = vec3(0.0, 0.0, 0.0);
	vec3 left = vec3(0.0, 0.0, 0.0);
	float pointIso;
	float tempFloat;
	vec3 pointPos;
	vec3 texPos;
	vec3 hitPos;
	float lastPointIso;
	vec3 lastTexPos = vec3((t0.x/dimensions.x),(t0.y/dimensions.y),(t0.z/dimensions.z));
	for(int i = 0; i < num_iterations; ++i)
	{
		t0 += eyeDirection * stepSize;
		texPos =vec3((t0.x/dimensions.x),(t0.y/dimensions.y),(t0.z/dimensions.z));
		pointIso = texture3D(my_tex, texPos).x;
		lastPointIso = texture3D(my_tex, lastTexPos).x;
		pointPos = t0.xyz;
		
		if(texPos.y > 1.0)
			break;
		if(texPos.y < 0.0)
			break;
		if(texPos.x > 1.0)
			break;	
		if(texPos.x < 0.0)
			break;	
		if(texPos.z > 1.0)
			break;
		if(texPos.z < 0.0)
			break;	
		
		currentColor = texture1D(my_coltex, pointIso);
		
		if(drawIso == 1)
		{
			//gl_FragColor = vec4(1,0,0,1);
			//return;
			if(pointIso == isoValue)
			{
				//t0 is actual position
				
				LightIntensity = getLightIntensity(getNormal(texPos));
				if(firstRun == 0)
				{
					intersection = vec4(t0.xyz, 1);
					iproj = gl_ModelViewProjectionMatrix * intersection;
					iproj.z /= iproj.w;
					gl_FragDepth = (iproj.z + 1.0)/2.0;
					gl_FragColor.xyz = texture1D(my_coltex, isoValue).xyz * LightIntensity;
					gl_FragColor.w = 1;
					return;
					//firstRun = 1;
				}
				else
				{
					tempFloat = 1.0 - finalColor.a;
					currentColor.a = 1.0 - pow(tempFloat, alphaCorrectV);
					currentColor = vec4(currentColor.xyz*LightIntensity, 1);
					finalColor.xyz = (currentColor.xyz) + ((1.0 - currentColor.a)*finalColor.xyz);
					finalColor.a = currentColor.a + ((1.0 - currentColor.a)*finalColor.a);
					gl_FragColor = finalColor;
					return;
				}
			}
			else if(pointIso > isoValue && lastPointIso < isoValue)
			{
				//t0 is actual position
				hitPos = mix(lastTexPos, texPos, 0.5);
				LightIntensity = getLightIntensity(getNormal(hitPos));
				pointIso = texture3D(my_tex, hitPos).x;
				if(firstRun == 0)
				{
					intersection = vec4(t0.xyz, 1);
					iproj = gl_ModelViewProjectionMatrix * intersection;
					iproj.z /= iproj.w;
					gl_FragDepth = (iproj.z + 1.0)/2.0;
					gl_FragColor.xyz = texture1D(my_coltex, isoValue).xyz * LightIntensity;
					gl_FragColor.w = 1;
					firstRun = 1;
					return;
				}
				else
				{
					tempFloat = 1.0 - finalColor.a;
					currentColor.a = 1.0 - pow(tempFloat, alphaCorrectV);
					currentColor = vec4(texture1D(my_coltex, isoValue).xyz*LightIntensity, 1);
					finalColor.xyz = (currentColor.xyz) + ((1.0 - currentColor.a)*finalColor.xyz);
					finalColor.a = currentColor.a + ((1.0 - currentColor.a)*finalColor.a);
					gl_FragColor = finalColor;
					return;
				}
				//gl_FragColor = vec4((iproj.z+1.0)/2.0, (iproj.z+1.0)/2.0, (iproj.z+1.0)/2.0, 1);
				//gl_FragColor = vec4(hitPos.x, hitPos.y, hitPos.z, 1);
				return;
			}
			lastTexPos = texPos;
		}
		if(currentColor.a != 0)
		{
			intersection = vec4(t0.xyz, 1);
			iproj = gl_ModelViewProjectionMatrix * intersection;
			iproj.z /= iproj.w;
			gl_FragDepth = (iproj.z + 1.0)/2.0;
			LightIntensity = getLightIntensity(getNormal(texPos));
			if(firstRun == 0)
			{
				
				finalColor = vec4(currentColor.xyz*LightIntensity, currentColor.a);
				firstRun = 1;
			}
			else
			{
				//tempFloat = 1.0 - finalColor.a;
				//currentColor.a = 1.0 - pow(tempFloat, alphaCorrectV);
				currentColor = vec4(currentColor.xyz*LightIntensity, currentColor.a);
				//finalColor.xyz = mix(finalColor.xyz, currentColor.xyz, finalColor.a);
				//finalColor.a = mix(finalColor.z, currentColor.a, finalColor.a);
				finalColor.xyz = (currentColor.xyz*currentColor.a) + ((1.0 - currentColor.a)*finalColor.xyz);
				finalColor.a = currentColor.a + ((1.0 - currentColor.a)*finalColor.a);
			}
			if(finalColor.a == 1)
			{
				gl_FragColor = finalColor;
				return;
			}
			//gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		}
	}
	discard;
}