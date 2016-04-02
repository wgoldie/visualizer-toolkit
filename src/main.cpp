#include "ofMain.h"
#include "ofApp.h"
#include "ofGLProgrammableRenderer.h"


//========================================================================
int main( ){
	//ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	//ofSetupOpenGL(2000,2000,OF_WINDOW);			// <-------- setup the GL context

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	ofCreateWindow(settings);

	ofSetWindowShape(1280, 720);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
