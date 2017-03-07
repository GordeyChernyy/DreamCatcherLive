//
//  ofxThreadedImageSaver.hpp
//  DreamCatcherLive
//
//  Created by Elastic Shore on 3/4/17.
//
//

#ifndef ofxThreadedImageSaver_hpp
#define ofxThreadedImageSaver_hpp

#include "ofMain.h"

class ofxThreadedImageSaver : public ofThread, public ofImage {
public:
    string fileName;
    int counter;
    
    ofxThreadedImageSaver(){
        counter=0;
    }
    
    void threadedFunction() {
        if(lock()) {
            save(fileName);
            counter++;
            unlock();
        } else {
            printf("ofxThreadedImageSaver - cannot save %s cos I'm locked", fileName.c_str());
        }
        stopThread();
    }
    
    void saveThreaded(ofFbo fbo) {
        
        std::stringstream buffer;
        buffer << setfill('0') << setw(8) << counter;
        string name = "render/frame_" + buffer.str() + ".png";
        
        this->fileName = name;
        
        ofPixels pix;
        fbo.readToPixels(pix);
        setFromPixels(pix);
        
        startThread(false);   // blocking, verbose
    }  
};

#endif /* ofxThreadedImageSaver_hpp */
