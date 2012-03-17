#include "testApp.h"
#include <vector>

//--------------------------------------------------------------
void testApp::setup(){
     nPts = 0;
	 currentRotation  = 0;

     rotationAxis.set(0,0,1);
	 viewingAngleAxis.set(1, 0, 0);

     speedOfRotation = 0.1f;

     ofBackground(255,255,255);

     ofVec3f center(ofGetWidth()/2, ofGetHeight()/2, 0);


	 //make axis
     xAxisMin.set(-100,0,0);
     xAxisMax.set(100,0,0);
     yAxisMin.set(0,-100,0);
     yAxisMax.set(0,100,0);
     zAxisMin.set(0,0,-100);
     zAxisMax.set(0,0,100);

     xAxisMin += center;
     xAxisMax += center;
     yAxisMin += center;
     yAxisMax += center;
     zAxisMin += center;
     zAxisMax += center;


	 //make spiral shape
	 //one half for now, I suck
	 float dphi = PI / 10.0f;
	 float maxRad = 400.0f;
	 float barRad = 200.0f;
	 float N = 5.0f;
	 float Phi = 0.45;
	 float phi = 0;
	 float r = 0;

	 ofVec3f cent(ofGetWidth()/2, ofGetHeight()/2,0);

	 vector<ofVec3f> temp_points;

	 //nPts = -1;
	 int pointcount = 0;
	 while(r < maxRad && pointcount < MAX_N_PTS/2)
	 {
		 pointcount++;

		 float denom = 1.0f - N * (sinf(Phi / N) * tanf(Phi) * logf(tanf(phi/(2*N)) / tanf(Phi / (2*N)) ));
		 phi += dphi;
		 r = barRad / denom;

		 

		 //translate to x,y
		 float x = r * sinf(phi);
		 float y = r * cosf(phi);

		 ofVec3f vec(x, y, 0);
		 
		 temp_points.push_back(vec);
	 }


	 nPts = -1;
	 for(int i = temp_points.size() - 1; i >= 0; i--)
	 {
		 nPts ++;
		 pts[nPts] = (-temp_points[i]) + cent;
	 }
	 for(int i = 0; i < temp_points.size(); i++)
	 {
		 nPts ++;
		 pts[nPts] = (temp_points[i]) + cent;
	 }
}

//--------------------------------------------------------------
void testApp::update(){

     // rotation occurs around 0,0,0 position.
     // since 0,0,0 is the top left corner, we need to:
     // (A) move over,
     // (B) rotate
     // (C) move back
     // this could all be one for loop, but broken out here for
     // clarity on the steps:

	currentRotation += speedOfRotation;

     ofVec3f center(ofGetWidth()/2, ofGetHeight()/2,0);

	 drawPoints.clear();

     // move the points so that their center (ofGetW/2, ofGetH/2) is at 0,0,0
     for (int i = 0; i < nPts; i++)
	 {
		pts[i]-= center;
     }

     // rotate the points
     for (int i = 0; i < nPts; i++){
        pts[i] = pts[i].rotated(speedOfRotation, rotationAxis);
		//pts[i] = pts[i].rotated(- HALF_PI / 2, viewingAngleAxis);
     }

     // move them back
     for (int i = 0; i < nPts; i++){
        pts[i]+= center;
     }


     xAxisMin -= center;
     xAxisMax -= center;
     yAxisMin -= center;
     yAxisMax -= center;
     zAxisMin -= center;
     zAxisMax -= center;


     xAxisMin.rotate(speedOfRotation, rotationAxis);
     xAxisMax.rotate(speedOfRotation, rotationAxis);
     yAxisMin.rotate(speedOfRotation, rotationAxis);
     yAxisMax.rotate(speedOfRotation, rotationAxis);
     zAxisMin.rotate(speedOfRotation, rotationAxis);
     zAxisMax.rotate(speedOfRotation, rotationAxis);

     xAxisMin += center;
     xAxisMax += center;
     yAxisMin += center;
     yAxisMax += center;
     zAxisMin += center;
     zAxisMax += center;
}

//--------------------------------------------------------------
void testApp::draw(){

     ofSetHexColor(0x000000);
     ofNoFill();
	 
     ofBeginShape();
     for (int i = 0; i < nPts; i++){
		 ofVertex(pts[i].x, pts[i].y);
     }
     ofEndShape();

     string info = "speed of rotation (a/z): " + ofToString(speedOfRotation,3) + "\n" +
                  "rotateAmount in x (s/x): " + ofToString(rotationAxis.x,3) + "\n" +
                  "rotateAmount in y (d/c): " + ofToString(rotationAxis.y,3) + "\n" +
                  "rotateAmount in z (f/v): " + ofToString(rotationAxis.z,3);

     ofFill();
     ofSetHexColor(0xE5A93F);
     ofRect(10,10,300,70);
     ofSetHexColor(0x000000);
     ofDrawBitmapString(info,30,30);


     ofSetColor(50,50,255);
     glBegin(GL_LINES);
        glVertex3f(xAxisMin.x, xAxisMin.y, xAxisMin.z);
        glVertex3f(xAxisMax.x, xAxisMax.y, xAxisMax.z);
        glVertex3f(yAxisMin.x, yAxisMin.y, yAxisMin.z);
        glVertex3f(yAxisMax.x, yAxisMax.y, yAxisMax.z);
        glVertex3f(zAxisMin.x, zAxisMin.y, zAxisMin.z);
        glVertex3f(zAxisMax.x, zAxisMax.y, zAxisMax.z);
     glEnd();

}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

    switch (key){
        case 'a':
            speedOfRotation += 0.05f;
            break;
        case 'z':
            speedOfRotation -= 0.05f;
            break;
        case 's':
            rotationAxis.x += 0.05f;
            break;
        case 'x':
             rotationAxis.x -= 0.05f;
            break;
        case 'd':
            rotationAxis.y += 0.05f;
            break;
        case 'c':
             rotationAxis.y -= 0.05f;
            break;
        case 'f':
            rotationAxis.z += 0.05f;
            break;
        case 'v':
             rotationAxis.z -= 0.05f;
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

     /*if (nPts < MAX_N_PTS){
        pts[nPts].x = x;
        pts[nPts].y = y;
        nPts++;
     }*/
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
