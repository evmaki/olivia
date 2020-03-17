#pragma once

#include "ofMain.h"

#include "ofxAppUtils.h"
#include "ofxMidi.h"

// optional ofxGUI control panel for transformer,
// uncomment this if you're using ofxGui in this project
#define HAVE_OFX_GUI

#ifdef HAVE_OFX_GUI
	#include "ofxGui.h"
	//#include "ofxTransformPanel.h"
#endif

// we only specify a pointer to the Particle Scene here, so tell the compiler
// that it exists as a class and we can include the class header in ofApp.cpp
//class ParticleScene;

class ofApp : public ofxApp {
	
	public:
	
		ofApp() {}
	
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		
		void windowResized(int w, int h);
    
        ofxMidiIn _midiIn;

		// handles the scenes
		ofxSceneManager _sceneManager;
		int _lastScene;

};
