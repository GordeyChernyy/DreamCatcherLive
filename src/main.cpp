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
    
	ofSetupOpenGL(win1W+win2W,873,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
}
