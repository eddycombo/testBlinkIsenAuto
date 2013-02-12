#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
          receiver.setup(OSCPORT);

        mantaRate = 2 ;

        mantaWidth = WIDTH;
        mantaHeight = HEIGHT;

        mantaExpos =  15000;

        mantaImage.allocate(mantaWidth, mantaHeight, OF_IMAGE_GRAYSCALE);
        camGray.allocate(mantaWidth, mantaHeight);

        if(PvInitialize() == ePvErrSuccess){
        cout<<"api initialized"<<endl;
        }

        while( PvCameraCount() < 1 ) {
            ofSleepMillis(250);
        }

        camCount = PvCameraListEx(cameraList,1,&camConnect, sizeof(cameraList));

        cout<<cameraList[0].ModelName<<endl;

        cameraUID = cameraList[0].UniqueId;

        if(PvCameraOpen( cameraUID, ePvAccessMaster, &cameraHandle ) == ePvErrSuccess){

        cout<<"camera opened"<<endl;
        }


    if(PvAttrEnumSet(cameraHandle, "PixelFormat", "Mono8")==ePvErrSuccess){
        cout<<"set to Mono8"<<endl;
    }else{

        cout<<"can't set FORMAT"<<endl;
    }

    if(PvAttrEnumSet(cameraHandle, "ExposureMode", "Manual")==ePvErrSuccess){
        cout<<"set to autoExpo"<<endl;
    }else{

        cout<<"can't set autoExpo"<<endl;
    }

    if(PvAttrUint32Set(cameraHandle, "ExposureValue", mantaExpos)==ePvErrSuccess){
        cout<<"expoval set"<<endl;
    }else{
        cout<<"can't set expoVal"<<endl;
    }


    if(PvAttrUint32Set(cameraHandle, "Width", mantaWidth)==ePvErrSuccess){
        cout<<"width: "+ofToString(mantaWidth)<<endl;
    }else{
        cout<<"can't set width"<<endl;
    }

    if(PvAttrUint32Set(cameraHandle, "Height", mantaHeight)==ePvErrSuccess){
        cout<<"height: "+ofToString(mantaHeight)<<endl;
    }else{
        cout<<"can't set height"<<endl;
    }

    if(PvAttrFloat32Set(cameraHandle,"FrameRate", mantaRate)==ePvErrSuccess){

        cout<<"frameRate: " + ofToString(mantaRate)<<endl;
    }else{
        cout<<"can't set frate"<<endl;
    }

    unsigned long FrameSize = 0;

    if(PvAttrUint32Get( cameraHandle, "TotalBytesPerFrame", &FrameSize )==ePvErrSuccess){

            cameraFrame.ImageBuffer = new char[FrameSize];
            cameraFrame.ImageBufferSize = FrameSize;

            camFrameSize = FrameSize;
            pixels = new unsigned char[FrameSize];

        cout<<"BPF set"+ofToString(FrameSize)<<endl;
    }

    if(PvCaptureAdjustPacketSize(cameraHandle, 8228)==ePvErrSuccess){

        cout<<"packetSize set"<<endl;
    }


    if(PvCaptureStart(cameraHandle) == ePvErrSuccess){

        cout<<"capture started"<<endl;
    }

    /*set cam to soft trigger*/
    if(PvAttrEnumSet(cameraHandle,"FrameStartTriggerMode","FixedRate")==ePvErrSuccess){

        cout<<"cam set to Fixed Rate"<<endl;
    }
    /* set to continuous acquisition*/
    if(PvAttrEnumSet(cameraHandle,"AcquisitionMode","Continuous")==ePvErrSuccess){

        cout<<"continuous mode set"<<endl;
    }
    /*start acquisition*/
    if(PvCommandRun(cameraHandle,"AcquisitionStart")==ePvErrSuccess){
        cout<<"acquisition started"<<endl;
    }
}

void testApp::exit(){

        if(PvCommandRun(cameraHandle,"AcquisitionStop")==ePvErrSuccess){

            cout<<"acquisition stopped"<<endl;
        }

        if(PvCaptureQueueClear(cameraHandle) == ePvErrSuccess){

            cout<<"queue cleared"<<endl;
        }


        PvCaptureEnd(cameraHandle) ;

        PvCameraClose(cameraHandle);
        cout<<"closing camera"<<endl;

        PvUnInitialize();
        cout<<"closing api"<<endl;



}


//--------------------------------------------------------------
void testApp::update(){

        while(receiver.hasWaitingMessages()==true){

            ofxOscMessage m;
            receiver.getNextMessage(&m);

            cout<<m.getAddress()<<endl;

            if(m.getAddress()== "/mantaRate"){

                    mantaRate = m.getArgAsFloat(0);
                    cout<<mantaRate<<endl;
                    PvAttrFloat32Set(cameraHandle,"FrameRate", mantaRate);

            }else if(m.getAddress()=="/mantaExpo"){

                    mantaExpos = m.getArgAsInt32(0);
                    cout<<mantaExpos<<endl;
                    PvAttrUint32Set(cameraHandle, "ExposureValue", mantaExpos);
            }


        }

        if( PvCaptureQueueFrame( cameraHandle, &cameraFrame, NULL) == ePvErrSuccess ){
              if(PvCaptureWaitForFrameDone(cameraHandle, &cameraFrame, PVINFINITE) == ePvErrSuccess){

                    memcpy(pixels, cameraFrame.ImageBuffer, 640*480);
                    mantaImage.setFromPixels(pixels, 640, 480, OF_IMAGE_GRAYSCALE );
              }
        }
        PvAttrFloat32Get(cameraHandle,"StatFrameRate",&mantaFrameRate);





}

//--------------------------------------------------------------
void testApp::draw(){

    mantaImage.draw(0, 0);

    ofDrawBitmapString(ofToString(mantaFrameRate)+" "+"Exposure: "+ofToString(mantaExpos), 10, 470);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    switch(key){

        case('a'):
            /*increment exposure*/

            mantaExpos++;
            if(PvAttrUint32Set(cameraHandle, "ExposureValue", mantaExpos)==ePvErrSuccess){
                    cout<<"expoval set to: " + ofToString(mantaExpos)<<endl;
            }else{
                cout<<"can't set expoVal"<<endl;
            }
        break;
        case('q'):
            if(mantaExpos > 58){

                mantaExpos -= 1;
            }
             if(PvAttrUint32Set(cameraHandle, "ExposureValue", mantaExpos)==ePvErrSuccess){
                    cout<<"expoval set to: " + ofToString(mantaExpos)<<endl;
            }else{
                cout<<"can't set expoVal"<<endl;
            }
        break;
    }



}

//--------------------------------------------------------------
void testApp::keyReleased(int key){




}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
