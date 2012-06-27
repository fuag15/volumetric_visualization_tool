/*! \file geometryPassthrough.glsl
 *  \author Jared Hoberock
 *  \brief Test shader.
 */ 
 
#version 120 
// varying vec3 my_position;

void main()
{
//  	my_position = gl_Vertex.xyz;
//  	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
 	
 	//gl_Position = ftransform();
  	gl_Position = gl_Vertex;
  	//gl_FrontColor = gl_Color;
}