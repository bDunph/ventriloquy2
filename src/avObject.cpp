//
//  avObject.cpp
//  regMap
//
//  Created by Bryan Dunphy on 20/02/2017.
//
//
#include <avObject.hpp>

void avObject::avSetup(std::string &textureFile){
//    delete gran;
    
//    sample1.clear();
//    sample1.load(ofToDataPath("samples/lowRumbleLoop.wav"));
//    samp = sample1;
//    gran = new maxiTimePitchStretch<grainPlayerWin, maxiSample>(&samp);
//    grainSpeed = 1.f;
//    grainLength = 0.05f;
    
    ofBackground(0);
    ofSetVerticalSync(true);
    
    //******* Texture ********//
    ofDisableArbTex();
    
    //********* Texture/Image Loading ***************//
    img.load(textureFile);
    img.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    normMap.load("Abstract_003/Abstract_003_NRM.jpg");
    normMap.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    
    //*********** GUI *****************//
    gui.setup();
    gui.setPosition(ofGetWindowWidth() - 220 - 220, ofGetWindowHeight() * .25);
    gui.add(xTrans.setup("xTrans", 0.f, -1.f, 1.f));
    gui.add(yTrans.setup("yTrans", 0.f, -1.f, 1.f));
    gui.add(zTrans.setup("zTrans", 0.f, -1.f, 1.f));
    gui.add(noiseSpeed.setup("noiseSpeed", 1, 0, 2000));
    gui.add(dispH.setup("dispH", 0.0f, 0.0f, 100.0f));
    gui.add(visPulse.setup("visPulse", 0.0f, 0.0f, 1000.0f));
    gui.add(size.setup("size0", 100, 50, 1050));
    gui.add(vertices.setup("vertices0", 20, 0, 33));
    gui.add(texColR.setup("texColR0", 0, 0, 255));
    gui.add(texColG.setup("texColG0", 0, 0, 255));
    gui.add(texColB.setup("texColB0", 0, 0, 255));
    gui.add(texColA.setup("texColA0", 0, 0, 255));
    gui.add(brightness.setup("brightness0", 0.75, 0.75, 1.0));
    gui.add(waveform.setup("waveform0", 0, 0, 2));
    gui.add(oscilFreq.setup("oscilFreq0", 10.0, 0.0, 4000.0));
    gui.add(oscilAmp.setup("oscilAmp0", 0.25, 0.0, 1.0));
    gui.add(FM.setup("FM0", true));
    gui.add(lfoOn.setup("lfoOn0", true));
    gui.add(lfoFreq.setup("lfoFreq0", 10.0, 0.0, 50.0));
    gui.add(harmRatio.setup("harmRatio0", 10, 0, 100));
    gui.add(modInd.setup("modInd0", 0.5, 0.0, 5.0));
    gui.add(controlVoltage.setup("controlVoltage0", 1, 0, 100));
    gui.add(pulseDuty.setup("pulseDuty0", 0.5, 0.0, 1.0));
    gui.add(noiseFiltLowCut.setup("noisefiltLowCut0", 0.2, 0.1, 1.0));
    gui.add(noiseFiltHiCut.setup("noiseFiltHiCut0", 0.7, 0.1, 1.0));
    gui.add(cf.setup("cf0", 500, 20, 10000));
    gui.add(q.setup("q0", 10, 2, 50));
    gui.add(res.setup("res0", 2.0, 1.0, 20.0));
    gui.add(attack.setup("attack0", 300, 0, 3000));
    gui.add(decay.setup("decay0", 1, 1, 500));
    gui.add(sustain.setup("sustain0", 1, 1, 1000));
    gui.add(release.setup("release0", 1000, 0, 3000));
    
    guiRW.setup();
    guiRW.setPosition(ofGetWindowWidth() - 260 - 260, ofGetWindowHeight() * .2);
    guiRW.add(saveParams.setup("saveParams", 20, 20));
    guiRW.add(r1.setup("r1", 20, 20));
    guiRW.add(w1.setup("w1", 20, 20));
    guiRW.add(pole1Freq.setup("pole1Freq0", 100, 20, 1000));
    guiRW.add(pole2Freq.setup("pole2Freq0", 1500, 1001, 2000));
    guiRW.add(pole3Freq.setup("pole3Freq0", 2500, 2001, 3000));
    guiRW.add(pole4Freq.setup("pole4Freq0", 3500, 3001, 4000));
    guiRW.add(pole1res.setup("pole1res0", 2.0, 1.0, 5.0));
    guiRW.add(pole2res.setup("pole2res0", 2.0, 1.0, 5.0));
    guiRW.add(pole3res.setup("pole3res0", 2.0, 1.0, 5.0));
    guiRW.add(pole4res.setup("pole4res0", 2.0, 1.0, 5.0));
    guiRW.add(a1.setup("a10", 300, 0, 3000));
    guiRW.add(d1.setup("d10", 100, 1, 500));
    guiRW.add(s1.setup("s10", 100, 1, 1000));
    guiRW.add(rel1.setup("rel10", 1000, 0, 3000));
    guiRW.add(a2.setup("a20", 300, 0, 3000));
    guiRW.add(d2.setup("d20", 100, 1, 500));
    guiRW.add(s2.setup("s20", 100, 1, 1000));
    guiRW.add(rel2.setup("rel20", 1000, 0, 3000));
    guiRW.add(subEnv1CV.setup("subEnv1CV0", 1, 0, 100));
    guiRW.add(subEnv2CV.setup("subEnv2CV0", 1, 0, 100));
    
    //************** Audio Setup ****************//
    hpc = 0;
    lpc = 0;
    hp = 0;
    lp = 0;
    
    ampEnv.setAttack(attack);
    ampEnv.setDecay(decay);  // Needs to be at least 1
    ampEnv.setSustain(sustain);
    ampEnv.setRelease(release);
    
    pole1Out = 0;
    pole2Out = 0;
    pole3Out = 0;
    pole4Out = 0;
    
    subADSR1.setAttack(a1);
    subADSR1.setDecay(d1);
    subADSR1.setSustain(s1);
    subADSR1.setRelease(rel1);
    
    subADSR2.setAttack(a2);
    subADSR2.setDecay(d2);
    subADSR2.setSustain(s2);
    subADSR2.setRelease(rel2);
    
    //*************** Visual Setup ****************//
    cubeShader.load("noise_light_bump_shader");
    
    result = false;
    isTrained = false;
    
    ofVec3f axis = ofVec3f(0, 1, 0);
    ofVec3f oppositeDirectionAxis = ofVec3f(0, -1, 0);
    box2.tilt(45);
    box2.rotate(45, axis);
    box3.tilt(45);
    box3.rotate(45, oppositeDirectionAxis);
}

void avObject::visual (const unsigned int triggerValue, float* moveModelView, float* camModelViewMatPtr, ofVec3f camPos, ofVec3f lightPos){
    
    if(size >= 1050){
        size = 1050;
    } else if(size <= 55){
        size = 55;
    }
    
    trigger = triggerValue;
    float objPosX = ofMap(xTrans, -1.f, 1.f, -ofGetWindowWidth(), ofGetWindowWidth());
    float objPosY = ofMap(yTrans, -1.f, 1.f, -ofGetWindowHeight(), ofGetWindowHeight());
    float objPosZ = ofMap(zTrans, -1.f, 1.f, 1.f, 1000.f);
    ofVec3f objectPositionVector = ofVec3f(objPosX, objPosY, objPosZ);

    //************** Size to Trigger Map ***********//
    unsigned int sizeVal = 1050 - size;
    unsigned int additionalSizeVal = ofMap(triggerValue, 0, 255, 0, sizeVal);
    
    if(additionalSizeVal >= 1000){
        additionalSizeVal = 1000;
    } else if(additionalSizeVal <= 2){
        additionalSizeVal = 2;
    }
    
    shapeSize = size + additionalSizeVal;
    
    if(shapeSize >= 1050){
        shapeSize = 1050;
    } else if (shapeSize <= 55){
        shapeSize = 55;
    }
    
    camModelViewMat.set(camModelViewMatPtr);
    camModelViewMat.glTranslate(objectPositionVector);
    
    //***** object sound level related to dist from camera and size ****//
    const double soundPowerThresh = 0.000000000001;
    soundPower = ofMap(shapeSize, 50, 1050, soundPowerThresh, 0.01);
    objPos = ofVec3f(xTrans, yTrans, zTrans) * camModelViewMat;
    ofVec3f absCamPos = ofVec3f(abs(camPos.x), abs(camPos.y), abs(camPos.z));
    
    //*** distance between object and camera
    dist = absCamPos.distance(objPos);
    if(dist <= shapeSize){
        dist = shapeSize;
    }
    soundArea = 4*PI*pow(dist, 2);
    soundIntensity = soundPower/soundArea;
    logCalc = soundIntensity / soundPowerThresh;
    dbVal = abs(10*log10f(logCalc));
    mappedVol = ofMap(dbVal, 0.f, 100.f, 0.f, 1.f);
    
    box.set(shapeSize);
    box.setResolution(vertices);
    
    box2.set(shapeSize);
    box2.setResolution(vertices);
    
    box3.set(shapeSize);
    box3.setResolution(vertices);
    
    speed = noiseFreq.phasor(noiseSpeed, 1, 9);

    box.mapTexCoordsFromTexture(img.getTexture());
    box2.mapTexCoordsFromTexture(img.getTexture());
    box3.mapTexCoordsFromTexture(img.getTexture());
    
    for(int i = 0; i < NUMLIGHTS; i++){
        float angle = (i / (NUMLIGHTS / 2)) * PI;
        float x = cos(angle);
        float y = sin(angle);
        lights[i].setPosition(lightPos.x * x * i, lightPos.x * y * i, lightPos.z * x * i);
        globalLightPos[i] = ofVec4f(lights[i].getGlobalPosition().x, lights[i].getGlobalPosition().y, lights[i].getGlobalPosition().z, 1.);
    }
}

void avObject::drawVisual(ofVec3f lightCol, ofVec3f ambCol, ofVec3f specCol, float lightPow){

        img.getTexture().bind();
        cubeShader.begin();
        ofPushMatrix();
        ofLoadMatrix(camModelViewMat);
        cubeShader.setUniformTexture("normMap", normMap, 1);
        for(int j = 0; j < NUMLIGHTS; j++){
            cameraSpaceLightPos[j] = camModelViewMat * globalLightPos[j];
            cubeShader.setUniform4f("cameraSpaceLightPos", cameraSpaceLightPos[j]);
        }
        cubeShader.setUniform1f("speed", speed);
        cubeShader.setUniform1f("displacementHeight", dispH);
        cubeShader.setUniform1f("pulse", visPulse);
        cubeShader.setUniform3f("lightCol", lightCol);
        cubeShader.setUniform3f("ambCol", ambCol);
        cubeShader.setUniform3f("specCol", specCol);
        cubeShader.setUniform1f("lightPow", lightPow);
        cubeShader.setUniform1f("brightness", brightness);
        box.draw();
        box2.draw();
        box3.draw();
        ofPopMatrix();
        cubeShader.end();
        img.getTexture().unbind();
}

double * avObject::audio(float blur){
    
    //********* FM Synth ****************//
    ampEnv.setAttack(attack);
    ampEnv.setDecay(decay);  // Needs to be at least 1
    ampEnv.setSustain(sustain);
    ampEnv.setRelease(release);
    
    filteredSig = 0;
    
    //*** Oscillators ***//
    double anotherLfoVal = anotherLfo.sinewave(phase.phasor(20));
    double lfoModHarmonicity = harmRatio * anotherLfoVal;
    double lfoModAmp = lfoModHarmonicity * ((modInd * 2) * anotherLfoVal);
    double lfoModulator = lfoModulatorOscil.triangle(lfoModHarmonicity) * lfoModAmp;
    
    lfo = lfOsc.sinewave(lfoFreq);
    harmonicity = oscilFreq * (harmRatio * lfo);
    modAmp = harmonicity * (modInd * lfo);
    modulator = mod.sinewave(harmonicity + lfoModulator) * modAmp;
    
    switch (waveform){
            
        case 0 :{
            
            sine1 = sineOsc1.sinewave(oscilFreq + modulator);
            sine2 = sineOsc2.sinewave((oscilFreq * 0.2) + lfo) + sine1;
            sine3 = sineOsc3.sinewave(oscilFreq * 0.1) + sine2;
            sine4 = sineOsc4.sinewave(oscilFreq * 0.78) + sine3;
            subSig1 = (((sine1 + sine2 + sine3 + sine4) * 0.25)  * mappedVol);
            break;
        }
        case 1 :{
            
            pulse1 = pulseGen1.pulse(oscilFreq + modulator, pulseDuty);
            pulse2 = pulseGen2.pulse((oscilFreq * 0.37) + lfo, pulseDuty) + pulse1;
            pulse3 = pulseGen3.pulse(oscilFreq * 0.92, pulseDuty) + pulse2;
            pulse4 = pulseGen4.pulse(oscilFreq * 0.31, pulseDuty) + pulse3;
            subSig1 = (((pulse1 + pulse2 + pulse3 + pulse4) * 0.25) * mappedVol);
            break;
        }
        case 2 :{
            
            square1 = squareOsc1.square(oscilFreq + modulator);
            square2 = squareOsc2.square((oscilFreq * 0.17) + lfo) + square1;
            square3 = squareOsc3.square(oscilFreq * 0.69) + square2;
            square4 = squareOsc4.square(oscilFreq * 0.04) + square3;
            subSig1 = (((square1 + square2 + square3 + square4) * 0.25) * mappedVol);
            break;
        }
    }
    
    subADSR1.setAttack(a1);
    subADSR1.setDecay(d1);
    subADSR1.setSustain(s1);
    subADSR1.setRelease(rel1);
    
    subADSR2.setAttack(a2);
    subADSR2.setDecay(d2);
    subADSR2.setSustain(s2);
    subADSR2.setRelease(rel2);
    
    for(int i = 0; i < SOUNDWAVES; i++){
        bassEnv[i].setAttack(10 + (10 * i));
        bassEnv[i].setDecay(30 + (10 * i));
        bassEnv[i].setSustain(20 + (10 * i));
        bassEnv[i].setRelease(70 + (10 * i));
        bass[i] = bassOsc[i].saw((oscilFreq * 0.1) + (3 * i));
        envelopedBassSig[i] = bass[i] * bassEnv[i].adsr(1., 1);
        bassSig += envelopedBassSig[i];
    }
    
    bassSig *= 0.05;
    subSig1 += bassSig;
    sympathetic1Osc1Out = subOsc1sympathetic1.square(oscilFreq / 2);
    subSig2 = subSig1 * lfo;
    sympathetic1Osc2Out = subOsc2sympathetic1.square(oscilFreq / 3);
    
    multOsc = subSig1 * sympathetic1Osc1Out * subSig2 * sympathetic1Osc2Out;
    multOscAlt = (subSig1 + sympathetic1Osc1Out + subSig2 + sympathetic1Osc2Out) * 0.25;
    
    env1trigger = env1osc.phasor(subEnv1CV, 1, 9);
    
    if (env1trigger == 1) {
        subADSR1.trigger = 1;
    } else {
        subADSR1.trigger = 0;
    }
    ADSR1 = subADSR1.adsr(1., subADSR1.trigger);
    
    oscilOutEnv = multOsc * ADSR1;
    
    pole1Out = pole1.lores(oscilOutEnv, pole1Freq, (double) pole1res);
    pole2Out = pole2.lores(oscilOutEnv, pole2Freq, (double) pole2res);
    pole3Out = pole3.lores(oscilOutEnv, pole3Freq, (double) pole3res);
    pole4Out = pole4.lores(oscilOutEnv, pole4Freq, (double) pole4res);
    
    fourPoleOutput = pole1Out + pole2Out + pole3Out + pole4Out;
    
    env2trigger = env2osc.phasor(subEnv2CV, 1, 9);
    
    if (env2trigger == 1) {
        subADSR2.trigger = 1;
    } else {
        subADSR2.trigger = 0;
    }
    
    ADSR2 = subADSR2.adsr(1., subADSR2.trigger);
    
    subOut = fourPoleOutput * ADSR2;
    
    sigMix = (subOut + subSig1) * mappedVol;
    
    //*** Filters & Envelopes ***//
    
    hiPass = noiseFiltHi.hipass(sigMix, noiseFiltHiCut);
    
    lowPass = noiseFiltLow.lopass(sigMix, noiseFiltLowCut);
    
    hpc = (double)cf + ((double)q / 2);
    lpc = (double)cf - ((double)q / 2);
    
    hp = hiRes.hires(sigMix, hpc, (double)res);
    lp = loRes.lores(sigMix, lpc, (double)res);
    
    band = hp + lp;
    
    filteredSig = hiPass + lowPass + band;
    
    //***** Output Envelope *********//
    
    noteTrig=counter.phasor(controlVoltage, 1, 9);//phasor can take three arguments; frequency, start value and end value.
    
    if (noteTrig==1) {
        ampEnv.trigger=1; //trigger the envelope
    } else {
        ampEnv.trigger=0;//release the envelope to make it fade out only if it's been triggered
    }
    
    amplitudeEnvelope=ampEnv.adsr(1.,ampEnv.trigger);
    envOutput = filteredSig * amplitudeEnvelope;
    
    //***** Granular Module ****//
    
//    gran->setLoopStart(44100);
//    gran->setLoopEnd(88200);
//    grainLength = ofMap(shapeSize, 50, 1050, 0.03, 0.3);
//    grainSpeed = ofMap(vertices, 0, 33, 0.0, 4.0);
//
//    grainSig = gran->play(0.75, grainSpeed, grainLength, 4, 0);
//
//    granVol = ofMap(shapeSize, 50, 1050, 0.0, 1.0);
//    grainSig *= granVol;
    
//    outSig = ((envOutput + grainSig) * 0.5) * mappedVol;
    outSig = envOutput * mappedVol;
    
    //*** Delay ***//
    delBoy = delay.dl(outSig, 410 , 0.8);
    
    //*** Output ***//
    float blurToHiPassFiltMap = ofMap(blur, 0.0, 0.01, 1.0, 0.0);
    finalSigFilteredThroughBlurMap = globalLowPass.lopass(delBoy, blurToHiPassFiltMap);
    multiOut = &finalSigFilteredThroughBlurMap;
    return multiOut;
}

double * avObject::panner(){
    
    objPos_camSpace = ofVec4f(xTrans, yTrans, zTrans, 1) * camModelViewMat;
    objPos_asPercentage_ofX_axis = ofMap(objPos_camSpace.x, -1440, 1440, 0.f, 1.f);
    panOut = &objPos_asPercentage_ofX_axis;
    return panOut;
}

void avObject::randomParams(){
    
    //********** Random Visual *************//
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<double> distribution(-1.0, 1.0);
    
    noiseSpeed = abs(2000 * distribution(generator));
    if(noiseSpeed <= 1){
        noiseSpeed = 1;
    }
    
    dispH = abs(100 * distribution(generator));
    if(dispH <=0){
        dispH = 0;
    }
    
    visPulse = abs(1000 * distribution(generator));
    if(visPulse <= 0){
        visPulse = 0;
    }
    
    size = abs(1050 * distribution(generator));
    if(size <= 55){
        size = 55;
    } else if(size >= 1050){
        size = 1050;
    }
    
    vertices = abs(33 * distribution(generator));
    if(vertices <= 1){
        vertices = 2;
    } else if(vertices >= 33){
        vertices = 33;
    }
    
    //************ Random Audio ****************//
    lfoFreq = abs(50 * distribution(generator));
    
    harmRatio = abs(100 * distribution(generator));
    
    modInd = abs(5.0 * distribution(generator));
    
    controlVoltage = abs(100 * distribution(generator));
    if (controlVoltage <= 1){
        controlVoltage = 1;
    }
    
    pulseDuty = abs(distribution(generator));
    
    noiseFiltLowCut = abs(distribution(generator));
    if (noiseFiltLowCut <= .1f){
        noiseFiltLowCut = .1f;
    }
    
    noiseFiltHiCut = abs(distribution(generator));
    if (noiseFiltHiCut <= .1f){
        noiseFiltHiCut = .1f;
    }
    
    cf = abs(10000 * distribution(generator));
    if (cf <= 1){
        cf = 1;
    }
    
    q = abs(50 * distribution(generator));
    if (q <= 1){
        q = 1;
    }
    
    res = abs(20.0 * distribution(generator));
    if (res <= 1.0){
        res = 1.0;
    }
    
    attack = abs(3000 * distribution(generator));
    if (attack <= 1){
        attack = 1;
    }
    
    decay = abs(500 * distribution(generator));
    if (decay <= 1){
        decay = 1;
    }
    
    sustain = abs(1000 * distribution(generator));
    if (sustain <= 1){
        sustain = 1;
    }
    
    release = abs(3000 * distribution(generator));
    if (release <= 1){
        release = 1;
    }
    
    pole1Freq = ofRandom(20, 1000);
    
    pole2Freq = ofRandom(1001, 2000);
    
    pole3Freq = ofRandom(2001, 3000);
    
    pole4Freq = ofRandom(3001, 4000);
    
    pole1res = abs(5.0 * distribution(generator));
    if (pole1res <= 1.0){
        pole1res = 1.0;
    }
    
    pole2res = abs(5.0 * distribution(generator));
    if (pole2res <= 1.0){
        pole2res = 1.0;
    }
    
    pole3res = abs(5.0 * distribution(generator));
    if (pole3res <= 1.0){
        pole3res = 1.0;
    }
    
    pole4res = abs(5.0 * distribution(generator));
    if (pole4res <= 1.0){
        pole4res = 1.0;
    }
    
    a1 = abs(3000 * distribution(generator));
    if (a1 <= 1){
        a1 = 1;
    }
    
    d1 = abs(500 * distribution(generator));
    if (d1 <= 1){
        d1 = 1;
    }
    
    s1 = abs(1000 * distribution(generator));
    if (s1 <= 1){
        s1 = 1;
    }
    
    rel1 = abs(3000 * distribution(generator));
    if (rel1 <= 1){
        rel1 = 1;
    }
    
    a2 = abs(3000 * distribution(generator));
    if (a2 <= 1){
        a2 = 1;
    }
    
    d2 = abs(500 * distribution(generator));
    if (d2 <= 1){
        d2 = 1;
    }
    
    s2 = abs(1000 * distribution(generator));
    if (s2 <= 1){
        s2 = 1;
    }
    
    rel2 = abs(3000 * distribution(generator));
    if (rel2 <= 1){
        rel2 = 1;
    }
    
    subEnv1CV = abs(100 * distribution(generator));
    if (subEnv1CV <= 1){
        subEnv1CV = 1;
    }
    
    subEnv2CV = abs(100 * distribution(generator));
    if (subEnv2CV <= 1){
        subEnv2CV = 1;
    }
    
    oscilAmp = abs(0.8 * distribution(generator));
    if (oscilAmp > 0.8){
        oscilAmp = 0.8;
    }
    
    oscilFreq = abs(5000 * distribution(generator));
    
}

void avObject::trainingExamples(float x, float y, float z){
    
    rapidlib::trainingExample tempExample;
    
    tempExample.input = {(double) x, (double) y, (double) z};
    
    tempExample.output = {(double) noiseSpeed, (double) dispH, (double) visPulse, (double) size, (double) vertices, (double) oscilFreq, (double) oscilAmp, (double) lfoFreq, (double) harmRatio, (double) modInd, (double) controlVoltage, (double) pulseDuty, (double) noiseFiltLowCut, (double) noiseFiltHiCut, (double) cf, (double) q, (double) res, (double) attack, (double) decay, (double) sustain, (double) release, (double) pole1Freq, (double) pole2Freq, (double) pole3Freq, (double) pole4Freq, (double) pole1res, (double) pole2res, (double) pole3res, (double) pole4res, (double) a1, (double) d1, (double) s1, (double) rel1, (double) a2, (double) d2, (double) s2, (double) rel2, (double) subEnv1CV, (double) subEnv2CV};

    trainingSet.push_back(tempExample);
}

bool avObject::trainModel(){
    
    result = reg.train(trainingSet);
    return result;
}

void avObject::trainedOutput(float x, float y, float z){
    
    std::vector<double> input;
    std::vector<double> output;
    
    input.push_back ((double)x);
    input.push_back ((double)y);
    input.push_back ((double)z);
    
    output = reg.run(input);
    
    noiseSpeed = output[0];
    if(output[0] <= 1){
        noiseSpeed = 1;
    }
    
    dispH = output[1];
    if(output[1] < 0){
        dispH = 0;
    }
    
    visPulse = output[2];
    if(output[2] < 0){
        visPulse = 0;
    }
    
    size = output[3];
    if(output[3] <= 55){
        size = 55;
    } else if(output[3] >= 1050){
        size = 1050;
    }
    
    vertices = output[4];
    if (output[4] < 2){
        vertices = 2;
    } else if(output[4] >= 33){
        vertices = 33;
    }
    
    oscilFreq = output[5];
    if(output[5] < 0){
        output[5] = 0;
    }
    
    oscilAmp = output[6];
    if(output[6] < 0.1){
        output[6] = 0.1;
    } else if(output[6] > 0.8){
        output[6] = 0.8;
    }
    
    lfoFreq = output[7];
    if (output[7] < 0){
        lfoFreq = 0;
    }
    
    harmRatio = output[8];
    if (output[8] < 0){
        harmRatio = 0;
    }
    
    modInd = output[9];
    if (output[9] < 0){
        modInd = 0;
    }
    
    controlVoltage = output[10];
    if (output[10] < 1){
        controlVoltage = 1;
    }
    
    pulseDuty = output[11];
    if (output[11] < 0){
        pulseDuty = 0;
    }
    
    noiseFiltLowCut = output[12];
    if (output[12] < .1f){
        noiseFiltLowCut = .1f;
    }
    
    noiseFiltHiCut = output[13];
    if (output[13] < .1f){
        noiseFiltHiCut = .1f;
    }
    
    cf = output[14];
    if (output[14] < 1){
        cf = 1;
    }
    
    q = output[15];
    if(output[15] < 1){
        q= 1;
    } else if(output[15] > 50){
        q = 50;
    }
    
    res = output[16];
    if(output[16] < 1.0){
        res = 1.0;
    } else if (output[16] > 20.0){
        res = 20.0;
    }
    
    attack = output[17];
    if (output[17] < 1){
        attack = 1;
    }
    
    decay = output[18];
    if (output[18] < 1){
        decay= 1;
    }
    
    sustain = output[19];
    if (output[19] < 1){
        sustain = 1;
    }
    
    release = output[20];
    if (output[20] < 1){
        release = 1;
    }
    
    pole1Freq = output[21];
    if(output[21] <= 20){
        pole1Freq = 20;
    } else if (output[21] >= 1000){
        pole1Freq = 1000;
    }
    
    pole2Freq = output[22];
    if(output[22] <= 1001){
        pole2Freq = 1001;
    } else if (output[22] >= 2000){
        pole2Freq = 2000;
    }
    
    pole3Freq = output[23];
    if(output[23] <= 2001){
        pole3Freq = 2001;
    } else if (output[23] >= 3000){
        pole3Freq = 3000;
    }
    
    pole4Freq = output[24];
    if(output[24] <= 3001){
        pole4Freq = 3001;
    } else if (output[24] >= 4000){
        pole4Freq = 4000;
    }
    
    pole1res = output[25];
    if(output[25] < 1.0){
        pole1res = 1.0;
    } else if (output[25] > 5.0){
        pole1res = 5.0;
    }
    
    pole2res = output[26];
    if(output[26] < 1.0){
        pole2res = 1.0;
    } else if (output[26] > 5.0){
        pole2res = 5.0;
    }
    
    pole3res = output[27];
    if(output[27] < 1.0){
        pole3res = 1.0;
    } else if (output[27] > 5.0){
        pole3res = 5.0;
    }
    
    pole4res = output[28];
    if(output[28] < 1.0){
        pole4res = 1.0;
    } else if (output[28] > 5.0){
        pole4res = 5.0;
    }
    
    a1 = output[29];
    if (output[29] < 1){
        a1 = 1;
    }
    
    d1 = output[30];
    if (output[30] < 1){
        d1= 1;
    }
    
    s1 = output[31];
    if (output[31] < 1){
        s1 = 1;
    }
    
    rel1 = output[32];
    if (output[32] < 1){
        rel1 = 1;
    }
    
    a2 = output[33];
    if (output[33] < 1){
        a2 = 1;
    }
    
    d2 = output[34];
    if (output[34] < 1){
        d2= 1;
    }
    
    s2 = output[35];
    if (output[35] < 1){
        s2 = 1;
    }
    
    rel2 = output[36];
    if (output[36] < 1){
        rel2 = 1;
    }
    
    subEnv1CV = output[37];
    if (output[37] < 1){
        subEnv1CV = 1;
    }
    
    subEnv2CV = output[38];
    if (output[38] < 1){
        subEnv2CV = 1;
    }
}

bool avObject::readWrite(){
    
    if(saveParams) {
        gui.saveToFile("data/ventriloquy2/2ndDraft_180409/vent2_reg0_obj4_mainGui.xml");
        guiRW.saveToFile("data/ventriloquy2/2ndDraft_180409/vent2_reg0_obj4_guiRW.xml");
    }
    
    if(r1) {isTrained = reg.readJSON("data/ventriloquy2/1stDraft_180403/trainedModels/Vent2_1stDraft_Reg2.JSON");}
    
    if(w1) {reg.writeJSON("data/ventriloquy2/2ndDraft_180409/trainedModels/Vent2_2ndDraft_Reg0.JSON");}
    
    return isTrained;
}

void avObject::loadExamples(const int objNum){
    
    gui.loadFromFile("data/ventriloquy2/2ndDraft_180409/section0/vent2_reg0_obj"+ofToString(objNum)+"_mainGui.xml");
    guiRW.loadFromFile("data/ventriloquy2/2ndDraft_180409/section0/vent2_reg0_obj"+ofToString(objNum)+"_guiRW.xml");
}

void avObject::drawGuiAndGuiRW(){
    
    ofDisableDepthTest();
    gui.draw();
    guiRW.draw();
}

