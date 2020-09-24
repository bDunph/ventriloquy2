#include "ofApp.h"
#include <random>

//--------------------------------------------------------------
void ofApp::setup(){
    
//    mainSyphonOut.setName("ventriloquy2Output");
//    ofSetFrameRate(60);
    
    controller.setup(PORT);
    camXyzValsPtr = NULL;
    objXyzValsPtr = NULL;
    
    camGui.setup();
    camGui.setPosition(0, ofGetWindowHeight() * .1);
    camGui.add(camX.setup("camX", 0.0f, -1.0f, 1.0f));
    camGui.add(camY.setup("camY", 0.0f, -1.0f, 1.0f));
    camGui.add(camZ.setup("camZ", 0.0f, -1.0f, 1.0f));
    
    lightGui.setup();
    lightGui.setPosition(0, ofGetWindowHeight() * .3);
    lightGui.add(lightColR.setup("lightColR", 1.f, 0.f, 1.f));
    lightGui.add(lightColG.setup("lightColG", 1.f, 0.f, 1.f));
    lightGui.add(lightColB.setup("lightColB", 1.f, 0.f, 1.f));
    lightGui.add(ambColR.setup("ambColR", 0.1f, 0.f, 1.f));
    lightGui.add(ambColG.setup("ambColG", 0.1f, 0.f, 1.f));
    lightGui.add(ambColB.setup("ambColB", 0.1f, 0.f, 1.f));
    lightGui.add(specColR.setup("specColR", 0.1f, 0.f, 1.f));
    lightGui.add(specColG.setup("specColG", 0.1f, 0.f, 1.f));
    lightGui.add(specColB.setup("specColB", 0.1f, 0.f, 1.f));
    lightGui.add(lightPow.setup("lightPow", 100000.f, 0.f, 1000000.f));//    397000.f
    lightGui.add(lightPosX.setup("lightPosX", 0.f, -1000.f, 1000.f));
    lightGui.add(lightPosY.setup("lightPosY", 0.f, -1000.f, 1000.f));
    lightGui.add(lightPosZ.setup("lightPosZ", -780.f, -1000.f, 1000.f));
    lightGui.add(blurAmnt.setup("blurAmnt", .001f, 0.f, .01f));

    volGui.setup();
    volGui.setPosition(0, ofGetWindowHeight() * .5);
    volGui.add(masterVol.setup("masterVol", 1.0, 0.0, 1.0));
    
    //********************Maxim Setup**********************//
    
    sampleRate 	= 44100; /* Sampling Rate */
    bufferSize	= 1024; /* Buffer Size. you have to fill this buffer with sound using the for loop in the audioOut method */
    
    int current = 0;
    ofSoundStreamListDevices();
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    ofSoundStreamSetup(2, 2, this, sampleRate, bufferSize, 4); /* this has to happen at the end of setup - it switches on the DAC */
    
    //******* Rapidmix Setup (code from oF example rapidAPI_006) ***************//
//#ifdef PREPMODE
//    trained = false;
//    recording = false;
//
//    loaded = false;
//#endif
    
    drawGui = true;
//    randomButton = false;
//    recordButton = false;
//    trainButton = false;
    
    selObj1 = false;
    selObj2 = false;
    selObj3 = false;
    //****************** FBO and Blur Shader setup ****************//
    
    ofDisableArbTex();
    
    ofFbo::Settings settings;
    settings.width = ofGetWindowWidth();
    settings.height = ofGetWindowHeight();
    settings.useDepth = true;
    settings.internalformat = GL_RGBA;
    settings.maxFilter = GL_LINEAR, GL_NEAREST;
    settings.numSamples = 4;
    settings.numColorbuffers = 4;
    settings.useStencil = false;
    
    ofFbo::Settings settings2;
    settings2.width = ofGetWindowWidth();
    settings2.height = ofGetWindowHeight();
    settings2.useDepth = true;
    settings2.internalformat = GL_RGBA;
    settings2.maxFilter = GL_LINEAR, GL_NEAREST;
    settings2.numSamples = 4;
    settings2.numColorbuffers = 4;
    settings2.useStencil = false;
    
    fbo1.allocate(settings);
    fbo1.begin();
    ofClear(0, 0, 0);
    fbo1.end();
    
    fbo2.allocate(settings2);
    fbo2.begin();
    ofClear(0, 0, 0);
    fbo2.end();
    blurX.load("shaders/blurX");
    blurY.load("shaders/blurY");
    
//#ifdef PREPMODE
//    jpgTexture1 = "TexturesCom_MarbleVeined0012_1_S.jpg";
//    avPrep.avSetup(jpgTexture1);
//#endif
    
    jpgTexture1 = "TexturesCom_MarbleVeined0012_1_S.jpg";
    jpgTexture2 = "Abstract_003/Abstract_003_COLOR.jpg";
    jpgTexture3 = "TexturesCom_RockSediment0128_1_S.jpg";
    av[0].avSetup(jpgTexture1);
    av[1].avSetup(jpgTexture2);
    av[2].avSetup(jpgTexture3);
    
//#ifdef PERFORMANCEMODE
    for(int i = 0; i < NUMAVS; i++){
av[i].gui.loadFromFile("data/ventriloquy2/2ndDraft_180409/section"+ofToString(i)+")/vent2_reg"+ofToString(i)+"_obj0_mainGui.xml");
av[i].guiRW.loadFromFile("data/ventriloquy2/2ndDraft_180409/section"+ofToString(i)+"/vent2_reg"+ofToString(i)+"_obj0_guiRW.xml");
//av[i].reg.readJSON("data/ventriloquy2/2ndDraft_180409/trainedModels/Vent2_2ndDraft_Reg"+ofToString(i)+"_new.JSON");
    }
//#endif

    float identityMatrixData[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    idMatDataPtr = &identityMatrixData[0][0];
    idMatrix.set(idMatDataPtr);
}

//--------------------------------------------------------------
void ofApp::exit(){
    
    //*********** Maxim *****************//
    
    ofSoundStreamStop();
    ofSoundStreamClose();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    controller.update();
    camXyzValsPtr = controller.getCamXyz();
    camXyzValsForModelInput = *camXyzValsPtr;
    objXyzValsPtr = controller.getObjXyz();
    objXyzValsForObjectPlacement = *objXyzValsPtr;
    
    ptrToPadCoords = controller.getPadCoords();
    for(int i = 0; i < 4; i++){
        padCoords[i] = *(ptrToPadCoords + i);
    }
    
    objXyzValsForObjectPlacement.x = ofMap(objXyzValsForObjectPlacement.x, 0.f, 127.f, -1.f, 1.f);
    objXyzValsForObjectPlacement.y = ofMap(objXyzValsForObjectPlacement.y, 0.f, 127.f, -1.f, 1.f);
    objXyzValsForObjectPlacement.z = ofMap(objXyzValsForObjectPlacement.z, 0.f, 127.f, -1.f, 1.f);
    
    masterFadeOut = controller.getMasterFader();
    masterFadeOutVis = ofMap(masterFadeOut, 0, 127, 0.f, 1.f);
    masterFadeOutVol = ofMap(masterFadeOut, 0, 127, 0.f, 1.f);
    
    fbo1.begin();
    ofClear(0, 0, 0);
    fbo1.end();

    fbo2.begin();
    ofClear(0, 0, 0);
    fbo2.end();
    
    camX = ofMap(camXyzValsForModelInput.x, 0.f, 127.f, -1.0, 1.0);
    camY = ofMap(camXyzValsForModelInput.y, 0.f, 127.f, -1.0, 1.0);
    camZ = ofMap(camXyzValsForModelInput.z, 0.f, 127.f, -1.0, 1.0);
    
    float xPos = ofGetWindowWidth() * camX;
    float yPos = ofGetWindowHeight() * camY;
    float zPos = ofGetWindowHeight() * camZ;
    
    ofVec3f lookAtPos;
    
    camPos = ofVec3f(0, 0, -300);
    lookAtPos = ofVec3f(0, 0, 0);
    cam.setPosition(camPos);
    
    cam.lookAt(lookAtPos, ofVec3f(0, 1, 0));
    
    cam.setFov(120);
    cam.setNearClip(1);
    cam.setFarClip(5000);
    
    cameraSpace = cam.getModelViewMatrix();
    
    lightCol = ofVec3f(lightColR * masterFadeOutVis, lightColG * masterFadeOutVis, lightColB * masterFadeOutVis);
    ambCol = ofVec3f(ambColR * masterFadeOutVis, ambColG * masterFadeOutVis, ambColB * masterFadeOutVis);
    specCol = ofVec3f(specColR * masterFadeOutVis, specColG * masterFadeOutVis, specColB * masterFadeOutVis);
    lightPos = ofVec3f(lightPosX, lightPosY, lightPosZ);
    
    //**** PSMOVE ****//
//    triggerVal = tracker.trig;
    triggerVal = controller.getSizeTrigValue();
    triggerVal = ofMap(triggerVal, 0, 127, 0, 255);
//    tracker.update();
//    blurAmnt = ofMap(tracker.wOrientation, -1.0, 1.0, 0.0, 0.01);
//    blurAmnt = ofMap(wOrient, -1.0, 1.0, 0.0, 0.01);
    blurToFilterMap = controller.getBlurValue();
    blurAmnt = ofMap(blurToFilterMap, 0, 127, 0.0, 0.01);
//#ifdef PREPMODE
//#ifdef PSMOVE_ON
//    camX = ofMap(tracker.moveXPos, -5.0, 5.0, -1.0, 1.0);
//    camY = ofMap(tracker.moveYPos, -4.0, 4.0, -1.0, 1.0);
//    camZ = ofMap(tracker.moveZPos, -1.0, -40.0, -1.0, 1.0);
//
//
//    if(tracker.rand){
//        avPrep.randomParams();
//    }
//
//    if(tracker.train){
//        trained = avPrep.trainModel();
//    }
//    //*** recording ***//
//
//    if (tracker.record) {
//
//        recording = true;
//        avPrep.trainingExamples(tracker.moveXPos, tracker.moveYPos, tracker.moveZPos);
//    } else if (!tracker.record){
//        recording = false;
//    }
//
//    //**** trained ****//
//
//        if (trained || loaded) {
//
//            avPrep.trainedOutput(tracker.moveXPos, tracker.moveYPos, tracker.moveZPos);
//        }
//#endif
//
//#endif
    
//    if(tracker.drawInterface){
//        drawGui = !drawGui;
//    }
    
//    if(keyboardDrawInterface){
//        drawGui = !drawGui;
//    }

    
    
//#ifdef PSMOVE_OFF
    
    
    
    
    
//    controllerValuePtr = controller.controlVals();
    
    //*** recording ***//
//#ifdef PREPMODE
    
//    if(tracker.rand){
//        avPrep.randomParams();
//    }

//    if (tracker.record) {
//        avPrep.trainingExamples(camXyzValsForModelInput.x, camXyzValsForModelInput.y, camXyzValsForModelInput.z);
//    }
    
//    if(tracker.train){
//        trained = avPrep.trainModel();
//    }
    
    //**************** quneo ************************//
    
//    if(randomButton){
//        avPrep.randomParams();
//    }
//
//    if(recordButton){
//        avPrep.trainingExamples(camXyzValsForModelInput.x, camXyzValsForModelInput.y, camXyzValsForModelInput.z);
//    }
//
//    if(trainButton){
//        trained = avPrep.trainModel();
//    }
//
//    //**** trained ****//
//
//    if (trained || loaded) {
//        avPrep.trainedOutput(camXyzValsForModelInput.x, camXyzValsForModelInput.y, camXyzValsForModelInput.z);
//    }
//#endif
    
//#endif
    
    

//#ifdef PREPMODE
//
//#ifdef PSMOVE_ON
//    avPrep.visual(triggerVal, tracker.controllerModelViewMat, cameraSpace.getPtr(), camPos, lightPos);
//        loaded = avPrep.readWrite();
//#endif
    
    //*************** quneo ********************//
    
//#ifdef PSMOVE_OFF
//    avPrep.xTrans = objXyzValsForObjectPlacement.x;
//    avPrep.yTrans = objXyzValsForObjectPlacement.y;
//    avPrep.zTrans = objXyzValsForObjectPlacement.z;
//    avPrep.visual(triggerVal, tracker.controllerModelViewMat, cameraSpace.getPtr(), camPos, lightPos);
//    avPrep.visual(triggerVal, idMatrix.getPtr(), cameraSpace.getPtr(), camPos, lightPos);
//    loaded = avPrep.readWrite();
//#endif
    
//#endif
    
//#ifdef PERFORMANCEMODE
    
    
//#ifdef PSMOVE_ON
//    for(int i = 0; i < NUMAVS; i++){
//
//        if(tracker.obj[0]){
//            av[0].texColR = 63;
//            av[0].texColG = 17;
//            av[0].texColB = 0;
//            av[0].shape = 1;
//            av[0].waveform = 0;
//            av[0].trainedOutput(tracker.moveXPos, tracker.moveYPos, tracker.moveZPos);
//            av[0].visual(triggerVal, tracker.controllerModelViewMat, cameraSpace.getPtr(), camPos, lightPos);
//        }
//
//        if(tracker.obj[1]){
//            av[1].texColR = 47;
//            av[1].texColG = 48;
//            av[1].texColB = 47;
//            av[1].shape = 0;
//            av[0].waveform = 1;
//            av[1].trainedOutput(tracker.moveXPos, tracker.moveYPos, tracker.moveZPos);
//            av[1].visual(triggerVal, tracker.controllerModelViewMat, cameraSpace.getPtr(), camPos, lightPos);
//        }
//
//        if(tracker.obj[2]){
//            av[2].texColR = 63;
//            av[2].texColG = 17;
//            av[2].texColB = 0;
//            av[2].shape = 0;
//            av[2].waveform = 2;
//            av[2].trainedOutput(tracker.moveXPos, tracker.moveYPos, tracker.moveZPos);
//            av[2].visual(triggerVal, tracker.controllerModelViewMat, cameraSpace.getPtr(), camPos, lightPos);
//        }
//
//    }
//#endif
    
//#ifdef PSMOVE_OFF
    
//    for(int i = 0; i < NUMAVS; i++){
//
//        if(tracker.obj[0]){
//            av[0].xTrans = objXyzValsForObjectPlacement.x;
//            av[0].yTrans = objXyzValsForObjectPlacement.y;
//            av[0].zTrans = objXyzValsForObjectPlacement.z;
////            av[0].texColR = 63;
////            av[0].texColG = 17;
////            av[0].texColB = 0;
////            av[0].shape = 1;
//            av[0].waveform = 0;
//            av[0].trainedOutput(camXyzValsForModelInput.x, camXyzValsForModelInput.y, camXyzValsForModelInput.z);
//            av[0].visual(triggerVal, tracker.controllerModelViewMat, cameraSpace.getPtr(), camPos, lightPos);
//        }
//
//        if(tracker.obj[1]){
//            av[1].xTrans = -objXyzValsForObjectPlacement.x;
//            av[1].yTrans = -objXyzValsForObjectPlacement.y;
//            av[1].zTrans = -objXyzValsForObjectPlacement.z;
////            av[1].texColR = 47;
////            av[1].texColG = 48;
////            av[1].texColB = 47;
////            av[1].shape = 0;
//            av[1].waveform = 1;
//            av[1].trainedOutput(camXyzValsForModelInput.x, camXyzValsForModelInput.y, camXyzValsForModelInput.z);
//            av[1].visual(triggerVal, tracker.controllerModelViewMat, cameraSpace.getPtr(), camPos, lightPos);
//        }
//
//        if(tracker.obj[2]){
////            av[2].xTrans = objXyzValsForObjectPlacement.x;
////            av[2].yTrans = objXyzValsForObjectPlacement.y;
////            av[2].zTrans = objXyzValsForObjectPlacement.z;
////            av[2].texColR = 63;
////            av[2].texColG = 17;
////            av[2].texColB = 0;
////            av[2].shape = 0;
//            av[2].waveform = 2;
//            av[2].trainedOutput(camXyzValsForModelInput.x, camXyzValsForModelInput.y, camXyzValsForModelInput.z);
//            av[2].visual(triggerVal, tracker.controllerModelViewMat, cameraSpace.getPtr(), camPos, lightPos);
//        }
//
//    }
  
    objectPointer = controller.getObj();
    objNum1 = *(objectPointer);
    objNum2 = *(objectPointer + 1);
    objNum3 = *(objectPointer + 2);

    if(objNum1){
        selObj1 = !selObj1;
    }

    if(objNum2){
        selObj2 = !selObj2;
    }

    if(objNum3){
        selObj3 = !selObj3;
    }

    for(int i = 0; i < NUMAVS; i++){

        //        if(tracker.obj[0]){
        if(selObj1){
            av[0].xTrans = objXyzValsForObjectPlacement.x;
            av[0].yTrans = objXyzValsForObjectPlacement.y;
            av[0].zTrans = objXyzValsForObjectPlacement.z;
            //            av[0].texColR = 63;
            //            av[0].texColG = 17;
            //            av[0].texColB = 0;
            //            av[0].shape = 1;
            av[0].waveform = 0;
            av[0].trainedOutput(camXyzValsForModelInput.x, camXyzValsForModelInput.y, camXyzValsForModelInput.z);
            av[0].visual(triggerVal, idMatrix.getPtr(), cameraSpace.getPtr(), camPos, lightPos);
        }

        //        if(tracker.obj[1]){
        if(selObj2){
            av[1].xTrans = -objXyzValsForObjectPlacement.x;
            av[1].yTrans = -objXyzValsForObjectPlacement.y;
            av[1].zTrans = -objXyzValsForObjectPlacement.z;
            //            av[1].texColR = 47;
            //            av[1].texColG = 48;
            //            av[1].texColB = 47;
            //            av[1].shape = 0;
            av[1].waveform = 1;
            av[1].trainedOutput(camXyzValsForModelInput.x, camXyzValsForModelInput.y, camXyzValsForModelInput.z);
            av[1].visual(triggerVal, idMatrix.getPtr(), cameraSpace.getPtr(), camPos, lightPos);
        }

        //        if(tracker.obj[2]){
        if(selObj3){
            //            av[2].xTrans = objXyzValsForObjectPlacement.x;
            //            av[2].yTrans = objXyzValsForObjectPlacement.y;
            //            av[2].zTrans = objXyzValsForObjectPlacement.z;
            //            av[2].texColR = 63;
            //            av[2].texColG = 17;
            //            av[2].texColB = 0;
            //            av[2].shape = 0;
            av[2].waveform = 2;
            av[2].trainedOutput(camXyzValsForModelInput.x, camXyzValsForModelInput.y, camXyzValsForModelInput.z);
            av[2].visual(triggerVal, idMatrix.getPtr(), cameraSpace.getPtr(), camPos, lightPos);
        }

    }
//#endif
    
//#endif
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    ofEnableDepthTest();
    
    fbo1.begin();
    cam.begin();
    
    //***** Draw Objects *****//
//#ifdef PREPMODE
//        avPrep.drawVisual(lightCol, ambCol, specCol, lightPow * masterFadeOutVis);
//#endif
    
//#ifdef PERFORMANCEMODE
//    for(int i = 0; i < NUMAVS; i++){
//        if(tracker.obj[i]){
//            av[i].drawVisual(lightCol, ambCol, specCol, lightPow);
//        }
//
//    }
        if(selObj1){
            av[0].drawVisual(lightCol, ambCol, specCol, lightPow * masterFadeOutVis);
        }

        if(selObj2){
            av[1].drawVisual(lightCol, ambCol, specCol, lightPow * masterFadeOutVis);
        }


        if(selObj3){
            av[2].drawVisual(lightCol, ambCol, specCol, lightPow * masterFadeOutVis);
        }
    
//#endif
    
    cam.end();
    fbo1.end();
    
    fbo1.getTexture().bind();
    fbo2.begin();
        blurX.begin();
        blurX.setUniform1f("blurAmnt", blurAmnt);
    fbo1.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
        blurX.end();
    fbo2.end();
    fbo1.getTexture().unbind();
    
        fbo2.getTexture().bind();
        blurY.begin();
        blurY.setUniform1f("blurAmnt", blurAmnt);
    fbo2.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
        blurY.end();
        fbo2.getTexture().unbind();
    
    //************ Gui ***********//
    
    if(drawGui){

        stringstream s;
        
//#ifdef PREPMODE
        s << "trained: " << trained;
        ofDrawBitmapString(s.str(), ofGetWindowWidth()*0.1, 10);
        s.str("");
        s << "recording: " << recordButton;
        ofDrawBitmapString(s.str(), ofGetWindowWidth()*0.3, 10);
        s.str("");
//#endif
        
//#ifdef PSMOVE_ON
//        s << "xPos: " << tracker.moveXPos;
//        ofDrawBitmapString(s.str(), ofGetWindowWidth()*0.5, 10);
//        s.str("");
//        s << "yPos: " << tracker.moveYPos;
//        ofDrawBitmapString(s.str(), ofGetWindowWidth()*0.5, 20);
//        s.str("");
//        s << "zPos: " << tracker.moveZPos;
//        ofDrawBitmapString(s.str(), ofGetWindowWidth()*0.5, 30);
//        s.str("");
//#endif
        
//#ifdef PSMOVE_OFF
        s << "xPos: " << camXyzValsForModelInput.x;
        ofDrawBitmapString(s.str(), ofGetWindowWidth()*0.5, 10);
        s.str("");
        s << "yPos: " << camXyzValsForModelInput.y;
        ofDrawBitmapString(s.str(), ofGetWindowWidth()*0.5, 20);
        s.str("");
        s << "zPos: " << camXyzValsForModelInput.z;
        ofDrawBitmapString(s.str(), ofGetWindowWidth()*0.5, 30);
        s.str("");
        s << "oscIn: " << controller.oscMessageReceived();
        ofDrawBitmapString(s.str(), ofGetWindowWidth()*0.5, 40);
        s.str("");
//#endif
//        s << "wOr: " << tracker.wOrientation;
        s << "blurFilt: " << blurToFilterMap;
        ofDrawBitmapString(s.str(), ofGetWindowWidth()*0.7, 10);
        s.str("");
        s << "trigger: " << triggerVal;
        ofDrawBitmapString(s.str(), ofGetWindowWidth()*0.7, 20);
        s.str("");
        
//#ifdef PREPMODE
//        avPrep.drawGuiAndGuiRW();
//#endif
        
//#ifdef PERFORMANCEMODE
//        for(int i = 0; i < NUMAVS; i++){
//            if(tracker.obj[i]){
//                av[i].drawGuiAndGuiRW();
//            }
//    }
        if(selObj1){
            av[0].drawGuiAndGuiRW();
        }

            if(selObj2){
                av[1].drawGuiAndGuiRW();
            }

            if(selObj3){
                av[2].drawGuiAndGuiRW();
            }
//#endif
    
        volGui.draw();
        camGui.draw();
        lightGui.draw();
    }
    
//    mainSyphonOut.publishScreen();
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    
    for (int i = 0; i < bufferSize; i++){
        
//#ifdef PREPMODE
//        synthOutPrep = avPrep.audio(blurAmnt);
//        prepChannel = *(synthOutPrep);
//        prepPanPtr = avPrep.panner();
//        prepPanVal = *(prepPanPtr);
//        prepMix.stereo(prepChannel, prepChannelOut, prepPanVal);
//#endif
        
//#ifdef PERFORMANCEMODE
//        for(int j = 0; j < NUMAVS; j++){
//            if(tracker.obj[j]){
////            if(keyboardCntrlObjs[i]){
//                synthOut[j] = av[j].audio(blurAmnt);
//                channel[j] = *(synthOut[j]);
//                panning[j] = av[j].panner();
//                panner[j] = *(panning[j]);
//                channelMix[j].stereo(channel[j], channelOut[j], panner[j]);
//            }
//        }

            if(selObj1){
                synthOut[0] = av[0].audio(blurAmnt);
                channel[0] = *(synthOut[0]);
                channel[0] *= 0.33;
                panning[0] = av[0].panner();
                panner[0] = *(panning[0]);
                channelMix[0].stereo(channel[0], channelOut[0], panner[0]);
            }
        if(selObj2){
            synthOut[1] = av[1].audio(blurAmnt);
            channel[1] = *(synthOut[1]);
            channel[1] *= 0.33;
            panning[1] = av[1].panner();
            panner[1] = *(panning[1]);
            channelMix[1].stereo(channel[1], channelOut[1], panner[1]);
        }
        if(selObj3){
            synthOut[2] = av[2].audio(blurAmnt);
            channel[2] = *(synthOut[2]);
            channel[2] *= 0.33;
            panning[2] = av[2].panner();
            panner[2] = *(panning[2]);
            channelMix[2].stereo(channel[2], channelOut[2], panner[2]);
        }
        
        
        
//#endif
        
//#ifdef PREPMODE
//        finalSig[0] += prepChannelOut[0];
//        finalSig[1] += prepChannelOut[1];
//#endif
        
//#ifdef PERFORMANCEMODE
        
        for(int j = 0; j < NUMAVS; j++){
            finalSig[0] += channelOut[j][0];
            finalSig[1] += channelOut[j][1];
        }

    
//        if(tracker.obj[3]){
//            float volDown = ofMap(tracker.wOrientation, -1.0, 1.0, 0.0, 1.0);
//            masterVol = volDown;
//        }
//#endif
        
        /*** Limiter ***/
        
        if (finalSig[0] > 0.95) {
            finalSig[0] = 0.95;
        } else if (finalSig[0] < -0.95){
            finalSig[0] = -0.95;
        } else if (finalSig[1] > 0.95){
            finalSig[1] = 0.95;
        } else if(finalSig[1] < -0.95){
            finalSig[1] = -0.95;
        }
        
    output[i*nChannels    ] = finalSig[0] * (masterVol * masterFadeOutVol);
    output[i*nChannels + 1] = finalSig[1] * (masterVol * masterFadeOutVol);
    
    
    finalSig[0] = 0.f;
    finalSig[1] = 0.f;
    }
}

//--------------------------------------------------------------

void ofApp::keyPressed(int key){
    
//#ifdef PREPMODE
//    if(key >= 48 && key <= 53){
//
//        objectNumber = key - 48;
//        avPrep.loadExamples(objectNumber);
//    }
//#endif
    
//    if(key == OF_KEY_RIGHT){
//        triggerVal+=20;
//        if(triggerVal >= 255){
//            triggerVal = 255;
//        }
//    } else if(key == OF_KEY_LEFT){
//        triggerVal-=20;
//        if(triggerVal <= 2){
//            triggerVal = 2;
//        }
//    }
//
//    if(key == OF_KEY_UP){
//        wOrient += 0.1;
//        if(wOrient >= 1.0){
//            wOrient = 1.0;
//        }
//    } else if(key == OF_KEY_DOWN){
//        wOrient -= 0.1;
//        if(wOrient <= -1.0){
//            wOrient = -1.0;
//        }
//    }
//
    if(key == 100){
        drawGui = !drawGui;
    }

    if(key == 98){
        keyboardCntrlObj1 = !keyboardCntrlObj1;
    }

    if(key == 110){
        keyboardCntrlObj2 = !keyboardCntrlObj2;
    }

    if(key == 109){
        keyboardCntrlObj3 = !keyboardCntrlObj3;
    }
    
    if(key == 32){
        randomButton = true;
    }

    if(key == 114){
        recordButton = true;
    }

    if(key == 116){
        trainButton = true;
    }
    
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
//    if(key == 32){
//        randomButton = false;
//    }
//    
//    if(key == 116){
//        trainButton = false;
//    }
//    
//    if(key == 114){
//        recordButton = false;
//    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
