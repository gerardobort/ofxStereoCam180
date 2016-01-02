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
    texturePlane.allocate(cameraWidth, cameraHeight, GL_RGBA);
    textureSphere.allocate(cameraWidth, cameraHeight, GL_RGBA);
    output.allocate(cameraWidth, cameraHeight, GL_RGBA);
    
    shader.load("shaders/rectify");
    image.load("cube_orig.jpg");

    setupGui();

    cam.setDistance(sphereRadius);
    cam.setNearClip(1.0);
    cam.setFarClip(2.0*sphereRadius);
    cam.lookAt(ofVec3f(0.0, 0.0, sphereRadius));

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
        mesh.addVertex(ofVec3f( w, -h, 0));
        
        mesh.addColor(ofColor(0, 100, 0));
        mesh.addColor(ofColor(0, 100, 0));
        mesh.addColor(ofColor(0, 100, 0));
        mesh.addColor(ofColor(0, 100, 0));

        mesh.addTexCoord(ofVec2f(-w, -h));
        mesh.addTexCoord(ofVec2f(-w,  h));
        mesh.addTexCoord(ofVec2f( w,  h));
        mesh.addTexCoord(ofVec2f( w, -h));

        mesh.addIndex(0);
        mesh.addIndex(1);
        mesh.addIndex(2);
        mesh.addIndex(0);
        mesh.addIndex(2);
        mesh.addIndex(3);

        input.begin();
            ofClear(0);
            ofBackground(0, 255, 0);
            ofSetColor(255, 255, 255);
            video.draw(0, 0, cameraWidth, cameraHeight);
        input.end();

        texturePlane.begin();
            ofClear(0);
            ofSetColor(255, 255, 255);
            input.draw(-rectifyOverflow, rectifyOverflow, cameraWidth +2*rectifyOverflow, cameraHeight -2*rectifyOverflow);
            if (calibrate) {
                ofEnableBlendMode(OF_BLENDMODE_ADD);
                image.draw(0, 0, cameraWidth, cameraHeight);
                ofEnableBlendMode(OF_BLENDMODE_ALPHA);
            }
        texturePlane.end();

        shader.begin();
            shader.setUniformTexture("u_sampler2dVideo", texturePlane.getTextureReference(0), 0);
            shader.setUniform1i("u_rectify", rectify);
            shader.setUniform1f("u_fovFactor", rectifyFovFactor);
            shader.setUniform1i("u_width", rectifyWidth);
            shader.setUniform1i("u_height", rectifyHeight);
        shader.end();


        textureSphere.begin();
            ofClear(0);
            ofBackground(0, 0, 0);
            ofSetColor(255, 255, 255);
            ofPushMatrix();
                ofTranslate((1.0-visibleRadius)*w/2.0, (1.0-visibleRadius)*h/2.0, 0);
                ofScale(visibleRadius, visibleRadius, 1.0);
                shader.begin();
                    mesh.draw();
                shader.end();
            ofPopMatrix();

            if (rectifyUseMirror) {
                textureSphere.getTextureReference()
                    .drawSubsection(0, 0, cameraWidth*0.25, cameraHeight, cameraWidth*0.5, 0, -cameraWidth*0.25, cameraHeight);
                textureSphere.getTextureReference()
                    .drawSubsection(cameraWidth*0.75, 0, cameraWidth*0.25, cameraHeight, cameraWidth*0.75, 0, -cameraWidth*0.25, cameraHeight);
            }
        
            if (displayVideoSource) {
                float scale = 0.2;
                int w = cameraWidth*scale;
                int h = cameraHeight*scale;
                video.draw(cameraWidth - (w + 10), cameraHeight - (h + 10), w, h);
            }
        textureSphere.end();

        output.begin();
            ofClear(0);
            ofBackground(0, 255, 0);
            ofSetColor(255, 255, 255);

            ofSpherePrimitive sphere;
            sphere.setRadius(sphereRadius);
            sphere.setMode(OF_PRIMITIVE_TRIANGLES);
            sphere.mapTexCoordsFromTexture(textureSphere.getTexture());
            textureSphere.getTextureReference().bind();

            cam.setPosition(-10.0*sphereOffsetX, 0.0, 0.0);
            ofPushMatrix();
                cam.begin();
                ofTranslate(sphereOffsetX, 0.0, 0.0);
                ofScale(-1.0, -1.0, 1.0);
                sphere.drawFaces();
                cam.end();
            ofPopMatrix();
            textureSphere.getTextureReference().unbind();
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
    parameters.add(rectifyOverflow.set("rectifyOverflow", 0, 0, 400));
    parameters.add(rectifyWidth.set("rectifyWidth", cameraWidth, 0, 4*cameraWidth));
    parameters.add(rectifyHeight.set("rectifyHeight", cameraHeight, 0, 4*cameraHeight));
    parameters.add(rectifyUseMirror.set("rectifyUseMirror", true));
    parameters.add(displayVideoSource.set("displayVideoSource", false));
    parameters.add(sphereRadius.set("sphereRadius", 2.0*cameraWidth, 0, 4.0*cameraWidth));
    parameters.add(sphereOffsetX.set("sphereOffsetX", 0, -100.0, 100.0));
}
