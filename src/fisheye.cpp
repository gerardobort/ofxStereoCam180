#include "fisheye.h"

//--------------------------------------------------------------
fisheye::fisheye(int cameraWidth, int cameraHeight, int deviceId, char* name)
    : cameraWidth(cameraWidth), cameraHeight(cameraHeight), deviceId(deviceId), name(name) {
    
    video.setVerbose(true);
    //video.listDevices();
    if (deviceId) {
        video.setDeviceID(deviceId);
        cout << "************** camera device IN USE: " << deviceId << endl;
    }
    video.initGrabber(cameraWidth, cameraHeight);
    output.allocate(cameraWidth, cameraHeight, GL_RGBA);
    
    shader.load("shaders/diff");

    setupGui();
}

//--------------------------------------------------------------
void fisheye::update(){
    video.update();
    
    if (video.isFrameNew()) {
        int w = video.getWidth();
        int h = video.getHeight();
        
        mesh.clear();
        mesh.addVertex(ofVec3f(-w, -h, 0));
        mesh.addVertex(ofVec3f(-w,  h, 0));
        mesh.addVertex(ofVec3f( w,  h, 0));
        
        mesh.addVertex(ofVec3f(-w, -h, 0));
        mesh.addVertex(ofVec3f( w, -h, 0));
        mesh.addVertex(ofVec3f( w,  h, 0));
        
        mesh.addColor(ofColor(0, 100, 0));
        mesh.addColor(ofColor(0, 100, 0));
        mesh.addColor(ofColor(0, 100, 0));
        
        mesh.addColor(ofColor(0, 100, 0));
        mesh.addColor(ofColor(0, 100, 0));
        mesh.addColor(ofColor(0, 100, 0));

        shader.begin();
            shader.setUniformTexture("u_sampler2dVideo", video.getTextureReference(), video.getTextureReference().getTextureData().textureID);
            shader.setUniform1i("u_rectify", rectify);
        shader.end();


        output.begin();
            ofClear(0);
            ofBackground(0, 255, 0);
            ofSetColor(255, 255, 255);
            ofPushMatrix();
                //ofScale(
                //        ofGetWindowWidth()/video.getWidth(),
                //        ofGetWindowHeight()/video.getHeight(),
                //        1.f
                //        );
                ofTranslate((1.0-visibleRadius)*w/2.0, (1.0-visibleRadius)*h/2.0, 0);
                ofScale(visibleRadius, visibleRadius, 1.0);
                shader.begin();
                    mesh.draw();
                shader.end();
            ofPopMatrix();
        
            if (displayVideoSource) {
                float scale = 0.2;
                //int w = ofGetWindowWidth()*scale;
                //int h = ofGetWindowHeight()*scale;
                int w = cameraWidth*scale;
                int h = cameraHeight*scale;
                video.draw(cameraWidth - (w + 10), cameraHeight - (h + 10), w, h);
            }
        output.end();

    }

}

//--------------------------------------------------------------
void fisheye::draw(){
}

//--------------------------------------------------------------
void fisheye::setupGui() {
    parameters.setName(name);

    // params
    ofVideoGrabber tvid;
    vector<ofVideoDevice> allCameras;
    allCameras = tvid.listDevices();
	parameters.add(cameraDeviceId.set("device", cameraDeviceId, 0, allCameras.size()-1));
	cameraDeviceId.addListener(this, &fisheye::setDeviceId);

    parameters.add(visibleRadius.set("visibleRadius", 1, 1, 2));
    parameters.add(rectify.set("rectify", true));
    parameters.add(displayVideoSource.set("displayVideoSource", false));
}
