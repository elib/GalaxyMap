#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

	ofAppGlutWindow window;
	//ofSetupOpenGL(&window, 1024,768, OF_WINDOW);			// <-------- setup the GL context

	int wid = 1200;
	float aspect = 1.77f;
	ofSetupOpenGL(&window, wid, (int) (wid / aspect), OF_WINDOW);

	//window.setGlutDisplayString("rgba double samples>=4");

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}