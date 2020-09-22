//
//  OscHandler.hpp
//  regMap
//
//  Created by Bryan Dunphy on 28/03/2018.
//

#ifndef OscHandler_hpp
#define OscHandler_hpp

#include <stdio.h>

#include "ofxOsc.h"

class OscHandler {
    
public:
    
    OscHandler();
    ~OscHandler();
    
    void setup(const int port);
    void receiveMessages();
    bool getReceivingMessageNotification();
    std::string address;
    float argValue;
    
private:
    
    ofxOscReceiver OscIn;
    bool receivingMessage;
    
    
};

#endif /* OscHandler_hpp */
