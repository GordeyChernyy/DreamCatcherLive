//
//  MeshBrush.hpp
//  StrokeBrush
//
//  Created by Zerc on 4/27/16.
//
//

#ifndef MeshBrush_hpp
#define MeshBrush_hpp

#include "ofMain.h"
#include "ofxAutoReloadedShader.h"
#include "ofxGui.h"
class MeshBrush{
private:
    vector<ofMesh> meshes;
    vector<ofMesh> meshTriangles;
    vector<ofVec3f> pointsL;
    vector<ofVec3f> pointsR;
    ofVec2f pMouse;
    bool isTriangle = false;
    int currMesh = 0;
    int currTriangleMesh = 0;
    float pressure;
    
    ofxAutoReloadedShader shader;
    ofxPanel gui;

    ofVec2f speed;
    float accel;
    float angle;
    
    ofFbo fbo;
    ofxAutoReloadedShader shaderBlur;
    bool isGui = true;
    int w;
    int h;
public:
    ofxFloatSlider s1, s2, s3, s4, s5, s6, s7, s8, s9, s10;
    ofParameter<float> minDist;
    ofParameter<float> maxDist;
    ofParameter<float> brushSize;
    ofParameter<float> blurSize;
    ofParameter<float> blurIteration;
    void setup(int _w, int _h){
        w = _w;
        h = _h;
        shader.load("meshBrushShader/livecoding");
        shader.setMillisBetweenFileCheck(100);
        gui.setup();
        gui.setup("meshBrush", "meshBrush.xml");
        gui.setName("strokeBrush");
        gui.add(s1.setup("s1",0, 0, 1));
        gui.add(s2.setup("s2",0, 0, 1));
        gui.add(s3.setup("s3",0, 0, 1));
        gui.add(s4.setup("s4",0, 0, 1));
        gui.add(s5.setup("s5",0, 0, 1));
        gui.add(s6.setup("s6",0, 0, 1));
        gui.add(s7.setup("s7",0, 0, 1));
        gui.add(s8.setup("s8",0, 0, 1));
        gui.add(s9.setup("s9",0, 0, 1));
        gui.add(s10.setup("s10",0, 0, 1));
        gui.add(minDist.set("minDist", 0.2, 0, 1));
        gui.add(maxDist.set("maxDist", 0.7, 0, 1));
        gui.add(brushSize.set("brushSize", 10, 0, 40));
        
        gui.add(blurSize.set("blurSize", 10, 0, 40));
        gui.add(blurIteration.set("blurIteration", 2, 0, 40));
        gui.loadFromFile("meshBrush.xml");
        
        shaderBlur.load("meshBrushShader/blur");
        shader.setMillisBetweenFileCheck(100);
        
        fbo.allocate(w, h);
        fbo.begin(); ofClear(0, 0); fbo.end();

    }
    void update(){
        ofEnableAlphaBlending();
        fbo.begin();
        ofClear(0, 0);
        shader.begin();
        shader.setUniform1f("time", ofGetElapsedTimef());
        shader.setUniform1f("vol", 1);
        shader.setUniform1f("s1", s1);
        shader.setUniform1f("s2", s2);
        shader.setUniform1f("s3", s3);
        shader.setUniform1f("s4", s4);
        shader.setUniform1f("s5", s5);
        shader.setUniform1f("s6", s6);
        shader.setUniform1f("s7", s7);
        shader.setUniform1f("s8", s8);
        shader.setUniform1f("s9", s9);
        shader.setUniform1f("s10", s10);
        glEnable(GL_BLEND);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        ofSetColor(255, 255);
        ofFill();
        ofSetLineWidth(3);
        for(auto mesh: meshes){
            mesh.draw();
        }
        for(auto mesh: meshTriangles){
            mesh.draw();
        }
        glDisable(GL_BLEND);
        shader.end();
        fbo.end();
    }
    void drawMesh(){
        for(auto mesh: meshes){
            mesh.draw();
        }
        for(auto mesh: meshTriangles){
            mesh.draw();
        }
    }
    void draw(int x, int y, int _w, int _h){
        shaderBlur.begin();
        shaderBlur.setUniform2f("windowSize", w, h);
        shaderBlur.setUniform2f("mouse", x, y);
        shaderBlur.setUniform1f("time", ofGetElapsedTimef());
        shaderBlur.setUniform1f("iteration", blurIteration);
        shaderBlur.setUniform1f("blursize", blurSize);
        ofSetColor(255, 255);
        fbo.draw(0, 0, _w, _h);
        shaderBlur.end();
        
    }
    void drawGui(){
        if(isGui) gui.draw();
    }
    void onPress(){
        if(isTriangle){
            ofMesh mesh;
            mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
            meshTriangles.push_back(mesh);
        }else{
            ofMesh mesh;
            mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
            meshes.push_back(mesh);
        }
    }
    void onDrag(int x, int y, float pressure){
        ofVec2f zero(1,0);
        ofVec2f mouse(x, y);
        angle = zero.angle(mouse-pMouse);
        float smooth = 0.8;
        speed = mouse-pMouse;
        pMouse.set(x, y);
        
        ofNode root;
        ofNode left;
        ofNode right;
        left.setParent(root);
        right.setParent(root);
        float size = pressure*brushSize;
        left.setPosition(-size, 0, 0);
        right.setPosition(size, 0, 0);
        root.setPosition(x, y, 0);
        
        root.setGlobalOrientation(ofQuaternion(angle, ofVec3f(0, 0, 1)));
        
        ofVec3f m(x, y, 0);
        
        if(isTriangle){
            meshTriangles[currTriangleMesh].addVertex(m);
            ofFloatColor col = ofFloatColor(1, 0, 0, pressure);
            col.setHueAngle(pressure*360);
            meshTriangles[currTriangleMesh].addColor(col);
            if(meshTriangles.size()>0){
                for (int j = 0; j < meshTriangles[currTriangleMesh].getVertices().size() ; j++) {
                    ofVec3f h = meshTriangles[currTriangleMesh].getVertex(j);
                    float joinchance = pressure/meshTriangles[currTriangleMesh].getNumVertices() + m.distance(h)/500;
                    if (joinchance > minDist && joinchance < ofRandom(maxDist)) {
                        meshTriangles[currTriangleMesh].removeVertex(j);
                        meshTriangles[currTriangleMesh].addVertex(h);
                        meshTriangles[currTriangleMesh].addColor(col);
                    }
                }
            }
        }else{
            if(meshes.size()>0){
                for (int j = 0; j < meshes[currMesh].getVertices().size() ; j++) {
                    ofVec3f h = meshes[currMesh].getVertex(j);
                    float joinchance = pressure/meshes[currMesh].getNumVertices() + m.distance(h)/500;
                    if (joinchance > 0 && joinchance < ofRandom(maxDist)) {
                        meshes[currMesh].removeColor(j);
                        meshes[currMesh].removeVertex(j);
                        right.setPosition(0, 0, m.distance(h)/2);
                        ofFloatColor col = meshes[currMesh].getColor(j);
                        col.setHueAngle(pressure*100);
                        meshes[currMesh].addVertex(h);
                        meshes[currMesh].addColor(col);
                        
                    }
                }
            }
            
            meshes[currMesh].addVertex(left.getGlobalPosition());
            meshes[currMesh].addColor(ofFloatColor(1, 0, 0, pressure));
            meshes[currMesh].addVertex(right.getGlobalPosition());
            meshes[currMesh].addColor(ofFloatColor(1, 0, 0, pressure));
        }
        ofSetColor(255, 255);
    }
    void onRelease(){
        if(isTriangle){
            currTriangleMesh++;
        }else{
            currMesh++;
        }
    }
    void hideGui(){
        isGui ^= true;
    }
    void resize(int _w, int _h){
        w = _w;
        h = _h;
        fbo.allocate(w, h);
        fbo.begin(); ofClear(0, 0); fbo.end();
    }
    void onMove(int x, int y){
        ofVec2f zero(1,0);
        ofVec2f mouse(x, y);
        angle = zero.angle(mouse-pMouse);
        speed = mouse-pMouse;
        pMouse.set(x, y);
    }
    void switchBrush(){
        isTriangle ^= true;
        if(isTriangle){
            ofMesh mesh;
            mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
            meshTriangles.push_back(mesh);
        }else{
            ofMesh mesh;
            mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
            meshes.push_back(mesh);
        }
    }
    void clear(){
        meshes.clear();
        meshTriangles.clear();
        currTriangleMesh = 0;
        currMesh = 0;
        if(isTriangle){
            ofMesh mesh;
            mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
            meshTriangles.push_back(mesh);
        }else{
            ofMesh mesh;
            mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
            meshes.push_back(mesh);
        }
    }
};

#endif /* MeshBrush_hpp */
