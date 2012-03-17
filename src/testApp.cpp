#include "testApp.h"
#include <vector>

//--------------------------------------------------------------
void testApp::setup()
{
	ofEnableSmoothing();

     nPts = 0;
	 currentRotation  = 0;
	 viewDegs = 85;
	 tiltDegs = 5;

	 regenRequested = false;

	 zDist = -140;

     rotationAxis.set(0,0,1);

     speedOfRotation = 0.005f;

     ofBackground(255,255,255);

	 //make axis
     xAxisMin.set(-100,0,0);
     xAxisMax.set(100,0,0);
     yAxisMin.set(0,-100,0);
     yAxisMax.set(0,100,0);
     zAxisMin.set(0,0,-100);
     zAxisMax.set(0,0,100);

	 
	 
	 RegenerateGalaxy();
}

void testApp::RegenerateGalaxy()
{
	//make spiral shape
	 float maxRad = 400.0f;

	 float barRad = ofRandom(0.1f, 0.4f) * maxRad;
	 float N = ofRandom(1.0f, 6.0f);
	 float Phi = ofRandom(0.1f, 0.5f);

	 float dphi = PI / 5.0f;
	 float phi = 0;
	 float r = 0;

	 vector<ofVec3f> temp_points;

	 //nPts = -1;
	 int pointcount = 0;
	 while(r < maxRad && pointcount < MAX_N_PTS/2)
	 {
		 pointcount++;

		 //found here: http://arxiv.org/ftp/arxiv/papers/0908/0908.0892.pdf
		 float denom = 1.0f - N * (sinf(Phi / N) * tanf(Phi) * logf(tanf(phi/(2*N)) / tanf(Phi / (2*N)) ));
		 phi += dphi;
		 r = barRad / denom;

		 //translate to x,y from polar
		 float x = r * sinf(phi);
		 float y = r * cosf(phi);

		 ofVec3f vec(x, y, 0);
		 
		 temp_points.push_back(vec);
	 }

	 nPts = -1;
	 for(int i = temp_points.size() - 1; i >= 0; i--)
	 {
		 nPts ++;
		 pts[nPts] = (-temp_points[i]);
	 }
	 for(int i = 0; i < temp_points.size(); i++)
	 {
		 nPts ++;
		 pts[nPts] = (temp_points[i]);
	 }
}

//--------------------------------------------------------------
void testApp::update()
{
	currentRotation += speedOfRotation * ofGetLastFrameTime() * 1000;

	if(regenRequested)
	{
		RegenerateGalaxy();
		regenRequested = false;
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofBackground(0x222222);

	ofVec3f center(ofGetWidth()/2, ofGetHeight()/2, - zDist);

	 ofPushMatrix();
	 ofTranslate(center);
	 ofRotateX(viewDegs);
	 ofRotateY(tiltDegs);
	 ofRotateZ(currentRotation);
	 
	 
		 ofSetHexColor(0xffffff);
		 ofNoFill();
	 
		 ofBeginShape();
		 for (int i = 0; i < nPts; i++)
		 {
			 ofVertex(pts[i].x, pts[i].y);
		 }
		 ofEndShape();
		 
		 ofSetColor(50,50,255);
		 glBegin(GL_LINES);
			glVertex3f(xAxisMin.x, xAxisMin.y, xAxisMin.z);
			glVertex3f(xAxisMax.x, xAxisMax.y, xAxisMax.z);
			glVertex3f(yAxisMin.x, yAxisMin.y, yAxisMin.z);
			glVertex3f(yAxisMax.x, yAxisMax.y, yAxisMax.z);
			glVertex3f(zAxisMin.x, zAxisMin.y, zAxisMin.z);
			glVertex3f(zAxisMax.x, zAxisMax.y, zAxisMax.z);
		 glEnd();

	 ofPopMatrix();

     string info = "speed of rotation (a/z): " + ofToString(speedOfRotation,3) + "\n" +
                  "view degrees (up/down): " + ofToString(viewDegs, 3) + "\n" +
                  "tilt degrees (left/right): " + ofToString(tiltDegs,3) + "\n" +
				  "z distance (+/-): " + ofToString(zDist,3) + "\n";

     ofFill();
     ofSetHexColor(0xE5A93F);
     ofRect(10,10,300,70);
     ofSetHexColor(0x000000);
     ofDrawBitmapString(info,30,30);
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

    switch (key){
	case OF_KEY_DOWN:
		viewDegs -= 5;
		break;
	case OF_KEY_UP:
		viewDegs += 5;
		break;
	case OF_KEY_LEFT:
		tiltDegs -= 5;
		break;
	case OF_KEY_RIGHT:
		tiltDegs += 5;
		break;
	case '+':
		zDist -= 10;
		break;
	case '-':
		zDist += 10;
		break;
    case 'a':
        speedOfRotation += 0.05f;
        break;
    case 'z':
        speedOfRotation -= 0.05f;
        break;
	case OF_KEY_RETURN:
		regenRequested = true;
		break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
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