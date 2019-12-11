#pragma once
#include "Renderer.h"
#include "Singleton.h"
#include <map>

class cServerSession;
class Manager :public cSingleton<Manager>
{
	cServerSession*	pSession;
	Renderer*		pRenderer;
	Textures*		pTextures;
	std::map<DWORD,Object*> objects;
public:
	Manager();
	~Manager();
	int			initializing(Renderer* renderer, Textures* textures, cServerSession* session);
	bool		isPointInObject(Vector2D point, Object* object);
	bool		isObjectInObject(Object* target, Object* base);
	void		movePos(float x, float y);
	int			update();
	int			draw();
	Vector2D	GetPos();
	void		SetPlayer(Object* player);
};

