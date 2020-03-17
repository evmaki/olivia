/*
 * Template for an empty scene with MIDI input.
 */
#pragma once

#include "MidiScene.h"

class EmptyScene : public MidiScene {

    public:
    
        EmptyScene(ofxMidiIn &midiInPtr) : MidiScene("Empty", midiInPtr) {

        }
    
        // scene setup
        void setup() {
            // call the superclass to setup midi input
            // any scenes that inherit from MidiScene need to call setup in the superclass
            // in order to work correctly
            MidiScene::setup();
        }

        // normal update
        void update() {

        }

        // draw
        void draw() {

        }
        
        // cleanup
        void exit() {
            // call the superclass to shut down midi input
            // any scenes that inherit from MidiScene need to call exit() in the superclass
            // in order to work correctly
            MidiScene::exit();
        }
    
        // handle incoming midi messages
        void newMidiMessage(ofxMidiMessage &msg) {

        }
    
};

