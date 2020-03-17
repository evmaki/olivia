/*
 * Implements an ofxFadeScene (a scene that fades in and out up entry/exit) with MIDI listening capability.
 *
 * Any scenes that inherit from MidiScene must implement void newMidiMessage(ofxMidiMessage &m)
 * to handle incoming midi message events. MidiScene simply ensures that when the inheriting scene is
 * queued up by the scene manager, that scene is set as the current listener for midi events incoming on the
 * ofxMidiIn provided to the scene at construction.
 */
#pragma once

class MidiScene : public ofxFadeScene, public ofxMidiListener {
    
    public:
    
        MidiScene(const std::string &name, ofxMidiIn &midiIn) : ofxFadeScene(name), _midiIn(midiIn) {
            setSingleSetup(false); // this needs to be false by default so setup() is called each time any inheriting scene is entered
        }
    
        void setup() {
            _midiIn.addListener(this);
        }
    
        void updateEnter() {
            ofxFadeScene::updateEnter();
        }
    
        void updateExit() {
            ofxFadeScene::updateExit();
        }
        
        void exit() {
            _midiIn.removeListener(this);
        }
        
    private:
    
        ofxMidiIn &_midiIn;

};
