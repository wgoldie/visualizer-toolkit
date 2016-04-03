#include "ofMain.h"
#include "ofApp.h"
#include "ofGLProgrammableRenderer.h"

//========================================================================
int main( ){
	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);

	ofCreateWindow(settings);
	ofSetWindowShape(1280, 720);
	ofRunApp(new ofApp());

}
