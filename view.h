#ifndef VIEW_H
#define VIEW_H

#include <poll.h>

#include "model.h"
#include "keypressable.h"

using namespace std;

using Timeoutable = std::function<void ()>;

class View
{
	public:
		View ();
		~View ();
		static View *Get (const char* type = nullptr);
		static void Destroy ();
		void Draw ();
		void Rebuild ();
		void Run ();
		void ClearScreen ();
		void Gotoxy (int x, int y);
		void XHashLine (int y);
		void YHashLine (int x);
		int XSize ();
		int YSize ();
		void Paint (Coord point, Direction dir, Color color);
		void SetOnKey (KeyPressable *f)
		{
			onkey_delegate = f;
		}
		void SetModel (Game *g)
		{
			game = g;
		}
		void SetOnTimer (pair <int, Timeoutable> t)
		{
			timer_ = t;
		}

	private:
		int x;
		int y;
		static View *inst;
		nfds_t nfds = 1;		//changable
		struct pollfd *poll_stdin_master;
		struct pollfd *poll_stdin_set;
		pair <int, Timeoutable> timer_;
		KeyPressable *onkey_delegate;
		Game *game;
};

#endif //VIEW_H

