#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup() {
    
#ifdef _USE_LIVE_VIDEO
    cam.setVerbose(true);
    cam.setup(320,240);
#else
    cam.load("video.mov");
    cam.play();
    cam.setLoopState(OF_LOOP_NORMAL);
#endif
    
    ofSetVerticalSync(true);
    
    // imitate() will set up previous and diff
    // so they have the same size and type as cam
    imitate(previous, cam);
    imitate(diff, cam);
}

//--------------------------------------------------------------
void ofApp::update() {
    
    cam.update();
    
    if(cam.isFrameNew()) {
        ofImage camImg = cam.getPixels();
        // take the absolute difference of prev and cam and save it inside diff
        absdiff(previous, camImg, diff);
        diff.update();
        
        // like ofSetPixels, but more concise and cross-toolkit
        copy(cam, previous);
        
        // mean() returns a Scalar. it's a cv:: function so we have to pass a Mat
        diffMean = mean(toCv(diff));
        
        // you can only do math between Scalars,
        // but it's easy to make a Scalar from an int (shown here)
        diffMean *= Scalar(50);
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(255);
    cam.draw(0, 0);
    diff.draw(320, 0);
    
    // use the [] operator to get elements from a Scalar
    float diffRed = diffMean[0];
    float diffGreen = diffMean[1];
    float diffBlue = diffMean[2];
    
    ofSetColor(255, 0, 0);
    ofDrawRectangle(0, 0, diffRed, 10);
    ofSetColor(0, 255, 0);
    ofDrawRectangle(0, 15, diffGreen, 10);
    ofSetColor(0, 0, 255);
    ofDrawRectangle(0, 30, diffBlue, 10);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
