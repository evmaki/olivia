#include "ofApp.h"

#include "scenes/scenes.h"

//--------------------------------------------------------------
void ofApp::setup() {
    _midiIn.listInPorts();
    _midiIn.openPort(0);
    
    ofBackground(0, 0, 0);
    
    // load scenes
    _sceneManager.add(new WavyScene(_midiIn));
    _sceneManager.add(new NautilusScene(_midiIn));
    _sceneManager.add(new ScanLineScene(_midiIn));

    // configure scene manager
    ofSetLogLevel("ofxSceneManager", OF_LOG_VERBOSE); // lets see whats going on inside
    
    // start with a specific scene
    // set now to true in order to ignore the scene fade and change now
    _sceneManager.gotoScene("Scan Lines", true);
    _lastScene = _sceneManager.getCurrentSceneIndex();
    
    // overlap scenes when transitioning
    _sceneManager.setOverlap(true);
    
    // attach scene manager to this ofxApp so it's called automatically,
    // you can also call the callbacks (update, draw, keyPressed, etc) manually
    // if you don't set it
    //
    // you can also turn off the auto sceneManager update and draw calls with:
    // setSceneManagerUpdate(false);
    // setSceneManagerDraw(false);
    //
    // the input callbacks in your scenes will be called if they are implemented
    //
    setSceneManager(&_sceneManager);
}

//--------------------------------------------------------------
// the current scene is automatically updated before this function
void ofApp::update() {

}

//--------------------------------------------------------------
// the current scene is automatically drawn before this function
void ofApp::draw() {
	// show the render area edges with a white rect
	if(isDebug()) {
		ofNoFill();
		ofSetColor(255);
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofDrawRectangle(1, 1, getRenderWidth()-2, getRenderHeight()-2);
		ofFill();
	}
	
	// draw current scene info using the ofxBitmapString stream interface
	// to ofDrawBitmapString
	ofSetColor(200);
	ofxBitmapString(12, ofGetHeight()-8)
		<< "Current Scene: " << _sceneManager.getCurrentSceneIndex()
		<< " " << _sceneManager.getCurrentSceneName() << endl;
	
	// the warp editor is drawn automatically after this function
}

// current scene input functions are called automatically before calling these
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch(key) {
	
		case 'd':
			bDebug = !bDebug;
			break;
									
		case 'f':
			ofToggleFullscreen();
			break;
	
		case OF_KEY_LEFT:
			_sceneManager.prevScene();
			break;
			
		case OF_KEY_RIGHT:
			_sceneManager.nextScene();
			break;
			
		case OF_KEY_DOWN:
			if(_sceneManager.getCurrentScene()) { // returns NULL if no scene selected
				_lastScene = _sceneManager.getCurrentSceneIndex();
			}
			_sceneManager.noScene();
			break;
			
		case OF_KEY_UP:
			_sceneManager.gotoScene(_lastScene);
			break;
		case 'o':
			_sceneManager.setOverlap(!_sceneManager.getOverlap());
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}
