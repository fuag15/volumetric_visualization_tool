// #version 120 
// #extension GL_EXT_geometry_shader4 : enable 
// a passthrough geometry shader for color and position 
void main() 
{ 
// 	vec4 bot = gl_ModelViewProjectionMatrix*gl_PositionIn[0];
// 	vec4 top = gl_ModelViewProjectionMatrix*gl_PositionIn[1];
// 	vec4 side = gl_ModelViewProjectionMatrix*gl_PositionIn[2];
//  	vec3 height = vec3(top - bot);
// 	vec3 width = vec3(side - top);
// 	
// 	float numRows = 4;
// 	height = height/(numRows+1);
// 	width = width/(numRows+1);
// 	vec3 orig = (gl_ModelViewProjectionMatrix*gl_PositionIn[0]).xyz;
// 	for(int j = 0; j < numRows; j++)
// 	{
// 		orig += height;
// 		for(int k = 0; k < numRows; k++)
// 		{
// 			orig += width;
// 			//at each poitn here draw a line
// 		}
// 	}

// 	 gl_Position = gl_ModelViewProjectionMatrix*gl_PositionIn[0];
// 	 
// 	 gl_TexCoord = gl_TexCoordIn[0];
// 	 
// 	 gl_FrontColor = gl_FrontColorIn[0];
// 	 
// 	 EmitVertex();
// 	 
// 	 gl_Position = gl_ModelViewProjectionMatrix*gl_PositionIn[1];
// 	 
// 	 gl_TexCoord = gl_TexCoordIn[1];
// 	 
// 	 gl_FrontColor = gl_FrontColorIn[1];
// 	 
// 	 EmitVertex();
// 	 
// 	 EndPrimitive();
  for(int i = 0; i < gl_VerticesIn; ++i) 
  { 
    // copy color 
    gl_FrontColor = gl_ModelViewProjectionMatrix*gl_FrontColorIn[i]; 
 
    // copy position 
    gl_Position = gl_ModelViewProjectionMatrix*gl_PositionIn[i]; 
    
    //gl_TexCoord = gl_TexCoordIn[i];
 
    // done with the vertex 
    EmitVertex(); 
  } 
} 