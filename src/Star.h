#pragma once

#include "ofMain.h"

class Star
{
public:
	Star(ofVec3f loc, ofColor col, float starSize);
	~Star(void);

	ofVec3f location;
	ofColor color;
	float baseSize;

};