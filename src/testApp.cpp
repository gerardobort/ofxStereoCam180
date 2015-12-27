#include "testApp.h"
#define RECORD_VIDEO_WIDTH 1920
#define RECORD_VIDEO_HEIGHT 1080

bool shouldShowSettings;

//--------------------------------------------------------------
void testApp::setup(){

    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    
    //camWidth  = 1280;
    //camHeight = 720;
    //camWidth  = 1920;
    //camHeight = 1080;

    recordFbo.allocate(RECORD_VIDEO_WIDTH, RECORD_VIDEO_HEIGHT, GL_RGB);

    leftEye = new fisheye(1920, 1080, 0, "left");
    rightEye = new fisheye(1920, 1080, 1, "right");
    
    gui.setup("settings", "settings.xml", 10, 10);
    gui.add(leftEye->parameters);
    gui.add(rightEye->parameters);
    gui.loadFromFile("settings.xml");
    ofBackground(255, 255, 255);
    ofClear(0);
    shouldShowSettings = true;

    fileName = "sbs180_1";
    fileExt = ".mov";
    videoRecorder.setVideoCodec("mpeg4"); 
    videoRecorder.setVideoBitrate("800k");
    videoRecorder.setAudioCodec("mp3");
    videoRecorder.setAudioBitrate("192k");
    ofAddListener(videoRecorder.outputFileCompleteEvent, this, &testApp::recordingComplete);
    bRecording = false;
    
}

//--------------------------------------------------------------
void testApp::update(){

    leftEye->update();
    rightEye->update();

    if((leftEye->video.isFrameNew() || rightEye->video.isFrameNew()) && bRecording){

        recordFbo.begin();
            ofClear(0);
            ofSetColor(255, 255, 255);
            leftEye->output.draw(0, 0, RECORD_VIDEO_WIDTH/2.0, RECORD_VIDEO_HEIGHT);
            rightEye->output.draw(RECORD_VIDEO_WIDTH/2.0, 0, RECORD_VIDEO_WIDTH/2.0, RECORD_VIDEO_HEIGHT);
        recordFbo.end();

        ofPixels pixels;
        recordFbo.readToPixels(pixels);
        bool success = videoRecorder.addFrame(pixels);
        if (!success) {
            ofLogWarning("This frame was not added!");
        }
    }
    
    // Check if the video recorder encountered any error while writing video frame or audio smaples.
    if (videoRecorder.hasVideoError()) {
        ofLogWarning("The video recorder failed to write some frames!");
    }
    
    if (videoRecorder.hasAudioError()) {
        ofLogWarning("The video recorder failed to write some audio samples!");
    }
    
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


    stringstream ss;
    ss << "video queue size: " << videoRecorder.getVideoQueueSize() << endl
    << "audio queue size: " << videoRecorder.getAudioQueueSize() << endl
    << "FPS: " << ofGetFrameRate() << endl
    << (bRecording?"pause":"start") << " recording: r" << endl
    << (bRecording?"close current video file: c":"") << endl;

    ofSetColor(0,0,0,100);
    ofDrawRectangle(0, 0, 260, 75);
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(ss.str(), ofGetWindowWidth() - 300, ofGetWindowHeight() - 90);


    if (bRecording) {
        ofSetColor(255, 0, 0);
        ofDrawCircle(ofGetWidth() - 20, 20, 5);
    }
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

    if (key == 'r') {
        bRecording = !bRecording;
        if (bRecording && !videoRecorder.isInitialized()) {
            videoRecorder.setup("videos/"+fileName+ofGetTimestampString()+fileExt, RECORD_VIDEO_WIDTH, RECORD_VIDEO_HEIGHT, 30, sampleRate, channels);
            videoRecorder.start();
        } else if (!bRecording && videoRecorder.isInitialized()) {
            videoRecorder.setPaused(true);
        } else if (bRecording && videoRecorder.isInitialized()) {
            videoRecorder.setPaused(false);
        }
    }
    if (key=='c') {
        bRecording = false;
        videoRecorder.close();
    }
    
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

//--------------------------------------------------------------
void testApp::recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args){
    cout << "The recoded video file is now complete." << endl;
}

//--------------------------------------------------------------
void testApp::exit(){
    ofRemoveListener(videoRecorder.outputFileCompleteEvent, this, &testApp::recordingComplete);
    videoRecorder.close();
}
