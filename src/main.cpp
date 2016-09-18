#include "ofxGui.h"
#include "ofApp.h"

//--------------------------------------------------------------
int main(){
    //ofGLWindowSettings settings;
    //settings.setGLVersion(2, 1);
    ofGLESWindowSettings settings;
    settings.setGLESVersion(2);
    settings.windowMode = OF_WINDOW;
    settings.width = 1200;
    settings.height = 960;
    ofCreateWindow(settings);
    ofRunApp(new ofApp());
}
