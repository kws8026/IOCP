#pragma once
#include "Object.h"
#include <iostream>
using namespace std;



class Renderer
{
	static const int len = 119;
	static const int row = 25;
	static const int ui = 3;
	char screen[row+1][len + 1];
	Position origin;
	Position text;
public:
	Renderer();
	~Renderer();
	void clearAll();
	void clearPart(int num);
	void draw(Texture* texture, Vector2D pos);
	void draw(char* texture, Vector2D pos);
	void render();

	static int getLen() { return len; }
	static int getUI() { return ui; }
};

