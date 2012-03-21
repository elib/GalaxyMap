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
	showCrosshair = false;

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
	float maxRad = MAX_GALAXY_RADIUS;

	float barRad = ofRandom(0.1f, 0.7f) * maxRad;
	//float N = ofRandom(1.0f, 4.0f);
	float Phi = ofRandom(0.35f, 0.7f);

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
		//float denom = 1.0f - N * (sinf(Phi / N) * tanf(Phi) * logf(tanf(phi/(2*N)) / tanf(Phi / (2*N)) ));
		float denom = 1.0f - Phi*tanf(Phi)*log(phi/Phi);
		if(isnan(denom))
		{
			int j = 100;
		}
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

	DistributeCloud();
}

void testApp::DistributeCloud()
{
	galaxyStars.clear();

	for(int i = 0; i < MAX_CLOUD_POINTS; i++)
	{
		bool found = false;

		while(!found)
		{
			//random point in space
			ofVec3f testPoint(ofRandom(-MAX_GALAXY_RADIUS, MAX_GALAXY_RADIUS),
				ofRandom(-MAX_GALAXY_RADIUS, MAX_GALAXY_RADIUS), 
				ofRandom(-GALAXY_THICKNESS, GALAXY_THICKNESS));
			//find distance to galactic curve
			float distance = DistanceToGalaxy(testPoint);
			//apply probability --> random
			float randomTest = expf( - distance * 10 / MAX_GALAXY_RADIUS);
			float rand = ofRandom(0, 1.0f);
			if(rand < randomTest)
			{
				//accept!
				found = true;
				Star s(testPoint, (int)ofRandom(0xffffff), ofRandom(1, 3));
				galaxyStars.push_back(s);
			}
			else
			{
				float eli = -1;
			}

			//if fail, retry random point
		}
	}
}

float testApp::DistanceToGalaxy(ofVec3f testPoint)
{
	//find closest point in list?
	int closestPoint = FindClosestGalaxyPoint(testPoint);
	if(closestPoint == -1)
	{
		int j = 4;
		//ERROR, should probably never happen. Maybe!
	}
	//choose second closest point that is +/-1 point away
	int secondPoint;
	if(closestPoint != 0 && closestPoint != (nPts-1))
	{
		//there are two options for segments, plus or minus.
		int candidate1 = closestPoint -1;
		int candidate2 = closestPoint +1;
		if(testPoint.distanceSquared(pts[candidate1])
			< testPoint.distanceSquared(pts[candidate2]))
		{
			//1 is closer
			secondPoint = candidate1;
		}
		else
		{
			secondPoint = candidate2;
		}
	}
	else if (closestPoint == 0)
	{
		secondPoint = 1;
	}
	else
	{
		secondPoint = nPts - 2;
	}

	//Determine distance to straight line defined by these two points.
	float distance = PointToSegmentDistance(testPoint, pts[closestPoint], pts[secondPoint]);
	return distance;
}

float testApp::PointToSegmentDistance( ofVec3f P, ofVec3f P0, ofVec3f P1 )
{
	//found here, I guess
	//http://www.softsurfer.com/Archive/algorithm_0102/algorithm_0102.htm

	ofVec3f v = P1 - P0;
	ofVec3f w = P - P0;

	//before P0
	float c1 = w.dot(v);
	if ( c1 <= 0 )
	{
		return P.distance(P0);
	}

	//past P1
	float c2 = v.dot(v);
	if ( c2 <= c1 )
	{
		return P.distance(P1);
	}

	//not outside of the edges
	float b = c1 / c2;
	ofVec3f Pb = P0 + v * b;
	return P.distance(Pb);
}

int testApp::FindClosestGalaxyPoint(ofVec3f testPoint)
{
	int closestPoint = -1;
	float closestDist = powf(MAX_GALAXY_RADIUS * 2, 2);
	for(int i = 0; i < nPts; i++)
	{
		float thisDist = testPoint.distanceSquared(pts[i]);
		if(thisDist < closestDist)
		{
			closestDist = thisDist;
			closestPoint = i;
		}
	}

	return closestPoint;
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

		for(int i = 0; i < galaxyStars.size(); i++)
		{
			//"billboard" those stars, and orient them towards the camera always
			ofPushMatrix();
				ofTranslate(galaxyStars[i].location);
				ofRotateZ(-currentRotation);
				ofRotateY(-tiltDegs);
				ofRotateX(-viewDegs);

				//ofSetHexColor(0xf9d35e);
				ofSetHexColor(galaxyStars[i].color);
				ofFill();
				ofCircle(ofVec3f(0,0,0), 3);//galaxyStars[i].baseSize);
			ofPopMatrix();
		}

		if(showCrosshair)
		{
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

	ofPopMatrix();

	string info = "speed of rotation (a/z): " + ofToString(speedOfRotation,3) + "\n" +
		"view degrees (up/down): " + ofToString(viewDegs, 3) + "\n" +
		"tilt degrees (left/right): " + ofToString(tiltDegs,3) + "\n" +
		"z distance (+/-): " + ofToString(zDist,3) + "\n" + 
		"(RETURN regen, X toggle crosshair)\n";

	ofFill();
	ofSetHexColor(0xE5A93F);
	ofRect(10,10,300,90);
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
	case 'x':
		showCrosshair = !showCrosshair;
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