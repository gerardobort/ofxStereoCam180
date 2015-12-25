#include "testApp.h"

float framerateMult;
bool shouldShowSettings;

//--------------------------------------------------------------
void testApp::setup(){
    framerateMult = 1.0f;

    ofSetFrameRate(60);
    
    //camWidth  = 1280;
    //camHeight = 720;
    camWidth  = 1920;
    camHeight = 1080;
    
    video.setVerbose(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    video.listDevices();
    video.setDeviceID(0);
    video.initGrabber(camWidth, camHeight);
    
    shader.load("shaders/diff");

    gui.setup("settings", "settings.xml", 10, 10);
    parameters.setName("s");
    parameters.add(leftCameraDeviceId.set("leftCameraDeviceId", 1, 0, 6));
    parameters.add(rightCameraDeviceId.set("rightCameraDeviceId", 1, 0, 6));
    parameters.add(displayVideoSource.set("displayVideoSource", true));
    gui.add(parameters);
    gui.loadFromFile("settings.xml");
    
    ofBackground(255, 255, 255);
    
    ofClear(0);
    shouldShowSettings = true;
    
}

//--------------------------------------------------------------
void testApp::update(){
    framerateMult = 60.0f/(1.0f/ofGetLastFrameTime());

    video.update();
    
    if (video.isFrameNew()) {
        int w = video.getWidth();
        int h = video.getHeight();
        
        mesh.clear();
        mesh.addVertex(ofVec3f(0, 0, 0));
        mesh.addVertex(ofVec3f(0, h, 0));
        mesh.addVertex(ofVec3f(w, h, 0));
        
        mesh.addVertex(ofVec3f(0, 0, 0));
        mesh.addVertex(ofVec3f(w, 0, 0));
        mesh.addVertex(ofVec3f(w, h, 0));
        
        mesh.addColor(ofColor(0, 100, 0));
        mesh.addColor(ofColor(0, 100, 0));
        mesh.addColor(ofColor(0, 100, 0));
        
        mesh.addColor(ofColor(0, 100, 0));
        mesh.addColor(ofColor(0, 100, 0));
        mesh.addColor(ofColor(0, 100, 0));

        shader.begin();
        shader.setUniformTexture("u_sampler2dVideo", video.getTextureReference(), video.getTextureReference().getTextureData().textureID);
        shader.end();

    }

}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255, 255, 255);
    
    ofPushMatrix();
    ofScale(
            ofGetWindowWidth()/video.getWidth(),
            ofGetWindowHeight()/video.getHeight(),
            1.f
            );
    shader.begin();
        ofPushMatrix();
            //ofScale(0.6, 0.6, 0.6);
            mesh.draw();
        ofPopMatrix();
    shader.end();
    ofPopMatrix();
    
    if (displayVideoSource) {
        float scale = 0.2;
        int w = ofGetWindowWidth()*scale;
        int h = ofGetWindowHeight()*scale;
        video.draw(ofGetWindowWidth() - (w + 10), ofGetWindowHeight() - (h + 10), w, h);
    }
    if (shouldShowSettings) {
        gui.draw();
    }
    string info = "fps: " + ofToString(ofGetFrameRate(),2)
    + "\nframerateMult: " + ofToString(framerateMult,2) ;
    ofDrawBitmapStringHighlight(info, 10, ofGetHeight()-40);

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
