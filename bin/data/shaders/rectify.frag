#version 150
 
in vec4 v_color;
in vec4 v_position;
in vec2 v_texCoord0;

uniform sampler2DRect u_sampler2dVideo;
uniform bool u_rectify;
uniform float u_fovFactor;
uniform int u_width;
uniform int u_height;

out vec4 outputColor;

vec3 rgb2hsv(vec3 c);
vec3 hsv2rgb(vec3 c);
vec3 rgb2hsl(vec3 c);
vec3 hsl2rgb(vec3 c);
vec3 getMask();
float rnd(vec2 x);
vec2 fish2sphere(vec2 pfish);
vec2 fish2persp(vec2 pfish);
 
void main()
{
    vec4 videoColor;

    if (u_rectify) {
        videoColor = texture(u_sampler2dVideo, fish2sphere(v_texCoord0));
    } else {
        videoColor = texture(u_sampler2dVideo, v_texCoord0);
    }
    outputColor = videoColor;
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

vec2 fish2persp(vec2 pfish)
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
    r = height * phi / FOV; 

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


float rnd(vec2 x)
{
    int n = int(x.x * 40.0 + x.y * 6400.0);
    n = (n << 13) ^ n;
    return 1.0 - float( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0;
}

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}


vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}


vec3 rgb2hsl(vec3 color)
{
    vec3 hsl; // init to 0 to avoid warnings ? (and reverse if + remove first part)

    float fmin = min(min(color.r, color.g), color.b); //Min. value of RGB
    float fmax = max(max(color.r, color.g), color.b); //Max. value of RGB
    float delta = fmax - fmin; //Delta RGB value

    hsl.z = (fmax + fmin) / 2.0; // Luminance

    if (delta == 0.0)   //This is a gray, no chroma...
    {
        hsl.x = 0.0;    // Hue
        hsl.y = 0.0;    // Saturation
    }
    else //Chromatic data...
    {
        if (hsl.z < 0.5)
            hsl.y = delta / (fmax + fmin); // Saturation
        else
            hsl.y = delta / (2.0 - fmax - fmin); // Saturation

        float deltaR = (((fmax - color.r) / 6.0) + (delta / 2.0)) / delta;
        float deltaG = (((fmax - color.g) / 6.0) + (delta / 2.0)) / delta;
        float deltaB = (((fmax - color.b) / 6.0) + (delta / 2.0)) / delta;

        if (color.r == fmax )
            hsl.x = deltaB - deltaG; // Hue
        else if (color.g == fmax)
            hsl.x = (1.0 / 3.0) + deltaR - deltaB; // Hue
        else if (color.b == fmax)
            hsl.x = (2.0 / 3.0) + deltaG - deltaR; // Hue

        if (hsl.x < 0.0)
            hsl.x += 1.0; // Hue
        else if (hsl.x > 1.0)
            hsl.x -= 1.0; // Hue
    }

    return hsl;
}

float hue2rgb(float f1, float f2, float hue)
{
    if (hue < 0.0)
        hue += 1.0;
    else if (hue > 1.0)
        hue -= 1.0;
    float res;
    if ((6.0 * hue) < 1.0)
        res = f1 + (f2 - f1) * 6.0 * hue;
    else if ((2.0 * hue) < 1.0)
        res = f2;
    else if ((3.0 * hue) < 2.0)
        res = f1 + (f2 - f1) * ((2.0 / 3.0) - hue) * 6.0;
    else
        res = f1;
    return res;
}

vec3 hsl2rgb(vec3 hsl)
{
    vec3 rgb;

    if (hsl.y == 0.0)
        rgb = vec3(hsl.z); // Luminance
    else
    {
        float f2;

        if (hsl.z < 0.5)
        f2 = hsl.z * (1.0 + hsl.y);
        else
        f2 = (hsl.z + hsl.y) - (hsl.y * hsl.z);

        float f1 = 2.0 * hsl.z - f2;

        rgb.r = hue2rgb(f1, f2, hsl.x + (1.0/3.0));
        rgb.g = hue2rgb(f1, f2, hsl.x);
        rgb.b= hue2rgb(f1, f2, hsl.x - (1.0/3.0));
    }

    return rgb;
}
