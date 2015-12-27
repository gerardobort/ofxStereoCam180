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
        ofFbo output;

        int deviceId;
        int cameraWidth;
        int cameraHeight;
        char* name;
        
    private:
        ofShader shader;
        ofMesh mesh;

        void setupGui();
        ofParameter<int> cameraDeviceId;
        ofParameter<float> visibleRadius;
        ofParameter<bool> displayVideoSource;
	    void setDeviceId(int& _value) { video.setDeviceID(_value); video.initGrabber(cameraWidth, cameraHeight); }
};
