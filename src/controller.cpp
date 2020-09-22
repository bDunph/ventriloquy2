//
//  controller.cpp
//  regMap
//
//  Created by Bryan Dunphy on 29/03/2018.
//

#include "controller.hpp"

//-------------------------------------------------------------------------
controller::controller(){
    
}

//-------------------------------------------------------------------------
controller::~controller(){
    
}

//-------------------------------------------------------------------------
void controller::setup(const int port){
    camXyz = ofVec3f(0, 0, 0);
    objXyz = ofVec3f(0, 0, 0);
    touch.setup(port);
    touchOscUpdatedValuesPtr = NULL;
}

//-------------------------------------------------------------------------
void controller::update(){
    
    touch.parseMessage();
    touchOscUpdatedValuesPtr = touch.getValues();
    camXyz = ofVec3f(*touchOscUpdatedValuesPtr, *(touchOscUpdatedValuesPtr + 1), *(touchOscUpdatedValuesPtr + 2));
    objXyz = ofVec3f(*(touchOscUpdatedValuesPtr + 3), *(touchOscUpdatedValuesPtr + 4), *(touchOscUpdatedValuesPtr + 5));
    objSizeTrigger = *(touchOscUpdatedValuesPtr + 6);
    blurFilterValue = *(touchOscUpdatedValuesPtr + 7);
    objectSelectButton1 = *(touchOscUpdatedValuesPtr + 8);
    objectSelectButton2 = *(touchOscUpdatedValuesPtr + 9);
    objectSelectButton3 = *(touchOscUpdatedValuesPtr + 10);
    objectArray[0] = objectSelectButton1;
    objectArray[1] = objectSelectButton2;
    objectArray[2] = objectSelectButton3;
    masterFader = *(touchOscUpdatedValuesPtr + 11);
    padCoords[0] = *(touchOscUpdatedValuesPtr + 12);
    padCoords[1] = *(touchOscUpdatedValuesPtr + 13);
    padCoords[2] = *(touchOscUpdatedValuesPtr + 14);
    padCoords[3] = *(touchOscUpdatedValuesPtr + 15);
}

//-------------------------------------------------------------------------
ofVec3f* controller::getCamXyz(){
    
    ofVec3f* camXyzPtr = &camXyz;
    return camXyzPtr;
    
}

//-------------------------------------------------------------------------
ofVec3f* controller::getObjXyz(){
    
    ofVec3f* objXyzPtr = &objXyz;
    return objXyzPtr;
    
}

//-------------------------------------------------------------------------
bool controller::oscMessageReceived(){
    bool messageReceived;
    messageReceived = touch.receivingOscSignal();
    return messageReceived;
}

//-------------------------------------------------------------------------
int controller::getSizeTrigValue(){
    return objSizeTrigger;
}

//-------------------------------------------------------------------------
int controller::getBlurValue(){
    return blurFilterValue;
}

//-------------------------------------------------------------------------
int* controller::getObj(){
    int* objPtr = &objectArray[0];
    return objPtr;
}

//-------------------------------------------------------------------------
int controller::getMasterFader(){
    return masterFader;
}

//-------------------------------------------------------------------------
int* controller::getPadCoords(){
    int* padCoodrsPtr = &padCoords[0];
    return padCoodrsPtr;
}
