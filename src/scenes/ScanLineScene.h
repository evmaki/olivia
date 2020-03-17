/*
 * Scan lines toggled by midi input.
 */
#pragma once

#include "MidiScene.h"

#include "ofColor.h"
#include "ofxTweenzor.h"

class ScanLineScene : public MidiScene {

    public:

        ScanLineScene(ofxMidiIn &midiInPtr) : MidiScene("Scan Lines", midiInPtr){
            Tweenzor::init();
            Tweenzor::add(&_pulseAmount, 100.f, 200.f, 0.f, _pulsePeriod);
            Tweenzor::getTween(&_pulseAmount)->setRepeat(-1, true);
            
            setFade(1000, 1000);
        }

        // scene setup
        void setup() {
            MidiScene::setup();
            
            _colorPalette.push_back(ofColor(255, 133, 177));
            _colorPalette.push_back(ofColor(247, 79, 140));
            _colorPalette.push_back(ofColor(241, 29, 107));
            _colorPalette.push_back(ofColor(133, 206, 255));
        }

        void update() {
            Tweenzor::update(ofGetElapsedTimeMillis());
            
            // remove invisible scan lines
            // without this, _scanLines gets filled with tons of invisible scanlines and the interface gets laggy
            for (auto it = _scanLines.cbegin(); it != _scanLines.cend() ; ) {
                if (*(it->second.fadeAmount) <= 0.f) {
                    _scanLines.erase(it++);
                }
                else {
                    ++it;
                }
            }
        }

        // draw
        void draw() {
            // TODO intermittent bug here (race condition)
            // accesses to _scanLines should probably be protected by a mutex so that the thread capturing midi events
            // and the thread performing draw updates cannot attempt to access it at the same time
            if (_scanLines.size() > 0) {
                for (auto it = _scanLines.begin(); it != _scanLines.end(); ++it) {
                    ScanLine scanLine = it->second;
                    float fadeAmount = *(scanLine.fadeAmount);

                    // alpha is inherited from ofxFadeScene to control scene fading in/out
                    ofSetColor(scanLine.color, (fadeAmount*_pulseAmount)*alpha);

                    float y;
                    // fill the screen with scan lines
                    while (scanLine.step * scanLine.pitch < ofGetHeight()) {
                        y = (scanLine.y + (scanLine.pitch * scanLine.step)) % (ofGetHeight() + scanLine.maxHeight) - scanLine.maxHeight;
                        ofDrawRectangle(scanLine.x, y, ofGetWidth(), scanLine.height);

                        ++scanLine.step;
                    }
                    
                    // increment base position so next draw() the scan lines will be drawn a bit further down,
                    // creating the appearance of movement
                    it->second.y = (it->second.y + it->second.speed) % ofGetHeight();
                }
            }
        }

        // cleanup
        void exit() {
            MidiScene::exit();
            
            Tweenzor::removeAllTweens();
            Tweenzor::removeAllListeners();
            _scanLines.clear();
        }
    
        void newMidiMessage(ofxMidiMessage &msg) {
            // create a new scanline for the incoming MIDI_NOTE_ON message
            // this involves:
            //      - creating a tween to control the fading in of the scanline
            //      - adding the new scanline to the _scanLines map to be rendered on the next draw() update
            if (msg.status == MIDI_NOTE_ON) {
                // create and add a tween value to control the "fade in/out" for the new note
                float *fadeAmount = new float[1];
                fadeAmount[0] = 100.f;
                
                Tweenzor::add(fadeAmount, 0.f, 1.f, 0.f, _fadeInTime, EASE_IN_OUT_CUBIC);
                
                if (_scanLines.count(msg.pitch) > 0) {
                    _scanLines.erase(msg.pitch);
                }
                
                ScanLineScene:ScanLine s(msg.pitch, _scanLines.size(), _colorPalette[_scanLines.size()%_colorPalette.size()], fadeAmount);

                _scanLines.insert({msg.pitch, s});
            }
            // prepare an existing scanline to be faded out in response to the incoming MIDI_NOTE_OFF message
            // this involves:
            //      - creating a tween to control the fade out of the scanline
            //      - adding a callback (onTweenComplete()) to be fired when the tween is complete
            else if (msg.status == MIDI_NOTE_OFF && _scanLines.count(msg.pitch) > 0) {
                // change the "fade in" tween to a "fade out" and add a callback to remove the tween once complete
                float *fadeAmount = _scanLines.at(msg.pitch).fadeAmount;
                float curFade = fadeAmount[0];

                Tweenzor::getTween(fadeAmount)->remove();
                Tweenzor::add(fadeAmount, curFade, 0.f, 0.f, _fadeOutTime, EASE_IN_OUT_CUBIC);
                Tweenzor::addCompleteListener(Tweenzor::getTween(fadeAmount), this, &ScanLineScene::onTweenComplete);
            }
        }
        
        void onTweenComplete(float* arg) {
            Tweenzor::getTween(arg)->remove();
        }

        class ScanLine {
            
            public:
                ScanLine(int p, int s, ofColor c, float* f)
                    : pitch(p), speed(s % maxSpeed), color(c), fadeAmount(f) {}

                int pitch;
                int speed;
                float* fadeAmount;
                ofColor color;
            
                int step = 0;
            
                int x = 0;
                int y = 0;
                int height = 20;
            
                static const int maxSpeed = 3;
                static const int maxHeight = 50;
            
        };
    
    private:
    
        std::map<int, ScanLine> _scanLines;
    
        int _scanPos = 0;
        int _scanSpeed = 2;
        float _pulseAmount;
        float _pulsePeriod = 2.f;
        float _fadeInTime = 0.1f;
        float _fadeOutTime = 1.f;
    
        std::vector<ofColor> _colorPalette;
    
};
