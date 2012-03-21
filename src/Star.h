#pragma once

#include "ofMain.h"

class Star
{
public:
	Star(ofVec3f loc, int col, float starSize);
	~Star(void);

	ofVec3f location;
	int color;
	float baseSize;

};