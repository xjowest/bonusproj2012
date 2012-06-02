#include "testApp.h"
#define c0 1.0/9
#define c1 -1.0/9
#define c2 8.0/9

//------LP filter mask------------------------------------------
float LP[3][3] = {{c0,c0,c0},
                  {c0,c0,c0},
                  {c0,c0,c0}};
float HP[3][3] = {{c1,c1,c1},
                  {c1,c2,c1},
                  {c1,c1,c1}};

//--------------------------------------------------------------
void testApp::setup(){

	camWidth 		= 320;	// try to grab at this size.
	camHeight 		= 240;

	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(camWidth,camHeight);

	grayPixels = new unsigned char[camWidth*camHeight*3];
	videoTexture.allocate(camWidth,camHeight, GL_RGB);
}


//--------------------------------------------------------------
void testApp::update(){

	ofBackground(100,100,100);

	vidGrabber.grabFrame();

	if (vidGrabber.isFrameNew()){
		unsigned char * pixels = vidGrabber.getPixels();

		get2DVector(vector2D, pixels);
		for(int i=0;i<2;i++) filter3x3(vector2D, LP);
		filter3x3(vector2D, HP);
		tHold(vector2D);
		for(int i=0;i<8;i++) shrink(vector2D);
		for(int i=0;i<10;i++) expand(vector2D);
		for(int i=0;i<12;i++) shrink(vector2D);
		getRGBVector(grayPixels, vector2D);
    }

	videoTexture.loadData(grayPixels, camWidth,camHeight, GL_RGB);
}

//------Convert from RGB pixel array to 2D intensity vector-----
void testApp::get2DVector(unsigned char vector2D[320][240], unsigned char * pixels){
    for(int y = 1; y < camHeight-1; y++){
        for(int x = 1; x < camWidth-1; x++){
            vector2D[x][y] = (pixels[x*3+y*camWidth*3] + pixels[x*3+y*camWidth*3+1] + pixels[x*3+y*camWidth*3+2]) / 3;
        }
    }
}

//------Convert from 2D intensity vector to gray scale RGB array
void testApp::getRGBVector(unsigned char * grayPixels, unsigned char vector2D[320][240]){
    for(int y = 1; y < camHeight-1; y++){
        for(int x = 1; x < camWidth-1; x++){
            grayPixels[x*3+y*camWidth*3]   = vector2D[x][y];
            grayPixels[x*3+y*camWidth*3+1] = vector2D[x][y];
            grayPixels[x*3+y*camWidth*3+2] = vector2D[x][y];
        }
    }
}

//------Tresh hold----------------------------------------------
void testApp::tHold(unsigned char img[320][240]){
    for(int y = 1; y < camHeight-1; y++){
        for(int x = 1; x < camWidth-1; x++){
            if(img[x][y] > 64)
                img[x][y] = 255;
            else
                img[x][y] = 0;
        }
    }
}

//------Filter function-----------------------------------------
void testApp::filter3x3(unsigned char img[320][240], float c3x3[3][3]){
    unsigned char tmp[320][240];
    for(int y = 1;y < camHeight-1; y++){
        for(int x = 1; x < camWidth-1; x++){
            tmp[x][y] = img[x-1][y-1] * c3x3[0][0] + img[x][y-1] * c3x3[1][0] + img[x+1][y-1] * c3x3[2][0] +
                        img[x-1][y]   * c3x3[0][1] + img[x][y]   * c3x3[1][1] + img[x+1][y]   * c3x3[2][1] +
                        img[x-1][y+1] * c3x3[0][2] + img[x][y+1] * c3x3[1][2] + img[x+1][y+1] * c3x3[2][2];
        }
    }
    for(int y = 1;y < camHeight-1; y++){
        for(int x = 1; x < camWidth-1; x++){
            img[x][y] = tmp[x][y];
        }
    }
}

//------Shrink function-----------------------------------------
void testApp::shrink(unsigned char img[320][240]){
    unsigned char tmp[320][240];

    for(int y = 1;y < camHeight-1; y++){
        for(int x = 1; x < camWidth-1; x++){
            if(countPix(img, x, y) > 2)
                tmp[x][y] = img[x][y];
            else
                tmp[x][y] = 0;
        }
    }
    for(int y = 1;y < camHeight-1; y++){
        for(int x = 1; x < camWidth-1; x++){
            img[x][y] = tmp[x][y];
        }
    }
}

//------Expand function-----------------------------------------
void testApp::expand(unsigned char img[320][240]){
    unsigned char tmp[320][240];

    for(int y = 1;y < camHeight-1; y++){
        for(int x = 1; x < camWidth-1; x++){
            if(countPix(img, x, y) > 4)
                tmp[x][y] = 255;
            else
                tmp[x][y] = img[x][y];
        }
    }
    for(int y = 1;y < camHeight-1; y++){
        for(int x = 1; x < camWidth-1; x++){
            img[x][y] = tmp[x][y];
        }
    }
}

//------Count surrounding pixels--------------------------------
int testApp::countPix(unsigned char img[320][240], int x, int y){
    int count;

    count = (img[x-1][y-1] + img[x][y-1] + img[x+1][y-1] +
             img[x-1][y]   +               img[x+1][y]   +
             img[x-1][y+1] + img[x][y+1] + img[x+1][y+1]) / 255;

    return count;
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetHexColor(0xffffff);
	vidGrabber.draw(20,20);
	videoTexture.draw(40+camWidth,20,camWidth,camHeight);
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	// in fullscreen mode, on a pc at least, the
	// first time video settings the come up
	// they come up *under* the fullscreen window
	// use alt-tab to navigate to the settings
	// window. we are working on a fix for this...

    if (key == 's' || key == 'S'){
		vidGrabber.videoSettings();
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
