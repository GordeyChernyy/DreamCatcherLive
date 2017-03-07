#pragma once

#include "ofMain.h"
#include "DreamBrush.h"
#include "ofxGui.h"
#include "ofxTablet.h"
#include "Kaleidoscope.h"
#include "FramesFbo.h"
#include "TriangleBrush.h"
#include "korg.h"
#include <GLFW/glfw3.h>
#include <GLUT/GLUT.h>
#include "MeshBrush.hpp"
#include "strokeMaker.hpp"
#include "ofxGLFog.h"
#include "OpticalFlow.hpp"
#include "ofxKinect.h"
#include "ofxThreadedImageSaver.hpp"

class ofApp : public ofBaseApp{
    
public:
		void setup();
		void update();
		void draw();
        void exit();
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void saveImage();
    void clearImage();
    
    // pressure
    float pressure;
    // audio
    void audioIn(float * input, int bufferSize, int nChannels);
    vector <float> left;
    vector <float> right;
    vector <float> volHistory;
    int 	bufferCounter;
    int 	drawCounter;
    float smoothedVol;
    float scaledVol;
    ofSoundStream soundStream;
    ofParameter<float> maxVolume;
    bool volumeRunOnce = true;
    bool isOverload = false;
    
    // optical flow && kinect
    OpticalFlow flow;
    ofxKinect kinect1;
    ofxKinect kinect2;
    ofParameter<float> farClip;
    ofParameter<float> farClip2;
    
    void tabletMoved(TabletData &data);
    strokeMaker flower;
    ofParameter<bool> isFlower;
    bool runOnce = true;
    bool runOnceLast = false;
    
    ofFbo winFbo;
    
    ofxGLFog fog;
    
    // camera
    ofEasyCam cam;
    ofNode camTarget;
    ofNode camPos;
    float camRotX, camRotY, camRotZ;
    float camDistance;
    int korgCamDistance = 127;
    int slider0 = 0, slider1 = 0, slider2 = 0;
    
    MeshBrush meshBrush;
    // map midi value to parameter
    void mapParameter(float midiValue, ofParameter<float> &pValue);
    void mapParameter(int midiValue, ofParameter<int> &pValue);
    void mapParameter(bool midiValue, ofParameter<bool> &pValue);
    void info();
    ofFbo win2Fbo;
    ofParameter<float> bgScale;
    ofParameter<ofVec2f> bgPos;
    ofParameter<ofRectangle> rect;
    int col;
    //---------------------------------------------- MultiWin
    int winWlow = 1024;
    int winHlow = 613;
    int winWmed = 1280;
    int winHmed = 773;
    int winWhigh = 1440;
    int winHhigh = 893;

    int win1W = 1440;
    int win1H = 900;
    
    int canvasWidth = 6480;
    int canvasHight = 1080;
    
    int win2W = 1280;
    int win2H = 800; // 698

    
    GLFWwindow *mainWindow, *auxWindow;
    int auxWidth, auxHeight;
    
    ofImage light;
    int lightPosX;
    ofTrueTypeFont font;
    ofFbo canvas;               // fbo which will be passed to brushes
    DreamBrush brush;           // dream catcher brush
    TriangleBrush brushTr;      // triangle brush
    movingFbo movingFbo;        // moving frames
    Kaleidoscope kaleidoscope;  // shader which distort fbo passed in
    //    KinectWin kinect;
    Korg korg;                  // midi for korg nano kontrol 2 
    
    ofxPanel gui;
    ofxGuiGroup group;
    ofParameterGroup parameters;
    ofParameterGroup stageParam;
    int currentParameter;
    bool showGui;
    bool drag;
    ofParameter<bool> kinectDebug;
    ofParameter<int> brushMode;
    ofParameter<string> brushModeLabel;
    ofParameter<bool> showInfo;
    ofParameter<bool> enableLight;
    ofParameter<float> lightSmooth;
    ofParameter<int> lightOpacity;
    ofParameter<float> lightScale;
    ofParameter<ofVec2f> lightPos;
    ofParameter<bool> enableKaleidoscope;
    ofParameter<bool> enableKinect;
    ofParameter<bool> enableMovingFbo;
    ofParameter<bool> enableMouse; // mouse and tablet has a different resolution! To use mouse 
    ofParameter<float> pointerSize;
    ofParameter<ofColor> pointerColor;
    ofParameter<float> fadeCanvas;

    ofImage bg;
    void blendBegin();
    void blendEnd();
    ofParameter<float> blendAlpha;
    bool enableBg = true;
    ofParameter<int> aNum;
    ofParameter<int> bNum;
    
    GLenum a[13] = {
        GL_ZERO,
        GL_ONE,
        GL_SRC_COLOR,
        GL_ONE_MINUS_SRC_COLOR,
        GL_DST_COLOR,
        GL_ONE_MINUS_DST_COLOR,
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA,
        GL_SRC_ALPHA_SATURATE,
        GL_CONSTANT_COLOR,
        GL_ONE_MINUS_CONSTANT_COLOR,
        GL_CONSTANT_ALPHA,
        GL_ONE_MINUS_CONSTANT_ALPHA
    };
    
    GLenum b[13] = {
        GL_ZERO,
        GL_ONE,
        GL_SRC_COLOR,
        GL_ONE_MINUS_SRC_COLOR,
        GL_DST_COLOR,
        GL_ONE_MINUS_DST_COLOR,
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA,
        GL_SRC_ALPHA_SATURATE,
        GL_CONSTANT_COLOR,
        GL_ONE_MINUS_CONSTANT_COLOR,
        GL_CONSTANT_ALPHA,
        GL_ONE_MINUS_CONSTANT_ALPHA
    };
    
    ofxThreadedImageSaver imgSaver;
    ofImage img;
    bool isRender = false;
    
    vector<ofFbo> fbos;
    void writeToDisk();
    
    ofSoundPlayer sound;
};
