#version 150
 
in vec4 a_position;
in vec4 a_color;
in vec2 a_texCoord0;

uniform mat4 modelViewProjectionMatrix;
 
out vec4 v_color;
out vec4 v_position;
out vec2 v_texCoord0;

void main(){
    gl_Position = modelViewProjectionMatrix * a_position;
    v_color = a_color;
    v_position = gl_Position;
    v_texCoord0 = a_position.xy;
}
