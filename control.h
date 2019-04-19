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

		virtual bool IsAI () = 0;
		virtual void OnMove () {};

		Snake *snake;
};

class Human: Control, KeyPressable
{
	public:
		Human (Snake *s, Game *g);
		void OnKey (char key);
		bool IsAI ()
		{
			return false;
		}

		Game *game;
};

class AI: Control
{
	public:
		AI (Snake *s, Game *g);
		void OnMove ();
		bool IsAI ()
		{
			return true;
		}

		Game *game;
};

#endif //CONTROL_H

