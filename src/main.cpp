#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main(){
    
// larger 1024, 613
// medium 1280, 773
// default 1440, 873
    int win1W = 1440;
    int win1H = 893;
    
    int win2W = 1280;
    int win2H = 720; // 698
    
    ofGLFWWindowSettings settings;
    
    settings.width = win1W+win2W; // 1280; // 
    settings.height = 800;
    settings.setPosition(ofVec2f(0,0)); //1440, 0
    settings.resizable = true;
//    settings.decorated = false;
    
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    
    shared_ptr<ofApp> mainApp(new ofApp);
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
}
