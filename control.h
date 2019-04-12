#ifndef CONTROL_H
#define CONTROL_H

using namespace std;

#include "model.h"
#include "keypressable.h"

class Control
{
	public:
		explicit Control (Snake *s)
		{
			snake = s;
		}

		Snake *snake;
};

class Human: Control, KeyPressable
{
	public:
		Human (Snake *s, Game *g);
		void OnKey (char key);

		Game *game;
};

#endif //CONTROL_H

