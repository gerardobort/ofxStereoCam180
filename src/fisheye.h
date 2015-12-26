#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class fisheye {
public:
    fisheye();
    void update();
    void draw();
    
    ofParameterGroup parameters;
    ofParameter<int> cameraDeviceId;
    ofParameter<bool> displayVideoSource;

    ofVideoGrabber video;

    int cameraWidth;
    int cameraHeight;
    
private:
    ofShader shader;
    ofMesh mesh;
};
