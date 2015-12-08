#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

// from example-contours-following

const float dyingTime = 0.5;

void Glow::setup(const cv::Rect& track) {
    color.setHsb(ofRandom(0, 255), 255, 255);
    cur = toOf(track).getCenter();
    smooth = cur;
}

void Glow::update(const cv::Rect& track) {
    cur = toOf(track).getCenter();
    smooth.interpolate(cur, .5);
    all.addVertex(smooth);
}

void Glow::kill() {
    float curTime = ofGetElapsedTimef();
    if(startedDying == 0) {
        startedDying = curTime;
    } else if(curTime - startedDying > dyingTime) {
        dead = true;
    }
}

void Glow::draw() {
    ofPushStyle();
    float size = 16;
    ofSetColor(255);
    if(startedDying) {
        ofSetColor(ofColor::red);
        size = ofMap(ofGetElapsedTimef() - startedDying, 0, dyingTime, size, 0, true);
    }
    ofNoFill();
    ofDrawCircle(cur, size);
    ofSetColor(color);
    all.draw();
    ofSetColor(255);
    ofDrawBitmapString(ofToString(label), cur);
    ofPopStyle();
}


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
    
    contourFinder.setMinAreaRadius(1);
    contourFinder.setMaxAreaRadius(100);
    contourFinder.setThreshold(15);
    
    contourFinder.setMinArea(50);
    contourFinder.setMaxArea(100);
    
    
    
    
    // wait for half a frame before forgetting something
    tracker.setPersistence(120);
    // an object can move up to 50 pixels per frame
    tracker.setMaximumDistance(60);
    
}

//--------------------------------------------------------------
void ofApp::update() {
    
    cam.update();
    
    if(cam.isFrameNew()) {
        
        ofImage camImg = cam.getPixels();
        // take the absolute difference of prev and cam and save it inside diff
        absdiff(previous, camImg, diff); //absdiff, add, subtract, multiply, divide
        diff.update();
        
        // like ofSetPixels, but more concise and cross-toolkit
        copy(cam, previous);
        
        blur(cam, 10);
        contourFinder.findContours(diff);
        tracker.track(contourFinder.getBoundingRects());
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(255);
    cam.draw(0, 0);
    
    ofTranslate(320, 0);
    
    diff.draw(0, 0);
    contourFinder.draw();
    vector<Glow>& followers = tracker.getFollowers();
    for(int i = 0; i < followers.size(); i++) {
        followers[i].draw();
    }
    
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
