#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void get2DVector(unsigned char vector2D[320][240], unsigned char * pixels);
		void getRGBVector(unsigned char * grayPixels, unsigned char vector2D[320][240]);
		void filter3x3(unsigned char img[320][240], float c3x3[3][3]);
		void tHold(unsigned char img[320][240]);
		void shrink(unsigned char img[320][240]);
		void expand(unsigned char img[320][240]);
		int  countPix(unsigned char img[320][240], int x, int y);

		ofVideoGrabber 		vidGrabber;
		unsigned char * 	videoNew;
		ofTexture			videoTexture;
		int 				camWidth;
		int 				camHeight;
		unsigned char *     grayPixels;
		unsigned char       vector2D[320][240];

};
