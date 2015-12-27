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
    input.allocate(cameraWidth, cameraHeight, GL_RGBA);
    output.allocate(cameraWidth, cameraHeight, GL_RGBA);
    
    shader.load("shaders/rectify");
    image.load("cube_orig.jpg");

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

        input.begin();
            ofClear(0);
            ofBackground(0, 255, 0);
            ofSetColor(255, 255, 255);
            video.draw(0, 0, cameraWidth, cameraHeight);
            if (calibrate) {
                ofEnableBlendMode(OF_BLENDMODE_ADD);
                image.draw(0, 0, cameraWidth, cameraHeight);
                ofEnableBlendMode(OF_BLENDMODE_ALPHA);
            }
        input.end();

        shader.begin();
            shader.setUniformTexture("u_sampler2dVideo", input.getTextureReference(), input.getTextureReference().getTextureData().textureID);
            shader.setUniform1i("u_rectify", rectify);
            shader.setUniform1f("u_fovFactor", rectifyFovFactor);
            shader.setUniform1i("u_width", rectifyWidth);
            shader.setUniform1i("u_height", rectifyHeight);
        shader.end();


        output.begin();
            ofClear(0);
            ofBackground(0, 255, 0);
            ofSetColor(255, 255, 255);
            ofPushMatrix();
                ofTranslate((1.0-visibleRadius)*w/2.0, (1.0-visibleRadius)*h/2.0, 0);
                ofScale(visibleRadius, visibleRadius, 1.0);
                shader.begin();
                    mesh.draw();
                shader.end();
            ofPopMatrix();
        
            if (displayVideoSource) {
                float scale = 0.2;
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

    parameters.add(visibleRadius.set("visibleRadius", 1, 1, 3));
    parameters.add(rectify.set("rectify", true));
    parameters.add(calibrate.set("calibrate", true));
    parameters.add(rectifyFovFactor.set("rectifyFovFactor", 1.0, 0, 4));
    parameters.add(rectifyWidth.set("rectifyWidth", cameraWidth, 0, 4*cameraWidth));
    parameters.add(rectifyHeight.set("rectifyHeight", cameraHeight, 0, 4*cameraHeight));
    parameters.add(displayVideoSource.set("displayVideoSource", false));
}
