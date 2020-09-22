//
//  controller.hpp
//  regMap
//
//  Created by Bryan Dunphy on 29/03/2018.
//

#ifndef controller_hpp
#define controller_hpp

#include <stdio.h>

#include "touchOsc.hpp"
#include "ofMain.h"

class controller {
  
public:
    
    controller();
    ~controller();
    
    void setup(const int port);
    void update();
    ofVec3f* getCamXyz();
    ofVec3f* getObjXyz();
    bool oscMessageReceived();
    int getSizeTrigValue();
    int getBlurValue();
    int* getObj();
    int getMasterFader();
    int* getPadCoords();
    
protected:
    
    float* touchOscUpdatedValuesPtr;
    ofVec3f camXyz;
    ofVec3f objXyz;
    TouchOsc touch;
    int objSizeTrigger;
    int blurFilterValue;
    int objectSelectButton1;
    int objectSelectButton2;
    int objectSelectButton3;
    int objectArray[3];
    int masterFader;
    int padCoords[4];
    
};
#endif /* controller_hpp */
