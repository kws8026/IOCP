#pragma once

class cServerSession;
class Object;
struct Vector2D;

class ClientNetwork
{
	const char* serverAddr;
	cServerSession* pSession;
	char			buf[2048];
public:
	bool Intialize(const char* serveraddr);
	bool Connect();
	void Login(int id);
	void RefPlayer(Object* player);
	void Move(float x, float y);
	void Move(Vector2D pos);
	void CheckLogin(bool& flag);
	cServerSession* GetSeesion() {
		return pSession;
	};
};