#include "ofApp.h"
#define SHAPE_COUNT 3	
#define SAMPLE_RATE 44100
#define N_AUDIO_BUFFERS 2
#define N_IN_CHANNELS 4
#define N_OUT_CHANNELS 0
//--------------------------------------------------------------
void ofApp::setup(){
	soundStream.listDevices();
	soundStream.setDeviceID(2);
	soundStream.setup(this, N_OUT_CHANNELS, N_IN_CHANNELS, SAMPLE_RATE, beat.getBufferSize(), N_AUDIO_BUFFERS);
	setupGui();
	regenHelper();

	polyShader.load("shaders/a");
	//wireShader.load("shaders/wires")
}

void ofApp::setupGui() {

	ofVec4f zero = ofVec4f(0., 0., 0., 0.);
	ofVec4f one = ofVec4f(1., 1., 1., 1.);

	shapeParamGroup.setName("SHAPE");
	shapeParamGroup.add(shapeType.set("shapeType", 0, 0, SHAPE_COUNT - 1));
	shapeParamGroup.add(size.set("size", ofVec3f(100.,100.,100.), ofVec3f(1., 1., 1.), ofVec3f(500., 500., 500.)));
	shapeParamGroup.add(grid.set("grid", ofVec3f(5.,5.,5.), ofVec3f(1., 1., 1.), ofVec3f(25, 25, 25)));

	animationParamGroup.setName("ANIMATION");
	animationParamGroup.add(gridVibeScale.set("gridVibeScale", 50, 0, 500));
	animationParamGroup.add(gridTension.set("gridTension", 0., 0, 0.3));
	animationParamGroup.add(colorVibeScale.set("colorVibeScale", 0.2, 0., 0.5));
	animationParamGroup.add(colorTension.set("colorTension", 0., 0, 0.025));
	animationParamGroup.add(wireframeVibeScale.set("wireframeVibeScale", 0.2, 0., 0.5));
	animationParamGroup.add(wireframeTension.set("wireframeTension", 0., 0, 0.025));
	animationParamGroup.add(bgVibeScale.set("bgVibeScale", 0.2, 0., 0.5));
	animationParamGroup.add(bgTension.set("bgTension", 0., 0, 0.025));

	colorParamGroup.setName("colors");
	colorParamGroup.add(wireframeColor.set("wireframeColor", ofColor(0, 0), ofColor(0, 0), ofColor(255, 255)));
	colorParamGroup.add(bgColor.set("bgColor", ofColor(0, 0), ofColor(0, 0), ofColor(255, 255)));

	renderParamGroup.setName("RENDER");
	renderParamGroup.add(xMin.set("xMin", one, zero, one));
	renderParamGroup.add(xMax.set("xMax", one, zero, one));
	renderParamGroup.add(yMin.set("yMin", one, zero, one));
	renderParamGroup.add(yMax.set("yMax", one, zero, one));
	renderParamGroup.add(fadeScale.set("fadeScale", 0.5, 0.0, 1.0));
	renderParamGroup.add(gradMix.set("gradMix", 0.5, 0.0, 1.0));

	gui.setup("GUI");
	gui.add(renderParamGroup);
	gui.add(shapeParamGroup);
	gui.add(colorParamGroup);
	gui.add(animationParamGroup);

	shapeType.addListener(this, &ofApp::regenI);
	size.addListener(this, &ofApp::regenV3);
	grid.addListener(this, &ofApp::regenV3);

}


void ofApp::regenV3(ofVec3f & v) {
	regenHelper();
}

void ofApp::regenI(int & i) {
	regenHelper();
}

void ofApp::regenF(float & f) {
	regenHelper();
}

void ofApp::regenHelper() {
	meshNode.setPosition(0, 0, 0);
	ofVec3f s = size;
	ofVec3f g = grid;
	switch (shapeType) {
	case 0:
		mesh = ofMesh::plane(s.x, s.y, g.x, g.y);
		break;
	case 1:
		mesh = ofMesh::sphere(s.x, g.x);
		break;
	case 2:
		mesh = ofMesh::box(s.x, s.y, s.z, g.x, g.y, g.z);
		break;
	}

	shadow = ofMesh(mesh);
}

//--------------------------------------------------------------
void ofApp::update(){
	ofVec3f s = size;
	beat.update(ofGetElapsedTimeMillis());
	for (int i = 0; i < mesh.getNumVertices(); i++) {
		ofVec3f * v = &mesh.getVertices().at(i);
		switch (shapeType) {
		case 0: 
			v->z += beat.hihat() * ofRandomf() * gridVibeScale;
			break;
		case 1:
			if (beat.hihat()) {
				v->scale(v->length() + ofRandomf() * gridVibeScale);
			}

			break;
		case 2:
			if (beat.hihat()) {
				v->scale(v->length() + ofRandomf() * gridVibeScale);
			}
		}

		v->interpolate(shadow.getVertices().at(i), gridTension);
	}



	//renderPolyColor.lerp(polyColor, colorVibeScale * beat.kick());
	renderWireFrameColor.lerp(wireframeColor, wireframeVibeScale * beat.hihat());
	renderBgColor.lerp(ofColor(bgColor, 1.), bgVibeScale * beat.snare());

	renderPolyColor.lerp(ofColor(0), colorTension);
	renderWireFrameColor.lerp(ofColor(0), wireframeTension);	
	renderBgColor.lerp(ofColor(((ofColor)bgColor).a), bgTension);

}

void ofApp::audioIn(float *input, int bufferSize, int nChannels, int deviceID, unsigned long long tickCount) {
	beat.audioReceived(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofClear(renderBgColor);
	cam.setTarget(meshNode);
	cam.begin();	
		ofPushMatrix();
		ofSetColor(renderPolyColor);
		polyShader.begin();
		polyShader.setUniforms(shapeParamGroup);
		polyShader.setUniforms(renderParamGroup);
		mesh.drawFaces();
		polyShader.end();
		ofSetColor(wireframeColor);
		mesh.drawWireframe();
		ofPopMatrix();
	cam.end();
	
	gui.draw();

	if (isRecording) {
		img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		rec.addFrame(img.getPixels());
	}
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'f') {
		ofToggleFullscreen();
	}
	else if (key == 'r') {
		if (isRecording && rec.isInitialized()) {
			rec.close();
			isRecording = false;
		} else {
			rec.setup("rec" + ofGetTimestampString() + ".mov", ofGetWidth(), ofGetHeight(), 60.);

			rec.setVideoCodec("mpeg4");
			rec.setVideoBitrate("8000k");
			rec.start();
			isRecording = true;
		}
	}

	if (key == 's') {
		polyShader.unload();
		polyShader.load("shaders/a");
	}
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	ofLog(OF_LOG_NOTICE, "w: %i h: %i\n", w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
