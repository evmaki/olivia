#include "ofMain.h"
#include "ofApp.h"

int main() {
	ofSetupOpenGL(1024, 768, OF_WINDOW);

	// this replaces ofRunApp() and kicks off the running of the app
	// with the app utils
	ofRunAppWithAppUtils(new ofApp());
}
