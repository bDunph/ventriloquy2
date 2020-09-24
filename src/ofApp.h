#pragma once

#include <ofMain.h>
#include <ofxMaxim.h>
#include <maximilian.h>
#include <ofxGui.h>
#include <ofxXmlSettings.h>
#include <ofxMidi.h>

#include <stdio.h>

#include <time.h>
#include <assert.h>

//#include "ofxSyphon.h"
//#include "ofxCv.h"
//#include "ofxOpenCv.h"
//#include "opencv2/core/core_c.h"
//#include "opencv2/highgui/highgui_c.h"

#include "avObject.hpp"
//#include "psMoveTrackerClass.hpp"
#include "controller.hpp"

//#define PREPMODE 3
//#define PERFORMANCEMODE 5
#define NUMAVS 3
//#define PSMOVE_ON 1
//#define PSMOVE_OFF 2
#define PORT 8000

class ofApp : public ofBaseApp {
    
public:
    void setup();
    void exit();
    void update();
    void draw();
    void globalRandParams();
    //******** Maxim **********//
    void audioOut(float * output, int bufferSize, int nChannels);
    
    
    void globalReadWrite();
    void globalLoadExamples(int objectNumber);
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    //********** Global Params **************//
    
    ofxPanel camGui;
    ofxPanel lightGui;
    ofxPanel volGui;
    
    ofxFloatSlider masterVol;
    
    ofxFloatSlider camX;
    ofxFloatSlider camY;
    ofxFloatSlider camZ;
    
    ofxFloatSlider lightColR;
    ofxFloatSlider lightColG;
    ofxFloatSlider lightColB;
    ofxFloatSlider ambColR;
    ofxFloatSlider ambColG;
    ofxFloatSlider ambColB;
    ofxFloatSlider specColR;
    ofxFloatSlider specColG;
    ofxFloatSlider specColB;
    ofxFloatSlider lightPow;
    ofxFloatSlider lightPosX;
    ofxFloatSlider lightPosY;
    ofxFloatSlider lightPosZ;
    ofxFloatSlider blurAmnt;
    ofVec3f lightCol;
    ofVec3f ambCol;
    ofVec3f specCol;
    ofVec3f lightPos;

    int objectNumber;
    //************ Visuals **********//
    
    bool drawGui;
    ofEasyCam cam;
    ofFbo fbo1;
    ofFbo fbo2;
    ofShader blurX;
    ofShader blurY;
    ofVec3f camPos;
    ofMatrix4x4 cameraSpace;
    //*********** Maxim *****************//
    
//#ifdef PREPMODE
    maxiMix prepMix;
    double* synthOutPrep;
    double prepChannel;
    double* prepPanPtr;
    double prepPanVal;
    double prepChannelOut[2];
    avObject avPrep;
    bool loaded;
    bool trained;
    bool recording;
//#endif
    
    double sampleRate, bufferSize;

    
//#ifdef PERFORMANCEMODE

    double* synthOut[NUMAVS];
    double* panning[NUMAVS];
    double channel[NUMAVS];
    double panner[NUMAVS];
    maxiMix channelMix[NUMAVS];
    double channelOut[NUMAVS][2];
    
    avObject av[NUMAVS];

//#endif

    double finalSig[2];
    double fullSig;
    
    maxiEnv lim;
    bool limTrig;
    double envSig;
    
//#ifdef PSMOVE_ON
    
    //********** PS Move ***********//
    
//    psMoveTrackerClass tracker;
    
//#endif
    
    controller controller;
    ofVec3f* camXyzValsPtr;
    ofVec3f camXyzValsForModelInput;
    ofVec3f* objXyzValsPtr;
    ofVec3f objXyzValsForObjectPlacement;
    
//    ofxSyphonServer mainSyphonOut;
    
    std::string jpgTexture1;
    std::string jpgTexture2;
    std::string jpgTexture3;
    
    unsigned int triggerVal = 0;
    float wOrient = 0;
//    bool keyboardDrawInterface;
    bool keyboardCntrlObj1;
    bool keyboardCntrlObj2;
    bool keyboardCntrlObj3;
    ofMatrix4x4 idMatrix;
    float* idMatDataPtr;
    int blurToFilterMap;
    bool randomButton;
    bool recordButton;
    bool trainButton;
    bool selObj1;
    bool selObj2;
    bool selObj3;
    int* objectPointer;
    int objNum1;
    int objNum2;
    int objNum3;
    int masterFadeOut;
    float masterFadeOutVis;
    float masterFadeOutVol;
    int* ptrToPadCoords;
    int padCoords[4];
};
