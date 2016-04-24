#include "testApp.h"
#include "ofxCv.h"
#include "ofBitmapFont.h"
#include "ofxAssimpModelLoader.h"
#include "ofxOsc.h"
#include "ofBitmapFont.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofDisableArbTex();
    
    Rmin = 0;
    Gmin = 0;
    Bmin = 0;
    Rmax = 0;
    Gmax = 0;
    Bmax = 0;
    
    receiver.setup(PORT);
    
    stoped = true;
    help = false;
    
    camWidth = 1280;
    camHeight = 800;
    
    portada.load("pint.mov");
    
    fotos.allowExt("jpg");
    fotos.listDir("fotos");
    currentFoto = 0;
    imagen.load(fotos.getPath(currentFoto));
    imagen.resize(camWidth,camHeight);
    
    audio.allowExt("wav");
    audio.listDir("voces");
    currentAudio = 0;
    voz.load(audio.getPath(currentAudio));
    voz.setVolume(0.75f);
    
    animations.allowExt("dae");
    animations.listDir("Animations");
    model.loadModel(animations.getPath(currentAudio));
    model.setScale(.5f, .5f, .5f);
    
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    
    for(int i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
    
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber(camWidth, camHeight);
    
    videoInverted.allocate(camWidth, camHeight, OF_PIXELS_RGB);
    videoTexture.allocate(videoInverted);
    ofSetVerticalSync(true);
    
	ofSetVerticalSync(true);
	string boardName = "boardConfiguration.yml";
    
    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
	aruco.setup("intrinsics.int", vidGrabber.getWidth(), vidGrabber.getHeight(), boardName);
	aruco.getBoardImage(board.getPixels());
	board.update();

	showMarkers = true;
	showBoard = true;
	showBoardImage = false;

	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::update(){
    if(audioPlay == true && !voz.isPlaying())
        voz.play();
    if(audioPlay == false)
        voz.stop();
    
    ofBackground(0, 0, 0);
    vidGrabber.update();
    model.update();
    ofSoundUpdate();
    portada.update();
    
    receiver.hasWaitingMessages();
    ofxOscMessage m;
    receiver.getNextMessage(m);
    
    if(m.getAddress() == "/parameters"){
        Rmin = m.getArgAsInt32(0);
        Gmin = m.getArgAsInt32(1);
        Bmin = m.getArgAsInt32(2);
        Rmax = m.getArgAsInt32(3);
        Gmax = m.getArgAsInt32(4);
        Bmax = m.getArgAsInt32(5);
    }
    
    if(vidGrabber.isFrameNew()){
        
        aruco.detectBoards(vidGrabber.getPixels());
        pixels = vidGrabber.getPixels();
        
        imagenP = imagen.getPixels();
        for(int c = 1; c < pixels.size(); c = c + 3){
            if ((pixels[i] > Rmin && pixels[j] > Gmin && pixels[k] > Bmin) && (pixels[i] < Rmax && pixels[j] < Gmax && pixels[k] < Bmax)) {
                videoInverted[i] = imagenP[i];
                videoInverted[j] = imagenP[j];
                videoInverted[k] = imagenP[k];
                }
            else {
                videoInverted[i] = pixels[i];
                videoInverted[j] = pixels[j];
                videoInverted[k] = pixels[k];
            }
            i = 3 + i;
            j = 3 + j;
            k = 3 + k;
        }
        videoTexture.loadData(videoInverted);
        i = 0;
        j = 1;
        k = 2;
    }
    mesh = model.getCurrentAnimatedMesh(0);
}

//--------------------------------------------------------------
void testApp::draw(){
    audioPlay = false;
    
    if(stoped){
        portada.draw(0,0, camWidth,camHeight);
        if(!portada.isPlaying())
            portada.play();
        voz.stop();
    }
    else {
        cv::Point_<float> point;
        float x,y;
        
        ofSetColor(255);
        videoTexture.draw(camWidth, 0, -camWidth, camHeight);
        mesh = model.getMesh(1);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofEnableDepthTest();
        ofEnableSeparateSpecularLight();
        model.draw(OF_MESH_FILL);
        if(showMarkers){
            for(int i=0;i<aruco.getNumMarkers();i++){
                aruco.begin(i);
                audioPlay = true;
                point = aruco.getMarkers()[i][1];
                x = camWidth - point.x;
                y = point.y;
                aruco.end();
            }
            if(currentAudio == 2)
                model.setPosition((x+25),(y-55),50);
            else {model.setPosition((x+25),(y+25),50);}
            model.draw(OF_MESH_FILL);
        }
    }
    
    if(help == true) {
        ofDrawBitmapString("Ayuda: ",20,20);
        ofDrawBitmapString("(s) para detener y volver a pantalla de inicio",20,40);
        ofDrawBitmapString("(p) para iniciar",20,60);
        ofDrawBitmapString("derecha o izquierda para cambiar de foto",20,80);
        ofDrawBitmapString("arriba o abajo para cambiar animacion",20,100);
        ofDrawBitmapString("cmd + (q) para salir de la aplicacion",20,120);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == 'h' || key == 'H'){
        help = true;
    }
    if(key == 's' || key == 'S'){
        stoped = true;
    }
    if(key == 'p' || key == 'P'){
        stoped = false;
    }
    if(key == OF_KEY_RIGHT) {
        if(currentFoto < (fotos.size()-1)) {
            currentFoto++;
            imagen.load(fotos.getPath(currentFoto));
            imagen.resize(camWidth,camHeight);
        }
    }
    if(key == OF_KEY_LEFT) {
        if(currentFoto > 0) {
            currentFoto--;
            imagen.load(fotos.getPath(currentFoto));
            imagen.resize(camWidth,camHeight);
        }
    }
    if(key == OF_KEY_UP) {
        if(currentAudio < (audio.size()-1)) {
            currentAudio++;
            model.loadModel(animations.getPath(currentAudio));
            model.setScale(.5f, .5f, .5f);
            model.setLoopStateForAllAnimations(OF_LOOP_NONE);
            model.playAllAnimations();
            voz.load(audio.getPath(currentAudio));
            voz.play();
        }
    }
    if(key == OF_KEY_DOWN) {
        if(currentAudio > 0) {
            currentAudio--;
            model.loadModel(animations.getPath(currentAudio));
            model.setScale(.5f, .5f, .5f);
            model.setLoopStateForAllAnimations(OF_LOOP_NONE);
            model.playAllAnimations();
            voz.load(audio.getPath(currentAudio));
            voz.play();
        }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if(key == 'h' || key == 'H'){
        help = false;
    }
}

