//ofShader: ofShader: program reports:
//ERROR:CUSTOM-28 (vertex shader, line 1) Incorrect #version

// https://www.khronos.org/opengles/sdk/docs/reference_cards/OpenGL-ES-2_0-Reference-card.pdf

attribute vec4 a_vertex; // vertex position
attribute vec3 a_normal; // vertex normal
attribute vec4 a_color; // vertex color
attribute vec2 a_texcoord; // texture coordinates

uniform mat4 modelViewProjectionMatrix;
//uniform sampler2DRect u_sampler2dVideo;

varying vec4 colorVarying;

varying vec4 v_color;
varying vec4 v_position;
varying vec2 v_texCoord0;

void main()
{
	//vec4 pos = gl_ModelViewProjectionMatrix * gl_Vertex;
	vec4 pos = modelViewProjectionMatrix * a_vertex;
	gl_Position = pos;
	
	v_color = a_color;
	v_position = pos;
	//v_texCoord0 = gl_MultiTexCoord0.xy;
	v_texCoord0 = a_texcoord.xy;
}
