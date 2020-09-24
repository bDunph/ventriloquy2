//
//  OscHandler.cpp
//  regMap
//
//  Created by Bryan Dunphy on 28/03/2018.
//

#include "OscHandler.hpp"

//------------------------------------------------------------------------
OscHandler::OscHandler(){
    
}

//------------------------------------------------------------------------

OscHandler::~OscHandler(){
    
}

//------------------------------------------------------------------------
void OscHandler::setup(const int port){
    
    OscIn.setup(port);
    receivingMessage = false;
    
}

//------------------------------------------------------------------------
void OscHandler::receiveMessages(){
    
    if(!OscIn.hasWaitingMessages()){
        receivingMessage = false;
    }
    
    while(OscIn.hasWaitingMessages()){
        receivingMessage = true;
        ofxOscMessage msg;
        OscIn.getNextMessage(msg);
        address = msg.getAddress();
//        argValue = msg.getArgAsFloat(0);
        argValue = msg.getArgAsInt(0);
        std::cout << "MESSSSSSSAGEE" << std::endl;
    }

}

//------------------------------------------------------------------------
bool OscHandler::getReceivingMessageNotification(){
    return receivingMessage;
}
