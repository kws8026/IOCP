#include "Object.h"



Object::Object()
{
}

Object::~Object()
{
}


Texture* Object::getTexture()
{
	return ptexture;
}

void Object::movePos(int vectorX, int vectorY, Rect stage)
{
	pos.x += vectorX;
	pos.y += vectorY;
	if (stage.width < pos.x)
		pos.x = stage.width;
	else if( 0 > pos.x)
		pos.x = 0;
	if (stage.height <= pos.y)
		pos.y = stage.height;
	else if (0 > pos.y)
		pos.y = 0;
	if(network != nullptr)
		network->SetPos(pos.x, pos.y);
}
