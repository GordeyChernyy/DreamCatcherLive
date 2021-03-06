#include "ofMain.h"
#include "ofxGui.h"

class Kaleidoscope{
private:
    void setUniform(int x, int y);
    ofShader shader;
public:
    ofFbo fboCanvas, fboFilter;
    void setup();
    void update(ofFbo fbo, int x, int y);
    void draw(int w, int h);
    void begin();
    void end();
    void clear();
    void resize();
    ofParameterGroup parameters;
    ofParameter<float> s1, s2, s3, s4, s5, s6, s7;
    ofParameter<ofVec2f> pos;
};
