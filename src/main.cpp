#include "ofxGui.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(){
    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    ofSetupOpenGL(1400, 940, OF_WINDOW);
    ofRunApp(new testApp());
}
