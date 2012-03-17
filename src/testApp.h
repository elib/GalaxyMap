#pragma once

#include "ofMain.h"

#define MAX_N_PTS         100

typedef vector< ofVec3f > Vec3fArray;

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

		float currentRotation;

		Vec3fArray drawPoints;
		
		ofVec3f	pts[MAX_N_PTS];
		int		nPts;

		ofVec3f	rotationAxis;  // amount to rotate in x,y,z;
		ofVec3f viewingAngleAxis;
		float	speedOfRotation; // speed;

		// a grid helpful for seeing the rotation
		ofVec3f	xAxisMin;
		ofVec3f	xAxisMax;
		ofVec3f	yAxisMin;
		ofVec3f	yAxisMax;
		ofVec3f	zAxisMin;
		ofVec3f	zAxisMax;
};

