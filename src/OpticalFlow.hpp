//
//  OpticalFlow.hpp
//  KinectFluid
//
//  Created by Zerc on 4/1/16.
//
//

#ifndef OpticalFlow_hpp
#define OpticalFlow_hpp

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxFlowTools.h"
#include "ofxKinect.h"
using namespace flowTools;

class OpticalFlow{
private:
    int w, h;
    enum drawModeEnum{
        DRAW_COMPOSITE = 0,
        DRAW_FLUID_DENSITY,
        DRAW_PARTICLES,
        DRAW_VELDOTS,
        DRAW_FLUID_FIELDS,
        DRAW_FLUID_VELOCITY,
        DRAW_FLUID_PRESSURE,
        DRAW_FLUID_TEMPERATURE,
        DRAW_FLUID_DIVERGENCE,
        DRAW_FLUID_VORTICITY,
        DRAW_FLUID_BUOYANCY,
        DRAW_FLUID_OBSTACLE,
        DRAW_FLOW_MASK,
        DRAW_OPTICAL_FLOW,
        DRAW_SOURCE,
        DRAW_MOUSE
    };
    // Camera
    bool				didCamUpdate;
    ftFbo				cameraFbo;
    ofParameter<bool>	doFlipCamera;
    
    // Time
    float				lastTime;
    float				deltaTime;
    
    // FlowTools
    int					flowWidth;
    int					flowHeight;
    int					drawWidth;
    int					drawHeight;
    
    ftOpticalFlow		opticalFlow;
    ftVelocityMask		velocityMask;

    ftParticleFlow		particleFlow;
    
    ftVelocitySpheres	velocityDots;
    
    ofImage				flowToolsLogoImage;
    bool				showLogo;
    
    // MouseDraw
    ftDrawMouseForces	mouseForces;
    
    // Visualisations
    ofParameterGroup	visualizeParameters;
    ftDisplayScalar		displayScalar;
    ftVelocityField		velocityField;
    ftTemperatureField	temperatureField;
    ftPressureField		pressureField;
    ftVTField			velocityTemperatureField;
    
    ofParameter<bool>	showScalar;
    ofParameter<bool>	showField;
    ofParameter<float>	displayScalarScale;
    void				setDisplayScalarScale(float& _value) { displayScalar.setScale(_value); }
    ofParameter<float>	velocityFieldScale;
    void				setVelocityFieldScale(float& _value) { velocityField.setVelocityScale(_value); velocityTemperatureField.setVelocityScale(_value); }
    ofParameter<float>	temperatureFieldScale;
    void				setTemperatureFieldScale(float& _value) { temperatureField.setTemperatureScale(_value); velocityTemperatureField.setTemperatureScale(_value); }
    ofParameter<float>	pressureFieldScale;
    void				setPressureFieldScale(float& _value) { pressureField.setPressureScale(_value); }
    ofParameter<bool>	velocityLineSmooth;
    void				setVelocityLineSmooth(bool& _value) { velocityField.setLineSmooth(_value); velocityTemperatureField.setLineSmooth(_value);  }
    
    // GUI
    ofxPanel			gui;
    void				setupGui();
    ofParameter<bool>	toggleGuiDraw;
    ofParameter<float>	guiFPS;
    ofParameter<float>	guiMinFPS;
    deque<float>		deltaTimeDeque;
    ofParameter<bool>	doFullScreen;
    void				setFullScreen(bool& _value) { ofSetFullscreen(_value);}
    
    // DRAW
    ofParameter<bool>	doDrawCamBackground;
    
    ofParameter<int>	drawMode;
    void				drawModeSetName(int& _value) ;
    ofParameter<string> drawName;
    
    void				drawComposite()			{ drawComposite(0, 0, w, h); }
    void				drawComposite(int _x, int _y, int _width, int _height);
    void				drawParticles()			{ drawParticles(0, 0, w, h); }
    void				drawParticles(int _x, int _y, int _width, int _height);
    void				drawFluidFields()		{ drawFluidFields(0, 0, w, h); }
    void				drawFluidFields(int _x, int _y, int _width, int _height);
    void				drawFluidDensity()		{ drawFluidDensity(0, 0, w, h); }
    void				drawFluidDensity(int _x, int _y, int _width, int _height);
    void				drawFluidVelocity()		{ drawFluidVelocity(0, 0, w, h); }
    void				drawFluidVelocity(int _x, int _y, int _width, int _height);
    void				drawFluidPressure()		{ drawFluidPressure(0, 0, w, h); }
    void				drawFluidPressure(int _x, int _y, int _width, int _height);
    void				drawFluidTemperature()	{ drawFluidTemperature(0, 0, w, h); }
    void				drawFluidTemperature(int _x, int _y, int _width, int _height);
    void				drawFluidDivergence()	{ drawFluidDivergence(0, 0, w, h); }
    void				drawFluidDivergence(int _x, int _y, int _width, int _height);
    void				drawFluidVorticity()	{ drawFluidVorticity(0, 0, w, h); }
    void				drawFluidVorticity(int _x, int _y, int _width, int _height);
    void				drawFluidBuoyance()		{ drawFluidBuoyance(0, 0, w, h); }
    void				drawFluidBuoyance(int _x, int _y, int _width, int _height);
    void				drawFluidObstacle()		{ drawFluidObstacle(0, 0, w, h); }
    void				drawFluidObstacle(int _x, int _y, int _width, int _height);
    void				drawMask()				{ drawMask(0, 0, w, h); }
    void				drawMask(int _x, int _y, int _width, int _height);
    void				drawOpticalFlow()		{ drawOpticalFlow(0, 0, w, h); }
    void				drawOpticalFlow(int _x, int _y, int _width, int _height);
    void				drawSource()			{ drawSource(0, 0, w, h); }
    void				drawSource(int _x, int _y, int _width, int _height);
    void				drawMouseForces()		{ drawMouseForces(0, 0, w, h); }
    void				drawMouseForces(int _x, int _y, int _width, int _height);
    
    void				drawVelocityDots()		{ drawVelocityDots(0, 0, w, h); }
    void				drawVelocityDots(int _x, int _y, int _width, int _height);
    
public:
    ftFluidSimulation	fluidSimulation;
    bool isTempFbo = false;
    bool isKinect2 = false;
    bool isKinect1 = true;
    ofFbo *fboTemp;
    void setTempFbo(ofFbo *fbo){
        fboTemp = fbo;
        isTempFbo = true;
    };
    void delFbo(){isTempFbo = false;};
    void setup(int winW, int winH,  float scale);
    void update(ofxKinect *kinect1, ofxKinect *kinect2);
    void drawGui();

    void draw();
    void keyPressed(int key);
};

#endif /* OpticalFlow_hpp */
