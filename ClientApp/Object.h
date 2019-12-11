#pragma once
#include "Util.h"
#include "NetworkObject.h"

class Object
{
protected:
	NetworkObject*	network;
	Texture*		ptexture;
	Vector2D		pos;
public:
	Object();
	Object(Texture* T, NetworkObject* net = nullptr,Vector2D p = { 0,0 })
		: pos(p) , ptexture(T), network(net)
	{
	}
	virtual ~Object();

	Vector2D	getPos() { return pos; }
	void		setPos(Vector2D num) { 
		pos = num; 
	}
	void		setNetwork(NetworkObject* other) {
		network = other;
	}
	void		update() {
		float x, y;
		network->GetPos(x, y);
		pos.x = x;
		pos.y = y;
	}
	virtual Texture* getTexture();
	virtual void movePos(int vectorX, int vectorY, Rect stage);
};

