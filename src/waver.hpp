//
//  waver.hpp
//  FogGL
//
//  Created by Zerc on 3/12/16.
//
//

#ifndef waver_hpp
#define waver_hpp

#include <stdio.h>
#include "ofMain.h"
class waver{
public:
    ofParameterGroup parameters;
    ofParameter<int> sineSwitch;
    ofParameter<float> amp;
    float counter; 
    void setup(string name, float value, float min, float max){
        counter = 0;
        parameters.setName(name);
        parameters.add(sineSwitch.set("sineSwitch", 0, 0, 4));
        parameters.add(amp.set("amp", value, min, max));
    }
    void update(float _counter){
        counter = _counter;
    }
    float p(){
        switch (sineSwitch) {
            case 0:
                return amp;
                break;
            case 1:
                return cos(counter)*amp;
                break;
            case 2:
                return sin(counter)*amp;
                break;
            case 3:
                return sin(abs(counter-amp))*amp;
                break;
            case 4:
                return cos(sin(counter+amp))*amp;
                break;
            default:
                return cos(counter)*amp;
                break;
        }
    }
};
#endif /* waver_hpp */
