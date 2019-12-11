#include "ClientNetwork.h"
#include "Player.h"
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include "Manager.h"
#include "Object.h"
using namespace std;
#pragma comment( lib, "ws2_32.lib")

int main() {
	//if (INIT_LOG() == false) {
	//	return -1;
	//}
	ClientNetwork client;
	Renderer renderer;
	Textures textures(1);
	textures.push('@');
	Object player(textures.peek());
	client.Intialize("127.0.0.1");
	Manager::Instance()->initializing(&renderer, &textures, client.GetSeesion());
	while (true) {
		if (client.Connect())
		{	
			break;
		}
		Sleep(333);
	}
	bool bFlag = false;
	while (!bFlag) {
		int id;
		cin >> id;
		client.Login(id);
		Sleep(2000);
		client.CheckLogin(bFlag);
	}
	Manager::Instance()->SetPlayer(&player);
	player.movePos(0, 0, { 120,25 });
	char key;
	while (true) {
		renderer.clearAll();
		if (_kbhit()) {
			key = _getch();
			switch (key)
			{
			case 'w':
				player.movePos(0, -1, {120,25});
				break;
			case 's':
				player.movePos(0, 1, { 120,25 });
				break;
			case 'a':
				player.movePos(-1, 0, { 120,25 });
				break;
			case 'd':
				player.movePos(1, 0, { 120,25 });
				break;
			case 'p':
				return 0;
				break;
			default:
				break;
			}
		}
		client.Move(player.getPos());
		Manager::Instance()->update();
		Manager::Instance()->draw();
		renderer.render();
		Sleep(100);
	}
	return 0;
}