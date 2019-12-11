#include "Manager.h"
#include "ServerSession.h"

Manager::Manager()
	:  pTextures(nullptr), pRenderer(nullptr)
{
}

Manager::~Manager()
{
	auto iter = objects.begin();
	for (; iter != objects.end(); iter++){
		auto object = iter->second;
		if (object == nullptr)
			continue;
		iter->second = nullptr;
		delete object;
	}
}

int Manager::initializing(Renderer* renderer, Textures* textures, cServerSession* session)
{
	pSession = session;
	pRenderer = renderer;
	pTextures = textures;
	return 0;
}

bool Manager::isPointInObject(Vector2D point, Object * object)
{
	if(object == nullptr || object->getTexture() == nullptr)
		return false;
	//int len = strlen(object->getTexture()->shape);
	if(point.x < object->getPos().x || point.x > object->getPos().x)
		return false;
	else if (point.y < object->getPos().y || point.y > object->getPos().y)
		return false;
	return true;
}

bool Manager::isObjectInObject(Object * target, Object * base)
{
	if(target == nullptr || base == nullptr)
		return false;
	if (target->getPos().x > base->getPos().x || 
		target->getPos().x < base->getPos().x )
		return false;
	return true;
}

void Manager::movePos(float x, float y)
{
	objects[pSession->GetId()]->movePos(x, y, { 120,25 });
};

int Manager::update()
{
	FastSpinlockGuard lock(lock_mng);
	auto others = pSession->GetOthers();
	if (others.size() > objects.size()) {
		auto iter = others.begin();
		for (; iter != others.end(); iter++) {
			DWORD id = iter->first;
			if (objects.find(id) == objects.end()) {
				objects[id] = new Object(pTextures->peek(), iter->second);
				LOG("new Object Created : %d", id);
			}
		}
	}
	draw();
	return 0;
}

int Manager::draw()
{
	auto iter = objects.begin();
	for (; iter != objects.end(); iter++) {
		Object* object = iter->second;
		if (object == nullptr)
			continue;
		object->update();
		pRenderer->draw(object->getTexture(), object->getPos());
	}
	return 0;
}

Vector2D Manager::GetPos()
{
	if (objects.find(pSession->GetId()) == objects.end())
		return { 0,0 };
	return objects[pSession->GetId()]->getPos();
}

void Manager::SetPlayer(Object* player)
{
	FastSpinlockGuard lock(lock_mng);
	std::map<DWORD,NetworkObject*> others = pSession->GetOthers();
	DWORD id = pSession->GetId();
	auto newObject = NEW(NetworkObject);
	others[id] = newObject;
	player->setNetwork(newObject);
}
