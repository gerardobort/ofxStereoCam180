#version 150
 
in vec4 v_color;
in vec4 v_position;
in vec2 v_texCoord0;

uniform sampler2DRect u_sampler2d;

out vec4 outputColor;

void main()
{
    vec4 c91 = texture(u_sampler2d, v_texCoord0 + vec2( - 4.0, 0.0));
    vec4 c92 = texture(u_sampler2d, v_texCoord0 + vec2( - 3.0, 0.0));
    vec4 c93 = texture(u_sampler2d, v_texCoord0 + vec2( - 2.0, 0.0));
    vec4 c94 = texture(u_sampler2d, v_texCoord0 + vec2( - 1.0, 0.0));
    vec4 c95 = texture(u_sampler2d, v_texCoord0);
    outputColor = 1.0 
    - vec4(1.0 
    	- c95.rgb 
    	- 0.6*c94.rgb 
    	- 0.3*c93.rgb 
    	- 0.02*c92.rgb 
    	- 0.01*c92.rgb 
    	- 0.005*c91.rgb, 0);
}
