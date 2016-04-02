#pragma once

#include "ofMain.h"
#include "ofxBeat.h"
#include "ofxGui.h"
#include "ofxNetwork.h"
#include "ofxVideoRecorder.h"
class ofApp : public ofBaseApp{

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void regenI(int & i);
		void regenF(float & f);
		void regenHelper();
		void setupGui();

		ofxBeat beat;
		void audioIn(float *input, int bufferSize, int nChannels, int deviceID, unsigned long long tickCount);

		ofSoundStream soundStream;

		ofxPanel shape;
		ofParameterGroup shapeParamGroup;
		ofParameter<int> gridX;
		ofParameter<int> gridY;
		ofParameter<int> gridZ;
		ofParameter<int> shapeType;
		ofParameter<int> size;
		ofParameter<int> iter;

		ofxPanel animation;
		ofParameterGroup animationParamGroup;
		ofParameter<float> gridVibeScale;
		ofParameter<float> gridTension;
		ofParameter<float> colorVibeScale;
		ofParameter<float> colorTension;
		ofParameter<float> bgVibeScale;
		ofParameter<float> bgTension;
		ofParameter<float> wireframeVibeScale;
		ofParameter<float> wireframeTension;

		ofxPanel color;
		ofParameterGroup colorParamGroup;
		ofParameter<ofColor> polyColor;
		ofParameter<ofColor> bgColor;
		ofParameter<ofColor> wireframeColor;

		ofMesh mesh;
		ofNode meshNode;

		ofEasyCam cam;
		
		ofImage img;
		ofxVideoRecorder rec;
		bool isRecording;
		int lastFrame = 0;

		ofColor renderPolyColor;
		ofColor renderBgColor;
		ofColor renderWireFrameColor;
		ofShader shader;
};