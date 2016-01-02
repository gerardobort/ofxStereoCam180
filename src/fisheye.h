#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class fisheye {
    public:
        fisheye(int cameraWidth = 640, int cameraHeight = 480, int deviceId = 0, char* name = "Unnamed");
        void update();
        void draw();
        
        ofParameterGroup parameters;

        ofVideoGrabber video;
        ofFbo input;
        ofFbo texturePlane;
        ofFbo textureSphere;
        ofFbo output;

        int deviceId;
        int cameraWidth;
        int cameraHeight;
        char* name;

    private:
        ofShader shader;
        ofMesh mesh;
        ofImage image;

        void setupGui();
        ofParameter<int> cameraDeviceId;
        ofParameter<float> visibleRadius;
        ofParameter<bool> rectify;
        ofParameter<bool> calibrate;
        ofParameter<float> rectifyFovFactor;
        ofParameter<int> rectifyOverflow;
        ofParameter<int> rectifyWidth;
        ofParameter<int> rectifyHeight;
        ofParameter<bool> rectifyUseMirror;
        ofParameter<bool> displayVideoSource;
        ofParameter<float> sphereRadius;
        ofParameter<float> sphereOffsetX;
	    void setDeviceId(int& _value) { video.setDeviceID(_value); video.initGrabber(cameraWidth, cameraHeight); }
        ofEasyCam cam;
};
