#version 120

uniform sampler2DRect u_sampler2dVideo;
uniform bool u_rectify;
uniform float u_fovFactor;
uniform int u_width;
uniform int u_height;

varying vec4 v_color;
varying vec4 v_position;
varying vec2 v_texCoord0;

float rnd(vec2 x);
vec2 fish2sphere(vec2 pfish);
 
void main()
{
    vec4 videoColor;

    if (u_rectify) {
        videoColor = texture2DRect(u_sampler2dVideo, fish2sphere(v_texCoord0));
    } else {
        videoColor = texture2DRect(u_sampler2dVideo, v_texCoord0);
    }
    gl_FragColor = videoColor;
}

// http://paulbourke.net/dome/fish2/
vec2 fish2sphere(vec2 pfish)
{
    vec2 ret;
    float theta, phi, r;
    vec3 psph;
    
    // float FOV = 3.141592654*1.0; // FOV of the fisheye, eg: 180 degrees
    // float width = 1920.0;
    // float height = 1080.0;
    float FOV = 3.141592654*u_fovFactor;
    float width = u_width;
    float height = u_height;

    // Polar angles
    theta = 2.0 * 3.14159265 * (pfish.x / width - 0.5); // -pi to pi
    phi = 3.14159265 * (pfish.y / height - 0.5);  // -pi/2 to pi/2

    // Vector in 3D space
    psph.x = cos(phi) * sin(theta);
    psph.y = cos(phi) * cos(theta);
    psph.z = sin(phi);
    
    // Calculate fisheye angle and radius
    theta = atan(psph.z, psph.x);
    phi = atan(sqrt(psph.x*psph.x + psph.z*psph.z), psph.y);
    r = height * phi / FOV; 

    // Pixel in fisheye space
    ret.x = 0.5 * width + r * cos(theta);
    ret.y = 0.5 * height + r * sin(theta);
    
    return ret;
}
