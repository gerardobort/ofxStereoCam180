# ofxStereoCam360
This project is a proof of concept on 3d side by side video recording by using 2 180º fisheye cameras.
The prototype built uses 2 cameras in an array mode to generate a stereographic stream (real time), with the addition of 3d navigation as result of the 3d mapping and a virtual camera within a skydome.

## Inspiration
The 180º image rectification process is based on the work presented by Paul Burke, [here](http://paulbourke.net/dome/fish2/).

## Captures
### Camera
[Original cameras](https://github.com/gerardobort/ofxStereoCam360/raw/master/doc/photo3.jpg "Original camera x2")
[Link to product](http://www.amazon.com/180degree-Fisheye-1080p-Angle-Camera/dp/B00LQ854AG)

### Homemade Camera
[Homemade stereo camera](https://github.com/gerardobort/ofxStereoCam360/raw/master/doc/photo2.jpg "Homemade stereo camera")
I used a small cardboard box with two holes and a couple of pair of screws.

### Software in action
[3d mapped cameras](https://github.com/gerardobort/ofxStereoCam360/raw/master/doc/photo3.jpg "At the top, left eye and right eye - VR")
This setup at the top shows up how to capture stereo video for Cardboard applications.

[App in action - video](https://www.instagram.com/p/_0hesPvNWe/?taken-by=gerardobort)

[![Result video - example](https://github.com/gerardobort/ofxStereoCam360/raw/master/doc/photo1.jpg "Result video example")](http://www.youtube.com/watch?v=u9Jdg3vNNtQ)

## Resources
The 3d navigation software is implemented in C++ using OpenFrameworks 0.9.  Modules used:
1. ofxGui for menues and settings
2. ofVideoGrabber for video capture
3. ofxVideoRecorder for video recording / live streamming
4. ofSphere / ofBox for skydome
5. OpenGL 3.2 - GLSL v150 for image rectification

:hamburger:
