#include "Star.h"


Star::Star(ofVec3f loc, ofColor col, float starSize)
{
	location = loc;
	color = col;
	baseSize = starSize;
}


Star::~Star(void)
{
}
