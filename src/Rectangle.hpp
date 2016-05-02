//
//  Rectangle.hpp
//  DrawingTool
//
//  Created by Zerc on 4/15/16.
//
//

#ifndef Rectangle_hpp
#define Rectangle_hpp

#include "ofMain.h"
class Rectangle{
public:
    ofParameter<ofVec2f> pos;
    ofParameter<ofVec2f> size;
    ofParameterGroup parameters;
    
    Rectangle(){
        parameters.add(pos.set("pos", ofVec2f(0, 0), ofVec2f(-500, -500), ofVec2f(500, 1000)));
    }
    void draw(){
    }
};
#endif /* Rectangle_hpp */
