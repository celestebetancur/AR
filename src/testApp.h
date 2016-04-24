#pragma once

#include "ofMain.h"
#include "ofxAruco.h"
#include "ofxAssimpModelLoader.h"
#include "ofxOsc.h"

#define PORT 7788


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed  (int key);
		void keyReleased(int key);

		ofBaseVideoDraws * video;

		ofxAruco aruco;
		bool useVideo;
		bool showMarkers;
		bool showBoard;
		bool showBoardImage;
		ofImage board;
		ofImage marker;    

        ofxAssimpModelLoader model;
        ofMesh mesh;
    
    public:
        ofVideoPlayer portada;
    
        ofSoundPlayer voz;
    
        ofVideoGrabber vidGrabber;
        ofPixels videoInverted;
        ofTexture videoTexture;
        int camWidth;
        int camHeight;
        ofImage imagen;
        ofPixels imagenP;
        ofPixels pixels;
        ofxOscReceiver receiver;
    
        int i = 0;
        int j = 1;
        int k = 2;
    
        int Rmin, Gmin, Bmin, Rmax, Gmax, Bmax;
    
        ofDirectory fotos;
        ofDirectory audio;
        ofDirectory animations;
    
        int currentFoto;
        int currentAudio;
    
        bool stoped;
        bool help;
        bool audioPlay;
};
