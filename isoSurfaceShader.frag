uniform sampler1D my_coltex;
uniform sampler3D my_tex;
uniform vec3 dimensions;
uniform float isoValue;
uniform float maxSize;
uniform float detail;
varying vec3 camera;
varying vec3 vertex;


// Lighting Stuff
vec3 LightPosition = vec3(-1, -1, -1);
vec3 LightPosition2 = vec3(1,1,1);
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
    float LightIntensity = 1.0;
    vec3 lightVec = normalize(LightPosition - ec);
    vec3 lightVec2 = normalize(LightPosition2 - ec);
    vec3 viewVec = normalize(-ec);

    vec3 reflectVec = reflect(-lightVec, norm);
    vec3 reflectVec2 = reflect(-lightVec2, norm);
    float diffuse = max(dot(lightVec, norm), 0.3);
    float diffuse2 = max(dot(lightVec2, norm), 0.3);
    float spec = 0.0;
    float spec2 = 0.0;
    float ambient = 0.1;
    
    if (diffuse > 0.0 || ambient > 0.0)
    {
        spec = max(dot(reflectVec, viewVec), 0.2);
        spec = pow(spec, 16.0);            
    }
    
    if (diffuse2 > 0.0 || ambient > 0.0)
    {
        spec2 = max(dot(reflectVec2, viewVec), 0.2);
        spec2 = pow(spec, 16.0);            
    }
    
    float lightOne = SpecularContribution*spec + DiffuseContribution*diffuse;
    float lightTwo = SpecularContribution*spec2 + DiffuseContribution*diffuse2;
    
    float lightFinal = (lightOne + lightTwo) / 2.0;
    
    return lightFinal + ambient;
    //return norm;
}

void main()
{
	float LightIntensity = 1.0;
	float stepSize = 0.5;
	stepSize = detail;
	//gl_FragColor = vec4(0,0,0,0);
	vec3 t0 = vertex;
	vec3 te = camera;
	
	vec3 eyeDirection = normalize(t0-te);
	ec = eyeDirection;
	//ec = vec3(dimensions.x/2.0, dimensions.y/2.0, dimensions.z/2.0);
	te = te + eyeDirection* stepSize;
	int num_iterations = 800;
	num_iterations = (maxSize * 2.0)/stepSize;
	vec4 loccol = vec4(0.0,0.0,0.0,0.0);
	vec4 intersection;
	vec4 iproj;
	vec3 right = vec3(0.0, 0.0, 0.0);
	vec3 left = vec3(0.0, 0.0, 0.0);
	vec3 lastTexPos = vec3((t0.x/dimensions.x),(t0.y/dimensions.y),(t0.z/dimensions.z));
	vec3 hitPos;
	vec3 texPos;
	float pointIso;
	float lastPointIso;
	vec3 pointPos;
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
		if(pointIso == isoValue)
		{
			//t0 is actual position
			intersection = vec4(t0.xyz, 1);
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
			
			hitPos = mix(lastTexPos, texPos, 0.5);
			intersection = vec4(t0.xyz, 1);
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
	}
	discard;
}