varying vec3 camera;
varying vec3 vertex;

void main()
{
	vec4 cameraPos = gl_ModelViewMatrixInverse*vec4(0,0,0,1);
	camera = cameraPos.xyz; 

	vec4 vertexPos = gl_Vertex;
	vertex = vertexPos.xyz;
	gl_Position = ftransform();
}