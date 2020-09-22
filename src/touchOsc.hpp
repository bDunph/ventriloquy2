//
//  touchOsc.hpp
//  regMap
//
//  Created by Bryan Dunphy on 28/03/2018.
//

#ifndef touchOsc_hpp
#define touchOsc_hpp

#include <stdio.h>

#include "OscHandler.hpp"
#include "ofxOsc.h"

class TouchOsc {
    
public:
    
    TouchOsc();
    ~TouchOsc();
    
    void setup(const int port);
    void parseMessage();
    float* getValues();
    bool receivingOscSignal();
    
private:
    
    OscHandler osc;
    float camX, camY, camZ;
    float objX, objY, objZ;
    float* valPtr;
    float valArray[16];
    int sizeTrig;
    int blurVal;
    int objSel1;
    int objSel2;
    int objSel3;
    int avFade;
    int leftX, leftY, rightX, rightY;
    
    
};
#endif /* touchOsc_hpp */
