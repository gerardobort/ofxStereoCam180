#include "fisheye.h"

//--------------------------------------------------------------
fisheye::fisheye(){
    //cameraWidth  = 1280;
    //cameraHeight = 720;
    cameraWidth  = 1920;
    cameraHeight = 1080;
    
    video.setVerbose(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    video.listDevices();
    video.setDeviceID(0);
    video.initGrabber(cameraWidth, cameraHeight);
    
    shader.load("shaders/diff");

    parameters.setName("camera");
    parameters.add(cameraDeviceId.set("cameraDeviceId", 1, 0, 6));
    parameters.add(displayVideoSource.set("displayVideoSource", true));
    
    ofBackground(255, 255, 255);
    
    ofClear(0);
}

//--------------------------------------------------------------
void fisheye::update(){
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
void fisheye::draw(){
    ofSetColor(255, 255, 255);
    ofPushMatrix();
        ofScale(
                ofGetWindowWidth()/video.getWidth(),
                ofGetWindowHeight()/video.getHeight(),
                1.f
                );
        shader.begin();
            mesh.draw();
        shader.end();
    ofPopMatrix();
    
    if (displayVideoSource) {
        float scale = 0.2;
        int w = ofGetWindowWidth()*scale;
        int h = ofGetWindowHeight()*scale;
        video.draw(ofGetWindowWidth() - (w + 10), ofGetWindowHeight() - (h + 10), w, h);
    }
}
