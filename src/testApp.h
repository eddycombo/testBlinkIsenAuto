#pragma once

#include "ofMain.h"
#define _LINUX
#define _x86
#define MAX_CAM 1
#define WIDTH 640
#define HEIGHT 480
#define OSCPORT 232323

#include <PvApi.h>
#include "ofxOpenCv.h"
#include "ofxOsc.h"

#define SOFTTRIGGER

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void exit();

       int camFrameSize;

        tPvUint32       camCount, camConnect;
        tPvCameraInfoEx   cameraList[MAX_CAM];

        unsigned long   cameraNum;
        unsigned long   cameraRle;

        unsigned long   cameraUID;


        tPvHandle       cameraHandle;
        tPvFrame        cameraFrame;


        tPvFloat32      mantaRate;
        tPvInt32        mantaWidth;
        tPvInt32        mantaHeight;
        tPvInt32        mantaExpos;

        float mantaFrameRate;

        ofImage   mantaImage;
        ofTexture camText;

        unsigned char* pixels;

        ofxCvGrayscaleImage     camGray;

        ofxOscReceiver receiver;
};
