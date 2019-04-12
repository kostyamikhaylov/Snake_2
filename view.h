#ifndef VIEW_H
#define VIEW_H

#include "model.h"
#include "keypressable.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <list>
#include <utility>
using namespace std;


class View
{
	public:
		View ();
		~View ();
		static View *Get (const char* type = nullptr);
		void Destroy ();
		void Draw ();
		void Rebuild ();
		void Run ();
		void ClearScreen ();
		void Gotoxy (int x, int y);
		void XHashLine (int y);
		void YHashLine (int x);
		int XSize ();
		int YSize ();
		void Paint (Coord point, Direction dir);
		void SetOnKey (KeyPressable *f)
		{
			onkey_delegate = f;
		}
		void SetModel (Game *g)
		{
			game = g;
		}

	private:
		int x;
		int y;
		static View *inst;
		static int count;
		KeyPressable *onkey_delegate;
		Game *game;
};

#endif //VIEW_H

