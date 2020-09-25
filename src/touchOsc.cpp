//
//  touchOsc.cpp
//  regMap
//
//  Created by Bryan Dunphy on 28/03/2018.
//

#include "touchOsc.hpp"

//-----------------------------------------------------------------------
TouchOsc::TouchOsc(){
    
}

//-----------------------------------------------------------------------
TouchOsc::~TouchOsc(){
    
}

//-----------------------------------------------------------------------
void TouchOsc::setup(const int port){
    osc.setup(port);
    valPtr = NULL;
    for(int i = 0; i < 6; i++){
        valArray[i] = 0;
    }
}
//-----------------------------------------------------------------------
void TouchOsc::parseMessage(){
    
    osc.receiveMessages();
    
    if(osc.address == "/quneo/vSliders/0/location"){

        camX = osc.argValue;
        valArray[0] = camX;

    } else if(osc.address == "/quneo/vSliders/1/location"){

        camY = osc.argValue;
        valArray[1] = camY;

    } else if(osc.address == "/quneo/vSliders/2/location"){

        camZ = osc.argValue;
        valArray[2] = camZ;

    } else if(osc.address == "/quneo/hSliders/0/location"){

        objX = osc.argValue;
        valArray[3] = objX;

    }  else if(osc.address == "/quneo/hSliders/1/location"){

        objY = osc.argValue;
        valArray[4] = objY;

    }  else if(osc.address == "/quneo/hSliders/2/location"){

        objZ = osc.argValue;
        valArray[5] = objZ;

    } else if(osc.address == "/quneo/pads/0/drum/pressure"){
        sizeTrig = (int)osc.argValue;
        valArray[6] = sizeTrig;
    } else if(osc.address == "/quneo/longSlider/location"){
        blurVal = (int)osc.argValue;
        valArray[7] = blurVal;
    } else if(osc.address == "/quneo/pads/12/drum/pressure"){
        objSel1 = (int)osc.argValue;
        valArray[8] = objSel1;
    } else if(osc.address == "/quneo/pads/13/drum/pressure"){
        objSel2 = (int)osc.argValue;
        valArray[9] = objSel2;
    } else if(osc.address == "/quneo/pads/14/drum/pressure"){
        objSel3 = (int)osc.argValue;
        valArray[10] = objSel3;
    } else if(osc.address == "/quneo/hSliders/3/location"){
        avFade = (int)osc.argValue;
        valArray[11] = avFade;
    } else if(osc.address == "quneo/pads/8/drum/x"){
        leftX = (int)osc.argValue;
        valArray[12] = leftX;
    } else if(osc.address == "quneo/pads/8/drum/y"){
        leftY = (int)osc.argValue;
        valArray[13] = leftY;
    } else if(osc.address == "quneo/pads/9/drum/x"){
        rightX = (int)osc.argValue;
        valArray[14] = rightX;
    } else if(osc.address == "quneo/pads/9/drum/y"){
        rightY = (int)osc.argValue;
        valArray[15] = rightY;
    }
    
}

//-----------------------------------------------------------------------
float* TouchOsc::getValues(){
    
    valPtr = valArray;
    return valPtr;
}

//-----------------------------------------------------------------------
bool TouchOsc::receivingOscSignal(){
    bool oscIn;
    oscIn = osc.getReceivingMessageNotification();
}
