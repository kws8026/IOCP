#include "Renderer.h"



Renderer::Renderer(): origin(Borland::wherex(),Borland::wherey()),
text(Borland::wherex(), Borland::wherey()+ row)
{
}


Renderer::~Renderer()
{
}

void Renderer::clearAll()
{
	memset(&screen, ' ', (len+1)*row);
	for(int i = 0; i < row-1; i++)
		screen[i][len] = '\n';
	screen[row-1][len] = '\0';
}

void Renderer::clearPart(int num)
{
	if (num > row-1 || num < 0)
		return;
	memset(&screen[num], ' ', len);
	if(num == row-1)
		screen[num][len] = '\0';
	else
		screen[num][len] = '\n';
}

void Renderer::draw(Texture* texture, Vector2D pos)
{
	if (texture == nullptr)
		return;
	//int size = strlen(texture->shape);
	memcpy(&screen[(int)pos.y][(int)pos.x], &texture->shape, 1);
	Borland::gotoxy(&text);
}

void Renderer::draw(char * text, Vector2D pos)
{
	if (text == nullptr)
		return;
	memcpy(&screen[(int)pos.y][(int)pos.x], text, strlen(text));

}


void Renderer::render()
{
	Borland::gotoxy(&origin);
	printf("%s", screen);
	Borland::gotoxy(&text);
}