/*
 *
 */
#pragma once

#include "MidiScene.h"

#include "ofPolyline.h"

class NautilusScene : public MidiScene {
        
    public:
    
        NautilusScene(ofxMidiIn &midiInPtr) : MidiScene("Nautilus", midiInPtr) {
            setFade(5000, 1000);
        }

        // scene setup
        void setup() {
            MidiScene::setup();
            
            // build a color palette of pleasant colors to toggle through
            _colorPalette.push_back(ofColor(255, 133, 177));
            _colorPalette.push_back(ofColor(247, 79, 140));
            _colorPalette.push_back(ofColor(241, 29, 107));
            _colorPalette.push_back(ofColor(133, 206, 255));
        }
        
        // normal update
        void update() {
            // -------------------------------------------------------------------------
            // calculate excitation based on current highest pitch being played
            // excitation is speed the peaks and troughs of the wave will move toward the maximum
            float excitation = 0.f;

            if (_pitches.size() > 0) {
                excitation = _pitches.back()/50.f;
            }
            // -------------------------------------------------------------------------

            _path.clear();
            
            float y;
            float sign = _dir;
            
            // no y values yet, initialize and use them
            if (_ys.size() == 0) {
                for (float x = 0; x < ofGetWidth(); x += ofGetWidth()/_xOffset) {
                    y = 0;
                    _ys.push_back(y);
                    
                    _path.curveTo(ofPoint(x, y));
                    
                    sign *= -1;
                }
            }
            // modify the existing y values
            else {
                int i = 0;
                float yAbsMax = 0;
                for (float x = 0; x < ofGetWidth(); x += ofGetWidth()/_xOffset) {
                    _ys[i] += excitation * sign;
                    _ys[i] = ofClamp(_ys[i], -1*_yAbsMax, _yAbsMax);
                                        
                    _path.lineTo(ofPoint(x, _ys[i]));
                    
                    // keep track of the current max y value so we can change direction when appropriate
                    if (glm::abs(_ys[i]) >= yAbsMax) {
                        yAbsMax = glm::abs(_ys[i]);
                    }
                    
                    sign *= -1;
                    ++i;
                }
                
                if (yAbsMax >= _yAbsMax) {
                    changeDirection();
                }
            }
        }

        // draw
        void draw() {
            int i = 0;

            ofSetLineWidth(100.f);
            
            ofPushMatrix();

            ofTranslate(ofGetWidth()/2, ofGetHeight()/2, -1000);

            while (i < ofGetHeight()) {
                ofColor c1 = _colorPalette[_curColorIdx];
                ofColor c2 = _colorPalette[(_curColorIdx+1)%_colorPalette.size()];

                c1 = c1.lerp(c2, ofNormalize(i, 0, ofGetHeight())); // compute the color blend
                c1.a *= alpha; // alpha is inherited from ofxFadeScene and controls the fade when entering/exiting scenes

                ofSetColor(c1);
                
                ofRotateDeg(ofGetElapsedTimef(), 0, 0, 5);

                _path.draw();
                _path.translate(*(new glm::vec3(0, _yOffset, 0)));
                i += _yOffset;
            }
            
            ofPopMatrix();
        }
        
        // cleanup
        void exit() {
            MidiScene::exit();
            _ys.clear();
            _pitches.clear();
        }
    
        // handle incoming midi messages
        void newMidiMessage(ofxMidiMessage &msg) {
//            if (msg.channel == 1) { // handle midi input on midi channel 1
//                if (msg.status == MIDI_NOTE_ON) {
//                    _pitches.push_back(msg.pitch);
//                }
//                else if (msg.status == MIDI_NOTE_OFF) {
//                    for (auto it = _pitches.cbegin(); it != _pitches.cend() ; ) {
//                        if (*it == msg.pitch) {
//                            it = _pitches.erase(it);
//                        }
//                        else {
//                            ++it;
//                        }
//                    }
//                }
//            }
//            else
            if (msg.channel == 1) { // handle midi input on midi channel 1
                if (msg.status == MIDI_NOTE_ON) {
                    _curColorIdx = (_curColorIdx + 1) % _colorPalette.size();
                }
            }

            // sort the vector of pitches so the highest pitch will be at the top
            std::sort(_pitches.begin(), _pitches.end());
        }
        
        void changeDirection() {
            _dir *= -1;
        }
    
    private:
    
        ofPolyline _path;           // the wave
        std::vector<int> _pitches;  // set of current active midi pitches
        std::vector<float> _ys;     // y values along the wave
        float _dir = 1;             // wavy direction, determined by sign
        float _yAbsMax = 10;        // maximum amplitude of waves
        float _xOffset = 20;        // distance between points on the x axis
        float _yOffset = 10;        // distance between waves on the y axis
    
        float _speed = 0;
    
        bool _animate = true;
    
        std::vector<ofColor> _colorPalette;
        int _curColorIdx = 0;

};


