#pragma once
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
using namespace std;

struct Texture
{
	char shape;
	Texture() {}
	Texture(char c) :  shape(c) {}
	operator char(){
		return shape;}
};

struct Position {
	int x;
	int y;
	Position(int x, int y) : x(x), y(y) {}
};

struct Rect {
	Position pos;
	int width, height;
	Rect():pos(0,0), width(0), height(0) {}
	Rect(int x, int y, int w, int h):pos(x, y), width(w), height(h) {}
	Rect(int w, int h): pos(0, 0), width(w), height(h) {}
};

struct Vector2D {
	float x, y;
	Vector2D() {}
	Vector2D(float x, float y) : x(x), y(y) {}
	bool operator==(Vector2D& v2)
	{
		return x == v2.x && y == v2.y ? true : false;
	}
};

class Textures
{
	Texture* ptexture;
	int size;
	int top;
public:
	Textures(int size) : size(size), top(-1),
		ptexture((Texture*)calloc(size, sizeof(Texture))) {}
	~Textures() {
		free(ptexture);
	}
	int isFull() {
		return top + 1 == size ? TRUE : FALSE;
	}
	int isEmpty() {
		return top == -1 ? TRUE : FALSE;
	}
	int push(Texture texture)
	{
		if (isFull())
			return -1;	// Increase the index number to insert
		ptexture[++top] = texture;
		return 0;
	}
	Texture* pop()
	{
		if (isEmpty())
			return nullptr;
		return &ptexture[top--];
		
	}
	Texture* peek()
	{
		// Check if stack is empty
		if (isEmpty())
			return nullptr;
		return &ptexture[top];
	}
};


class Borland {

public:
	static int wherex()
	{
		CONSOLE_SCREEN_BUFFER_INFO  csbiInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
		return csbiInfo.dwCursorPosition.X;
	}
	static int wherey()
	{
		CONSOLE_SCREEN_BUFFER_INFO  csbiInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
		return csbiInfo.dwCursorPosition.Y;
	}
	static void gotoxy(int x, int y)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _COORD{ (SHORT)x, (SHORT)y });
	}
	static void gotoxy(const Position* pos)
	{
		if (pos == nullptr)
			return;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _COORD{ (SHORT)pos->x, (SHORT)pos->y });
	}
};
