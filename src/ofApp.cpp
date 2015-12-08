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
    
    // ContourFinder
    contourFinder.setThreshold(100);
    contourFinder.setMinArea(50);
    contourFinder.setMaxArea(100);
    
    // Tracker
    // wait for half a frame before forgetting something
    tracker.setPersistence(120);
    // an object can move up to 50 pixels per frame
    tracker.setMaximumDistance(60);
    
    // GUI
    gui.setup(); // most of the time you don't need a name
    gui.add(threshold.setup("threshold", 100, 0, 255));
//    gui.add(filled.setup("fill", true));
//    gui.add(radius.setup("radius", 140, 10, 300));
//    gui.add(center.setup("center", ofVec2f(ofGetWidth()*.5, ofGetHeight()*.5), ofVec2f(0, 0), ofVec2f(ofGetWidth(), ofGetHeight())));
//    gui.add(color.setup("color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
//    gui.add(twoCircles.setup("two circles"));
//    gui.add(ringButton.setup("ring"));
//    gui.add(screenSize.setup("screen size", ofToString(ofGetWidth())+"x"+ofToString(ofGetHeight())));
    
    isGuiHidden = false;
    
    threshold.addListener(this, &ofApp::thresholdChanged);
    
}

//--------------------------------------------------------------
void ofApp::thresholdChanged(int &threshold){
    contourFinder.setThreshold(threshold);
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
    
    contourFinder.setThreshold(threshold);
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    ofSetColor(255);
    cam.draw(0, 0);
    
    ofPushMatrix();
    ofTranslate(320, 0);
    
    diff.draw(0, 0);
    contourFinder.draw();
    vector<Glow>& followers = tracker.getFollowers();
    for(int i = 0; i < followers.size(); i++) {
        followers[i].draw();
    }
    ofPopMatrix();
    
    if(!isGuiHidden){
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key) {
        case 'h':
            isGuiHidden = !isGuiHidden;
            break;
    }

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
