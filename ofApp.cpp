#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	auto noise_seed_1 = ofRandom(1000);
	auto noise_seed_2 = ofRandom(1000);
	int deg_span = 230;
	for (int radius = 50; radius <= 250; radius += 10) {

		int deg_start = ofMap(ofNoise(noise_seed_1 + radius * 0.005 + ofGetFrameNum() * 0.008), 0, 1, -360, 360);
		bool flag = true;
		auto height = ofMap(ofNoise(noise_seed_2, radius * 0.008 + ofGetFrameNum() * 0.008), 0, 1, 10, 150);

		for (int deg = deg_start; deg < deg_start + 360; deg += deg_span) {

			if (flag = !flag) {

				this->setRingToMesh(this->face, this->frame, glm::vec3(0, 0, -height * 0.5), radius, 9, height, deg, deg + deg_span);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(135);

	ofSetColor(39);
	this->face.draw();

	ofSetColor(239);
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setRingToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float radius, float width, float height, int deg_start, int deg_end) {

	if (deg_start == deg_end) { return; }

	int index = face_target.getNumVertices();

	for (int deg = deg_start; deg <= deg_end; deg += 1) {

		auto face_index = face_target.getNumVertices();

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos(deg * DEG_TO_RAD), (radius + width * 0.5) * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos((deg + 1) * DEG_TO_RAD), (radius + width * 0.5) * sin((deg + 1) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos((deg + 1) * DEG_TO_RAD), (radius + width * 0.5) * sin((deg + 1) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos(deg * DEG_TO_RAD), (radius + width * 0.5) * sin(deg * DEG_TO_RAD), height * 0.5));

		vertices.push_back(glm::vec3((radius - width * 0.5) * cos(deg * DEG_TO_RAD), (radius - width * 0.5) * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius - width * 0.5) * cos((deg + 1) * DEG_TO_RAD), (radius - width * 0.5) * sin((deg + 1) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius - width * 0.5) * cos((deg + 1) * DEG_TO_RAD), (radius - width * 0.5) * sin((deg + 1) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3((radius - width * 0.5) * cos(deg * DEG_TO_RAD), (radius - width * 0.5) * sin(deg * DEG_TO_RAD), height * 0.5));

		for (auto& vertex : vertices) {

			vertex += location;
		}

		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

		face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 6);
		face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 7);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 1);

		face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 7); face_target.addIndex(face_index + 6);
		face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 2);

		auto frame_index = frame_target.getNumVertices();

		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 1);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 3);
		frame_target.addIndex(frame_index + 4); frame_target.addIndex(frame_index + 5);
		frame_target.addIndex(frame_index + 6); frame_target.addIndex(frame_index + 7);
	}

	face_target.addIndex(index + 0); face_target.addIndex(index + 3); face_target.addIndex(index + 7);
	face_target.addIndex(index + 0); face_target.addIndex(index + 7); face_target.addIndex(index + 4);

	frame_target.addIndex(index + 0); frame_target.addIndex(index + 3);
	frame_target.addIndex(index + 0); frame_target.addIndex(index + 4);
	frame_target.addIndex(index + 7); frame_target.addIndex(index + 3);
	frame_target.addIndex(index + 7); frame_target.addIndex(index + 4);

	index = frame_target.getNumVertices() - 8;

	face_target.addIndex(index + 1); face_target.addIndex(index + 2); face_target.addIndex(index + 6);
	face_target.addIndex(index + 1); face_target.addIndex(index + 6); face_target.addIndex(index + 5);

	frame_target.addIndex(index + 1); frame_target.addIndex(index + 2);
	frame_target.addIndex(index + 1); frame_target.addIndex(index + 5);
	frame_target.addIndex(index + 6); frame_target.addIndex(index + 2);
	frame_target.addIndex(index + 6); frame_target.addIndex(index + 5);
}


//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}