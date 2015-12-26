#include "ofxGui.h"
#include "testApp.h"

//--------------------------------------------------------------
int main(){
    ofGLWindowSettings settings;
    settings.setGLVersion(3,2);
    ofCreateWindow(settings);
    ofRunApp(new testApp());
}
