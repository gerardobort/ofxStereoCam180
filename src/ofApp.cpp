#include "ofApp.h"
#define RECORD_VIDEO_WIDTH 1920
#define RECORD_VIDEO_HEIGHT 1080

bool shouldShowSettings;

//--------------------------------------------------------------
void ofApp::setup(){

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
    fileExt = ".mp4";
    videoRecorder.setVideoCodec("mpeg4");  // h264, libx264
    videoRecorder.setVideoBitrate("1600k");
    videoRecorder.setAudioCodec("mp3");
    videoRecorder.setAudioBitrate("192k");
    ofAddListener(videoRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    soundStream.listDevices();
    soundStream.setDeviceID(2);
    soundStream.setup(this, 0, channels, sampleRate, 256, 4);
    bRecording = false;
    
}

//--------------------------------------------------------------
void ofApp::update(){

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
void ofApp::draw(){
    ofBackground(0, 0, 0);
    ofSetColor(255, 255, 255);

    float eyeWidth = ofGetWindowWidth()/2.0;
    float eyeHeight = ofGetWindowHeight()/2.0;
    leftEye->texture.draw(0, 0, eyeWidth, eyeHeight);
    rightEye->texture.draw(eyeWidth, 0, eyeWidth, eyeHeight);
    leftEye->output.draw(0, eyeHeight, eyeWidth, eyeHeight);
    rightEye->output.draw(eyeWidth, eyeHeight, eyeWidth, eyeHeight);
    
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
void ofApp::keyPressed(int key){
    switch (key) {
        case 's':
            shouldShowSettings = !shouldShowSettings;
            break;
        case ' ':
            break;
        case OF_KEY_LEFT:
            if (leftEye->sphereSpinY < leftEye->sphereSpinY.getMax())
                leftEye->sphereSpinY+=2;
            if (rightEye->sphereSpinY < rightEye->sphereSpinY.getMax())
                rightEye->sphereSpinY+=2;
            break;
        case OF_KEY_RIGHT:
            if (leftEye->sphereSpinY > leftEye->sphereSpinY.getMin())
                leftEye->sphereSpinY-=2;
            if (rightEye->sphereSpinY > rightEye->sphereSpinY.getMin())
                rightEye->sphereSpinY-=2;
            break;
        case OF_KEY_UP:
            if (leftEye->sphereSpinX < leftEye->sphereSpinX.getMax())
                leftEye->sphereSpinX+=2;
            if (rightEye->sphereSpinX < rightEye->sphereSpinX.getMax())
                rightEye->sphereSpinX+=2;
            break;
        case OF_KEY_DOWN:
            if (leftEye->sphereSpinX > leftEye->sphereSpinX.getMin())
                leftEye->sphereSpinX-=2;
            if (rightEye->sphereSpinX > rightEye->sphereSpinX.getMin())
                rightEye->sphereSpinX-=2;
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    if (key == 'r') {
        bRecording = !bRecording;
        if (bRecording && !videoRecorder.isInitialized()) {
            // - for saving video
            // videoRecorder.setup("videos/"+fileName+ofGetTimestampString()+fileExt, RECORD_VIDEO_WIDTH, RECORD_VIDEO_HEIGHT, 30, sampleRate, channels);

            // - for custom ffmpeg output string (streaming, etc)
            // http://go.yuri.at/streaming-ffmpeg-with-nodejs-on-web-sockets-to-html/
            // from VLC: udp://@:1234 - unicast
            videoRecorder.setupCustomOutput(RECORD_VIDEO_WIDTH, RECORD_VIDEO_HEIGHT, 20, "-vcodec mpeg4 -r 20 -s 1920x1080 -g 0 -f mpegts -b 1600k udp://127.0.0.1:1234");
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
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void ofApp::audioIn(float *input, int bufferSize, int nChannels){
    if (bRecording)
        videoRecorder.addAudioSamples(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args){
    cout << "The recoded video file is now complete." << endl;
}

//--------------------------------------------------------------
void ofApp::exit(){
    ofRemoveListener(videoRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    videoRecorder.close();
}
