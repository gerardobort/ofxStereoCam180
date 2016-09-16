#include "ofxGui.h"
#include "ofApp.h"

//--------------------------------------------------------------
int main(){
    ofGLWindowSettings settings;
    settings.setGLVersion(2, 0);
    settings.windowMode = OF_FULLSCREEN;
    ofCreateWindow(settings);
    ofRunApp(new ofApp());
}
