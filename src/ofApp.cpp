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

	shader.load("shaders/a");
}

void ofApp::setupGui() {
	shapeParamGroup.setName("SHAPE");
	shapeParamGroup.add(shapeType.set("SHAPE TYPE", 0, 0, SHAPE_COUNT - 1));
	shapeParamGroup.add(size.set("SIZE", 500, 0, 2000));
	shapeParamGroup.add(iter.set("ITER", 1, 0, 10));
	shapeParamGroup.add(gridX.set("GRID X", 5, 0, 25));
	shapeParamGroup.add(gridY.set("GRID Y", 5, 0, 25));
	shapeParamGroup.add(gridZ.set("GRID Z", 5, 0, 25));

	animationParamGroup.setName("ANIMATION");
	animationParamGroup.add(gridVibeScale.set("GRID VIBE SCALE", 50, 0, 500));
	animationParamGroup.add(gridTension.set("GRID TENSION", 0., 0, 0.3));
	animationParamGroup.add(colorVibeScale.set("COLOR VIBE SCALE", 0.2, 0., 0.5));
	animationParamGroup.add(colorTension.set("COLOR TENSION", 0., 0, 0.025));
	animationParamGroup.add(wireframeVibeScale.set("WIREFRAME VIBE SCALE", 0.2, 0., 0.5));
	animationParamGroup.add(wireframeTension.set("WIREFRAME TENSION", 0., 0, 0.025));
	animationParamGroup.add(bgVibeScale.set("BG VIBE SCALE", 0.2, 0., 0.5));
	animationParamGroup.add(bgTension.set("BG TENSION", 0., 0, 0.025));

	colorParamGroup.setName("COLORS");
	colorParamGroup.add(polyColor.set("POLY COLOR", ofColor(0, 0), ofColor(0, 0), ofColor(255, 255)));
	colorParamGroup.add(wireframeColor.set("WIREFRAME COLOR", ofColor(0, 0), ofColor(0, 0), ofColor(255, 255)));
	colorParamGroup.add(bgColor.set("BG COLOR", ofColor(0, 0), ofColor(0, 0), ofColor(255, 255)));

	renderParamGroup.setName("RENDER");
	renderParamGroup.add(fadeScale.set("FADE SCALE", 0.5, 0.0, 1.0));

	gui.setup("GUI");
	gui.add(renderParamGroup);
	gui.add(shapeParamGroup);
	gui.add(colorParamGroup);
	gui.add(animationParamGroup);


	
	//render.setup(renderParamGroup);
	//shape.setup(shapeParamGroup, "settings.xml", );
	//animation.setup(animationParamGroup);
	//color.setup(colorParamGroup);

	


	gridX.addListener(this, &ofApp::regenI);
	gridY.addListener(this, &ofApp::regenI);
	gridZ.addListener(this, &ofApp::regenI);
	shapeType.addListener(this, &ofApp::regenI);
	iter.addListener(this, &ofApp::regenI);
	size.addListener(this, &ofApp::regenI);
}

void ofApp::regenI(int & i) {
	regenHelper();
}

void ofApp::regenF(float & f) {
	regenHelper();
}

void ofApp::regenHelper() {
	meshNode.setPosition(0, 0, 0);
	switch (shapeType) {
	case 0:
		mesh = ofMesh::plane(size, size, gridX, gridY);
		break;
	case 1:
		mesh = ofMesh::sphere(size, iter);
		break;
	case 2:
		mesh = ofMesh::box(size, size, size, gridX, gridY, gridZ);
		break;
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	beat.update(ofGetElapsedTimeMillis());
	switch (shapeType) {
	case 0:
		for (int i = 0; i < mesh.getNumVertices(); i++) {
			ofVec3f * v = &mesh.getVertices().at(i);
			v->z += beat.hihat() * ofRandomf() * gridVibeScale;
			v->z = ofLerp(v->z, 0, gridTension);
		}

		break;
	case 1:
		for (int i = 0; i < mesh.getNumVertices(); i++) {
			ofVec3f * v = &mesh.getVertices().at(i);
			if (beat.hihat()) {
				v->scale(v->length() + ofRandomf() * gridVibeScale);
			}
			v->scale(ofLerp(v->length(), size, gridTension));

		}

	case 2:
		for (int i = 0; i < mesh.getNumVertices(); i++) {
			ofVec3f * v = &mesh.getVertices().at(i);
			if (beat.hihat()) {
				v->scale(v->length() + ofRandomf() * gridVibeScale);
			}
			v->scale(ofLerp(v->length(), size / 2, gridTension));
		}
	}


	renderPolyColor.lerp(polyColor, colorVibeScale * beat.kick());
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
	cam.setFarClip(2 * cam.getDistance());
	cam.setNearClip(0.1 * cam.getDistance());
	cam.begin();	
		ofPushMatrix();
		ofSetColor(renderPolyColor);
		shader.begin();
		shader.setUniform1f("FAR", cam.getFarClip());
		shader.setUniform1f("NEAR", cam.getNearClip());
		shader.setUniform1f("fadeScale", fadeScale);
		shader.setUniform3f("baseColor", renderPolyColor.r / 255.0, renderPolyColor.g / 255.0, renderPolyColor.b / 255.0);
		mesh.drawFaces();
		shader.setUniform3f("baseColor", renderWireFrameColor.r / 255.0, renderWireFrameColor.g / 255.0, renderWireFrameColor.b / 255.0);
		mesh.drawWireframe();
		shader.end();
		ofPopMatrix();
	cam.end();
	
	gui.draw();

	//shape.draw();
	//color.draw();
	//animation.draw();

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
