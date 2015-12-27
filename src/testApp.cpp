#include "testApp.h"

bool shouldShowSettings;

//--------------------------------------------------------------
void testApp::setup(){

    ofSetFrameRate(60);
    
    //camWidth  = 1280;
    //camHeight = 720;
    //camWidth  = 1920;
    //camHeight = 1080;

    leftEye = new fisheye(1920, 1080, 0, "left");
    rightEye = new fisheye(1920, 1080, 1, "right");
    
    gui.setup("settings", "settings.xml", 10, 10);
    gui.add(leftEye->parameters);
    gui.add(rightEye->parameters);
    gui.loadFromFile("settings.xml");
    ofBackground(255, 255, 255);
    ofClear(0);
    shouldShowSettings = true;
    
}

//--------------------------------------------------------------
void testApp::update(){

    leftEye->update();
    rightEye->update();
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0, 0, 0);
    ofSetColor(255, 255, 255);

    float eyeWidth = ofGetWindowWidth()/2.0;
    float eyeHeight = ofGetWindowHeight()/2.0;
    float ribbonHeight = eyeHeight/2.0;
    leftEye->output.draw(0, ribbonHeight, eyeWidth, eyeHeight);
    rightEye->output.draw(eyeWidth, ribbonHeight, eyeWidth, eyeHeight);
    
    if (shouldShowSettings) {
        gui.draw();
    }

    string fpsInfo = "fps: " + ofToString(ofGetFrameRate(), 2);
    ofDrawBitmapStringHighlight(fpsInfo, 10, ofGetWindowHeight()-20);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case 's':
            shouldShowSettings = !shouldShowSettings;
            break;
        case ' ':
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}
