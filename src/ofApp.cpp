#include "ofApp.h"
void ofApp::blendBegin(){
    glEnable(GL_BLEND);
    glBlendFunc(a[aNum], b[bNum]);
    glBlendEquation(GL_FUNC_ADD);
    glBlendColor(1., 1., 1., blendAlpha);
}
void ofApp::blendEnd(){
    glDisable(GL_BLEND);
}
void ofApp::saveImage() {

    // get the raw buffer from ofImage
    ofPixels pix;
    
//    fbo.allocate(canvasWidth, canvasHight);
    canvas.readToPixels(pix);
    recorder.addFrame(pix);
    
}
void ofApp::writeToDisk(){
    

    
//    int counter = 0;
//
//
//    for(auto &f : fbos){
//        
//        ofPixels pix;
//        f.readToPixels(pix);
//        
//        std::stringstream buffer;
//        buffer << setfill('0') << setw(8) << counter;
//        string name = "render/" +folderName + "/frame_" + buffer.str() + ".png";
//        
//        recorder.addFrame(pix);
//        
//        
//        counter++;
//    }
//    fbos.clear();

    
}
void ofApp::makeMovie(){
    string terminalCommand;
    
    ofDirectory dir(curFolderPath);
    ofDirectory renderPath("render");
    
    terminalCommand.append("cd "+ dir.getAbsolutePath() + "\n");
    terminalCommand.append("ffmpeg -i frame_%04d.tga -r 60 -vf scale=640:-1 -vcodec mjpeg -qscale 8 "
                           + renderPath.getAbsolutePath()
                           + "/"
                           + curFileName
                           + ".mov" + "\n");
    
    ofLog() << terminalCommand;
//    terminalCommand.append("git push origin master");
    
    ofSystem(terminalCommand);
}
//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    
    float sf = 0.3;
    canvasWidth *= sf;
    canvasHight *= sf;
    
    ofLog() << "canvasWidth : " <<canvasWidth;

    // audio
    soundStream.printDeviceList();
    //if you want to set a different device id
    //soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
    int bufferSize = 8;
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);
    bufferCounter	= 0;
    drawCounter		= 0;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    sound.load("sound.wav");

    // kinect && flowtools
    kinect1.setRegistration(true);
    kinect1.init(false, false);
    kinect1.open(0);
    kinect2.setRegistration(true);
    kinect2.init(false, false);
    kinect2.open(1);
    flow.setup(win1W, win1H, 1.5);
    
//    ofSetWindowPosition(0, 0);
    winFbo.allocate(win1W, win1H);
    winFbo.begin(); ofClear(0, 0); winFbo.end();
    
    //fog.setup("fog");
    fog.gui.setPosition(100, 20);
    flower.setup();
    // cam setup
    cam.disableMouseInput();
    cam.setAutoDistance(false);
    cam.setParent(camTarget);
    cam.setPosition(0, 0, 800);
    camTarget.setPosition(0, 0, 0);
    
    win2Fbo.allocate(win2W, win1H);
    win2Fbo.begin(); ofClear(0, 0); win2Fbo.end();
    meshBrush.setup(win1W, win1H);
    kaleidoscope.setup();
    brush.setup();
    movingFbo.setup();
    brushTr.setup();
    korg.setup();
    light.load("light.png");
    
    stageParam.setName("stageParam");
    stageParam.add(isFlower.set("isFlower", true));
    stageParam.add(maxVolume.set("maxVolume", 0.02, -0.0004, 1.9));
    stageParam.add(farClip.set("farClip", 1000, 0, 12000));
    stageParam.add(farClip2.set("farClip2", 1000, 0, 12000));

    stageParam.add(brushMode.set("brushMode", 0, 0, 2));
    stageParam.add(showInfo.set("showInfo", true));
    stageParam.add(lightPos.set("lightPos", ofVec2f(0, 0), ofVec2f(-500, -500), ofVec2f(500, 1000)));
    stageParam.add(lightSmooth.set("lightSmooth", 0.02, 0., 1.));
    stageParam.add(lightOpacity.set("lightOpacity", 255, 0, 255));
    stageParam.add(lightScale.set("lightScale", 0.02, 0., 1.));
    stageParam.add(enableLight.set("enableLight", true));
    stageParam.add(enableMouse.set("enableMouse", true));
    stageParam.add(enableKaleidoscope.set("enableKaleidoscope", false));
    stageParam.add(enableMovingFbo.set("enableMovingFbo", false));
    stageParam.add(pointerSize.set("pointerSize", 5., 0., 10.));
    stageParam.add(pointerColor.set("pointerColor", ofColor(255, 255), ofColor(0, 0), ofColor(255, 255)));
    stageParam.add(aNum.set("aNum", 1, 0, 13));
    stageParam.add(bNum.set("bNum", 1, 0, 13));
    stageParam.add(blendAlpha.set("blendAlpha", 1, 0, 1));
    stageParam.add(bgPos.set("bgPos", ofVec2f(0, 0), ofVec2f(-500, -500), ofVec2f(500, 1000)));
    stageParam.add(bgScale.set("bgScale", 1, 0.5, 5));


    parameters.setName("parameters");
    parameters.add(stageParam);
    parameters.add(brush.parameters);
    parameters.add(brushTr.parameters);
    parameters.add(movingFbo.parameters);
    parameters.add(kaleidoscope.parameters);
    parameters.add(flower.parameters);
    
    gui.setup(parameters);
    gui.loadFromFile("settings.xml");
    gui.minimizeAll();
    vector<string> names = gui.getControlNames();
    
    // set gui headers color
    for (int i = 0; i<gui.getNumControls(); i++) {
        ofColor col = ofColor(0, 162, 208);
        gui.getGroup(names[i]).setHeaderBackgroundColor(col);
        gui.getGroup(names[i]).setTextColor(ofColor(0));
        gui.getGroup(names[i]).setBorderColor(col);
    }
    
    ofxTablet::start();
    ofAddListener(ofxTablet::tabletEvent, this, &ofApp::tabletMoved);
    ofBackground(0);
    drag = false;
    currentParameter = 0;
    showGui = true;
    canvas.allocate(canvasWidth, canvasHight);
    canvas.begin(); ofClear(0, 0); canvas.end();
    font.loadFont("Arial.ttf", 12);
    bg.loadImage("/Users/gordey/Projects/LaurynHill/lauryn.jpg");
    
    
}

//--------------------------------------------------------------
void ofApp::updateSaving(){
    if(recorder.isThreadRunning()){
        ofSetColor(255);
        
        stringstream c;
        c << "\nQueue Size: " << recorder.q.size() << endl;
        
        ofDrawBitmapString(c.str(), ofGetWidth()/2, ofGetHeight()/2);

        if(recorder.q.size()==0){
            recorder.stopThread();
            makeMovie();
        }
    }
}
void ofApp::update(){
    korg.update();
    meshBrush.update();
    meshBrush.s2 = ofMap(korg.sliders[5], 0, 127, 0, 10);
    meshBrush.s1 = ofMap(korg.sliders[5], 0, 127, 0, 10);
    meshBrush.s3 = ofMap(korg.sliders[5], 0, 127, 0, 30);
    meshBrush.blurSize = ofMap(korg.knobs[5], 0, 127, 0, 100);

    isFlower = korg.buttonsSolo[3];
    if(isFlower) flower.update(0, 0, 0);
    
    if(korg.knobs[6]==127){
        meshBrush.clear();
    }
    
    enableKaleidoscope = korg.buttonsSolo[0];
    enableMovingFbo = korg.buttonsSolo[1];
    brushMode = ofMap(korg.sliders[7], 0, 127, 0, 2);
    enableBg = korg.buttonsRec[7];
    
    // korg: change colors
    int col = ofMap(korg.sliders[6], 0, 127, 0, 3);
    brush.changeColor(col);
    
    // korg: fade to black
    float fade = ofMap(korg.knobs[7], 0, 127, 0, 50);
    
    canvas.begin();
    ofEnableAlphaBlending();
    ofSetColor(0, fade);
    ofRect(0, 0, canvasWidth, canvasHight);
    ofDisableAlphaBlending();
    canvas.end();
    
//    if (korg.buttonsRec[7]) {
//        movingFbo.resize();
//    }
    
    // korg: kaleidoscope
    mapParameter(korg.knobs[0], kaleidoscope.s4); // distort x
    mapParameter(korg.knobs[1], kaleidoscope.s5); // distort y
    mapParameter(korg.knobs[2], kaleidoscope.s6); // offset x
    mapParameter(korg.knobs[3], kaleidoscope.s7); // offset y
    // light
    mapParameter(korg.sliders[7], lightOpacity); // distort x
    
    if (enableMovingFbo) movingFbo.update();
    
    // movingFbo -+
    //            +-->  kaleidoscope shader
    // canvas ----+
    if (enableKaleidoscope) {
        if (enableMovingFbo) {
            kaleidoscope.update(movingFbo.getCurrentFrame(), mouseX, mouseY);
        } else {
            kaleidoscope.update(canvas, mouseX, mouseY);
        }
    }
    if(isRender){
        saveImage();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    winFbo.begin();
    ofClear(0, 0);
    
    ofDisableAlphaBlending();

    if(enableBg){
        blendBegin();
    }
    if (enableBg) {
        bg.draw(bgPos->x, bgPos->y, bg.getWidth()*bgScale, bg.getHeight()*bgScale);
    }
//    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    // flow.draw();
    ofSetColor(255, 255);
    if(enableKaleidoscope){
        kaleidoscope.draw(win1W, win1H);
    }else{
        if (enableMovingFbo) {
            movingFbo.draw(win1W, win1H);
        }else{
            int w = win1W;
            float a = (float)canvasHight/(float)canvasWidth;
            float h = win1W*a;
            canvas.draw(0, 0, w, h);               // Canvas can accept graphics from all type of static brushes
            switch (brushMode) {             // Draw any elements out of canvas on top
                case 0:                      // Dream Catcher Brush
                    break;
                case 1:                      // Triangle Brush
                    if(drag) brushTr.draw(); // This will draw white triangle
                    break;
                case 2:
                    meshBrush.draw(mouseX, mouseY, win1W, win1H);
                    break;
                default:
                    break;
            }
        }
    }
    ofSetColor(255, 255);
    if (isFlower) {
        ofEnableDepthTest();
        ofEnableAlphaBlending();
        cam.begin();
        fog.enable();
        flower.draw();
        fog.disable();
        cam.end();
        ofDisableAlphaBlending();
        ofDisableDepthTest();
    }
    if(enableBg){
        blendEnd();
    }
    ofDisableBlendMode();
    winFbo.end();
    
    
    winFbo.draw(0, 0, win1W, win1H);
    winFbo.draw(win1W, 0, win2W, win2H);

    
    
    
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);

    if (showGui) {
        gui.draw();
        meshBrush.drawGui();
        fog.drawGui();
        if (showInfo) {
            info();
        }
        flow.drawGui();
    }
    ofFill();
    ofSetColor(pointerColor);
    //ofCircle(mouseX, mouseY, pointerSize);
    glDisable(GL_BLEND);
    glPopAttrib();

    string m;
    m = isRender ? "Recording..." : "";
    m.append("\n");
    m.append(ofToString(ofGetFrameRate()));
    ofDrawBitmapString(m, 20, 20);
    
    updateSaving();

}
void ofApp::info(){
    string b = "";
    switch (brushMode) {
        case 0:
            b = "Dream Catcher Brush";
            break;
        case 1:
            b = "Triangle Brush";
            break;
            
        default:
            break;
    }
    string s =
    "-- KEY ----------------------------------\n";
    s.append("1, 2, 3, 4    change colors\n");
    s.append("TAB           hide gui\n");
    s.append("q, w          cycle through parameters\n");
    s.append("SPACE         clear image\n");
    s.append("e             triangle brush\n");
    s.append("r             dream catcher brush\n");
    s.append("-- INFO ---------------------------------\n");
    s.append("fps           "+ ofToString(ofGetFrameRate()) +"\n");
    s.append("history size  "+ ofToString(brush.history.size()) +"\n");
    s.append("brush         "+ b +"\n");
    s.append("-- TIPS ---------------------------------\n");
    s.append("Try to enable movingFbo and Kaleidoscope.\n");
    s.append("It looks interesting if you change color \n");
    s.append("and draw at the same time.\n");
    s.append("Kaleidoscope is still in development. Po-\n");
    s.append("sition of drawing is not matching. Try \n");
    s.append("to draw more on the left top corner.\n");
    s.append("To use tablet properly you need disable  \n");
    s.append("mouse. Tablet data more precise than mo- \n");
    s.append("use data.\n");
    
    ofSetColor(0, 255); // shadow
    ofDrawBitmapString(s, gui.getWidth()+41, 21);
    ofSetColor(255, 255);
    ofDrawBitmapString(s, gui.getWidth()+40, 20);
    ofDisableAlphaBlending();
    
    ofPushMatrix();
    ofTranslate(0,ofGetHeight()/2);
    korg.draw();
    ofPopMatrix();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'r'){
        string folderName = ofSystemTextBoxDialog("folder name: ");
        curFileName =folderName;
        curFolderPath = "render/" + folderName;
        recorder.setPrefix(ofToDataPath("render/" + folderName + "/frame_")); // this directory must already exist
        recorder.setFormat("tga"); // png is really slow but high res, bmp is fast but big, jpg is just right
        
        recorder.startThread(false);
        isRender = true;
    }
    if(key == 's'){
        isRender = false;
    }
    if(key == 'd'){
        writeToDisk();
    }
    switch (key) {
        case 'u':
            flow.isKinect2 ^= true;
            break;
        case 'y':
            flow.isKinect1 ^= true;
            break;
        case 't':
            flow.keyPressed('C');
            break;
        case 'c':
            clearImage();
            break;
        case 'n':
            flower.newShape();
            break;
        case 'm':
            if(flower.isMesh){
                flower.isMesh = false;
                flower.isPoly = true;
            }else{
                flower.isMesh = true;
                flower.isPoly = false;
            }
            break;
        case '0':
            ofSetWindowPosition(0, 0);
            ofSetWindowShape(win1W+win2W, 800);
            break;
        case '1':
            brush.changeColor(0);
            break;
        case '2':
            brush.changeColor(1);
            break;
        case '3':
            brush.changeColor(2);
            break;
        case '4':
            brush.changeColor(3);
            break;
        case 'r':
            brushMode = 0;
            break;
        case 'e':
            brushMode = 1;
            break;
        case 'f':
            meshBrush.switchBrush();
            break;
        case ' ':
            meshBrush.clear();
            canvas.begin(); ofClear(0, 0); canvas.end();
            brushTr.clear();
            brush.clear();
            movingFbo.resize();
            break;
        case 9: // TAB key
            showGui ^= true;
            meshBrush.hideGui();
            break;
        case 'q':{ //left
            currentParameter--;
            if (currentParameter<0) {
                currentParameter = gui.getNumControls()-1;
            };
            vector<string> names = gui.getControlNames();
            gui.minimizeAll();
            gui.getGroup(names[currentParameter]).maximize();
            cout <<  "name " << names[currentParameter] << endl;
            break;
        }
        case 'w':{ //right
            currentParameter++;
            if (currentParameter>gui.getNumControls()-1) {
                currentParameter = 0;
            }
            vector<string> names = gui.getControlNames();
            gui.minimizeAll();
            gui.getGroup(names[currentParameter]).maximize();
            cout <<  "name " << names[currentParameter] << endl;
            break;
        }
        case 'z':{
            enableBg ^=true;
            break;
        }
        

    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 'c'){
        flow.delFbo();
    }
    if(key == 's'){
        saveImage();
    }
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    meshBrush.onMove(x, y);
}
void ofApp::tabletMoved(TabletData &data) {
    pressure = data.pressure;
//    cout <<  "data.pressure " << data.pressure << endl;
    if (drag && !enableMouse){
        float penX = data.abs_screen[0]*win1W - ofGetWindowPositionX();
        float penYinv = ofMap(data.abs_screen[1], 0, 1, 1, 0);
        float penY = penYinv*win1H - ofGetWindowPositionY();
        float p = data.pressure;
        if(enableMovingFbo){
            int index = movingFbo.currentIndex;
            switch (brushMode) {
                case 0: // Dream Catcher Brush
                    brush.updateCanvas(movingFbo.frames[index], penX, penY, p);
                    break;
                case 1: // Triangle Brush
                    brushTr.updateCanvas(movingFbo.frames[index], penX, penY, brush.activeColor);
                    break;
                case 2: // Triangle Brush
                    meshBrush.onDrag(penX, penY, p);
                    break;
                default:
                    break;
            }
        }else{
            switch (brushMode) {
                case 0:{
                    float px = ofMap(penX, 0, win1W, 0, canvasWidth);
                    float py = ofMap(penY, 0, win1W, 0, canvasWidth);
                    brush.updateCanvas(canvas, px, py, p);
                    break;
                }
                case 1:
                    brushTr.updateCanvas(canvas, penX, penY, brush.activeColor);
                    break;
                case 2: // Triangle Brush
                    meshBrush.onDrag(penX, penY, p);
                    break;
                default:
                    break;
            }
        }
        brushTr.setPressure(p);
    }
}
void ofApp::mapParameter(float midiValue, ofParameter<float> &pValue){
    pValue = ofMap(midiValue, 0, 127, pValue.getMin(), pValue.getMax());
}
void ofApp::mapParameter(int midiValue, ofParameter<int> &pValue){
    pValue = ofMap(midiValue, 0, 127, pValue.getMin(), pValue.getMax());
}
void ofApp::mapParameter(bool midiValue, ofParameter<bool> &pValue){
    pValue = ofMap(midiValue, 0, 127, pValue.getMin(), pValue.getMax());
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(enableMouse){
        drag = true;
        if(enableMovingFbo){
            int index = movingFbo.currentIndex;
            switch (brushMode) {
                case 0: // Dream Catcher Brush
                    brush.updateCanvas(movingFbo.frames[index], mouseX, mouseY, pressure);
                    break;
                case 1: // Triangle Brush
                    brushTr.updateCanvas(movingFbo.frames[index], mouseX, mouseY, brush.activeColor);
                    break;
                case 2: // Triangle Brush
                    meshBrush.onDrag(mouseX, mouseY, pressure);
                    break;
                default:
                    break;
            }
        }else{
            switch (brushMode) {
                case 0:
                    brush.updateCanvas(canvas, mouseX, mouseY, pressure);
                    break;
                case 1:
                    brushTr.updateCanvas(canvas, mouseX, mouseY, brush.activeColor);
                    break;
                case 2: // Triangle Brush
                    meshBrush.onDrag(mouseX, mouseY, pressure);
                    break;
                default:
                    break;
            }
        }
        brushTr.setPressure(pressure);
    }
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    volumeRunOnce = true;
    drag = true;
    meshBrush.onPress();
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    brushTr.clearHistory();
    drag = false;
    brush.clearHistory();
    meshBrush.onRelease();
}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
//    ofSetWindowPosition(0, 0);
//    ofSetWindowShape(win1W+win2W, 873);
}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}
void ofApp::clearImage(){
    flow.setTempFbo(&winFbo);
    meshBrush.clear();
    canvas.begin(); ofClear(0, 0); canvas.end();
    brushTr.clear();
    brush.clear();
    movingFbo.resize();
}
//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
//    cout <<  "input = " << input[0]  << endl;
    if(input[8]>maxVolume){
//        cout << "overload!" << endl;
        isOverload = true;
    }else{
        isOverload = false;
    }
//    float curVol = 0.0;
//    
//    // samples are "interleaved"
//    int numCounted = 0;
//    
//    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
//    for (int i = 0; i < bufferSize; i++){
//        left[i]		= input[i*2]*0.5;
//        right[i]	= input[i*2+1]*0.5;
//        
//        curVol += left[i] * left[i];
//        curVol += right[i] * right[i];
//        numCounted+=2;
//    }
//    
//    //this is how we get the mean of rms :)
//    curVol /= (float)numCounted;
//    
//    // this is how we get the root of rms :)
//    curVol = sqrt( curVol );
//    
//    smoothedVol *= 0.93;
//    smoothedVol += 0.07 * curVol;
//    
//    bufferCounter++;
    
}
void ofApp::dragEvent(ofDragInfo dragInfo){
    bg.loadImage(dragInfo.files[0]);
}
void ofApp::exit(){
    cam.clearParent();
    korg.exit();
    kinect1.close();
    kinect2.close();
}
