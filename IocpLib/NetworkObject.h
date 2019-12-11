#pragma once
#include "Objectpool.h"

struct stPacket;

class NetworkObject : public cObjectPool<NetworkObject>
{
protected:
	float			x;
	float			y;
	float			axis;
	unsigned char	state;
public:
	NetworkObject():state(0), axis(0),x(0),y(0) {};
	~NetworkObject() {};
	void PackPacket(stPacket* target);
	void SetPos(float _x, float _y) {
		x = _x;
		y = _y;
	};
	void GetPos(float& _x, float& _y) {
		_x = x;
		_y = y;
	}
};

