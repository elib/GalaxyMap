#pragma once

#include "ofMain.h"
#include "Star.h"

#define ASPECT_RATIO (1.77f)

#define MAX_N_PTS			200
#define MAX_CLOUD_POINTS	2500

#define MAX_GALAXY_RADIUS	(420.0f)
#define GALAXY_THICKNESS	(150.0f)

typedef vector< ofVec3f > Vec3fArray;
typedef vector< Star > StarArray;

class testApp : public ofBaseApp
{

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

	void RegenerateGalaxy();
	void DistributeCloud();
	float DistanceToGalaxy(ofVec3f testPoint);
	int FindClosestGalaxyPoint(ofVec3f testPoint);
	float PointToSegmentDistance( ofVec3f P, ofVec3f P0, ofVec3f P1 );

	float currentRotation;

	bool showCrosshair;
	bool showGalaxyLines;

	bool regenRequested;

	int pointsTested, pointsAccepted;

	StarArray galaxyStars;

	ofVec3f	pts[MAX_N_PTS];
	int		nPts;

	ofVec3f	rotationAxis;  // amount to rotate in x,y,z;
	float	speedOfRotation; // speed;

	float viewDegs, tiltDegs, zDist;

	// a grid helpful for seeing the rotation
	ofVec3f	xAxisMin;
	ofVec3f	xAxisMax;
	ofVec3f	yAxisMin;
	ofVec3f	yAxisMax;
	ofVec3f	zAxisMin;
	ofVec3f	zAxisMax;
};