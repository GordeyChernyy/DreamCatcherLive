//
//  strokeMaker.hpp
//  ofNodeLearn
//
//  Created by Zerc on 3/10/16.
//
//

#ifndef strokeMaker_hpp
#define strokeMaker_hpp

#include "ofMain.h"
#include "waver.hpp"
#include "ofxGui.h"

class strokeMaker{
public:
    ~strokeMaker(){
        parentL.clearParent();
        parentR.clearParent();
        childR.clearParent();
        childL.clearParent();
    }
    ofNode parentL;
    ofNode childL;
    ofNode parentR;
    ofNode childR;
    ofNode sphere;
    
    waver childLx;
    waver childLy;
    waver childLz;
    waver childRx;
    waver childRy;
    waver childRz;
    
    float counter;
    float rotation;
    int cycle;
    bool isGrow;
    vector<ofVec3f> pointsL;
    vector<ofVec3f> pointsR;
    
    vector<ofMesh> meshes;
    int currMesh;
    
    ofParameterGroup parameters;
    ofParameter<float> angle;
    ofParameter<float> speed;
    ofParameter<float> sphereAngle;
    
    waver sphereRx;
    waver sphereRy;
    waver sphereRz;
    
    vector<ofPolyline> polysL;
    vector<ofPolyline> polysR;
    ofParameter<bool> isPoly;
    ofParameter<bool> isMesh;
    ofParameter<bool> isDebug;
    ofParameter<bool> isManualRotation;
    int currPoly;
    
    void setup(){
        isGrow = true;
        counter = 0;
        rotation = 0.9;
        cycle = 0;
        parameters.setName("strokeMaker");
        parameters.add(angle.set("angle", 0.1, 0, 10));
        parameters.add(sphereAngle.set("sphereAngle", 0, 0, 360));
        parameters.add(speed.set("speed", 0.1, 0, 0.2));
        parameters.add(isPoly.set("isPoly", true));
        parameters.add(isMesh.set("isMesh", true));
        parameters.add(isDebug.set("isDebug", true));
        parameters.add(isManualRotation.set("isManualRotation", true));
        
        childLx.setup("childLx", 40, -300, 300);
        parameters.add(childLx.parameters);
        childLy.setup("childLy", 40, -300, 300);
        parameters.add(childLy.parameters);
        childLz.setup("childLz", 40, -300, 300);
        parameters.add(childLz.parameters);

        childRx.setup("childRx", 40, -300, 300);
        parameters.add(childRx.parameters);
        childRy.setup("childRy", 40, -300, 300);
        parameters.add(childRy.parameters);
        childRz.setup("childRz", 40, -300, 300);
        parameters.add(childRz.parameters);

        sphereRx.setup("sphereRx", 0.2, 0.01, 2);
        parameters.add(sphereRx.parameters);
        sphereRy.setup("sphereRy", 0.2, 0.01, 2);
        parameters.add(sphereRy.parameters);
        sphereRz.setup("sphereRz", 0.2, 0.01, 2);
        parameters.add(sphereRz.parameters);
        
        angle = 5;
        parentL.setParent(sphere);
        parentR.setParent(sphere);
        
        childL.setParent(parentL);
        childR.setParent(parentR);
        
        currMesh = 0;
        ofMesh m;
        m.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        meshes.push_back(m);

        currPoly = 0;
        ofPolyline p;
        ofPolyline p2;
        polysL.push_back(p);
        polysR.push_back(p2);
        
        parentL.setPosition(0, 0, 300);
        childL.setPosition(40, 0, 0);
        
        parentR.setPosition(0, 0, 300);
        childR.setPosition(40, 40, 0);
    }
    void update(int x, int y, int z){
        sphere.setGlobalPosition(x, y, z);
        sphereRx.update(counter);
        sphereRy.update(counter);
        sphereRz.update(counter);

//        sphere.rotate(sphereRx.p(), 1, 0, 0);
//        sphere.rotate(sphereRy.p(), 0, 1, 0);
        sphere.rotate(sphereRz.p(), 0, 1, 0);
        
        childLx.update(counter);
        childLy.update(counter);
        childLz.update(counter);
        childRx.update(counter);
        childRy.update(counter);
        childRz.update(counter);
        
        childL.setPosition(childLx.p(), childLy.p(), childLz.p());
        childR.setPosition(childRx.p(), childRy.p(), childRz.p());
        
        parentL.rotate(cos(counter)*angle, 0, 0, 1);
        parentR.rotate(cos(counter)*angle, 0, 0, 1);
        
        pointsL.push_back(childL.getGlobalPosition());
        pointsR.push_back(childR.getGlobalPosition());
       
        if(isGrow){
            if(isMesh){
                meshes[currMesh].addVertex(childL.getGlobalPosition());
                meshes[currMesh].addVertex(childR.getGlobalPosition());
            }
            if(isPoly){
                polysL[currPoly].addVertex(childL.getGlobalPosition());
                polysR[currPoly].addVertex(childR.getGlobalPosition());
            }
        }

        counter += speed;
        if(counter >270){
            isGrow = false;
        }
    }
    void debug(){

        ofSetColor(255, 255, 255);
        ofSetLineWidth(20);
        ofCircle(sphere.getGlobalPosition(), 3);
        ofLine(sphere.getGlobalPosition(), parentL.getGlobalPosition());
        
        ofCircle(parentL.getGlobalPosition(), 3);
        ofLine(parentL.getGlobalPosition(), childL.getGlobalPosition());
        ofCircle(childL.getGlobalPosition(), 3);
        
        ofLine(parentR.getGlobalPosition(), childR.getGlobalPosition());
        ofCircle(childR.getGlobalPosition(), 3);
    }
    void draw(){
        ofPushMatrix();
        ofRotateX(-90);
        if(isMesh){
            for(auto mesh: meshes){
                mesh.draw();
            }
        }
        if(isPoly){
            ofSetLineWidth(3);
            for(auto poly: polysL){
                poly.draw();
            }
            for(auto poly: polysR){
                poly.draw();
            }
        }
        if(isDebug){
            debug();
        }
        ofPopMatrix();
    }
    void newShape(){
        clear();
        angle.set(ofRandom(0, 10));
        float interval = 150;
        childLx.amp = ofRandom(-interval, interval);
        childLy.amp = ofRandom(-interval, interval);
        childLy.amp = ofRandom(-interval, interval);
        childRx.amp = ofRandom(-interval, interval);
        childRy.amp = ofRandom(-interval, interval);
        childRy.amp = ofRandom(-interval, interval);
        childLx.sineSwitch = ofRandom(0, 4);
        childLy.sineSwitch = ofRandom(0, 4);
        childLz.sineSwitch = ofRandom(0, 4);
        childRx.sineSwitch = ofRandom(0, 4);
        childRy.sineSwitch = ofRandom(0, 4);
        childRz.sineSwitch = ofRandom(0, 4);
        if(isMesh){
            ofMesh m;
            m.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
            meshes.push_back(m);
            currMesh++;
        }

        if(isPoly){
            ofPolyline p;
            ofPolyline p2;
            polysL.push_back(p);
            polysR.push_back(p2);
            currPoly++;
        }
        isGrow = true;
        counter = 0;
    }
    void clear(){
        meshes.clear();
        currMesh = 0;

        ofMesh m;
        m.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        meshes.push_back(m);

        polysL.clear();
        polysR.clear();
        ofPolyline p;
        ofPolyline p2;
        polysL.push_back(p);
        polysR.push_back(p2);
        currPoly = 0;
    }
    void toggle(){
        isGrow ^= true;
    }
    
    
};

#endif /* strokeMaker_hpp */
