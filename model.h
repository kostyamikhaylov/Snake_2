#ifndef MODEL_H
#define MODEL_H

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
#include <functional>
using namespace std;

using Coord = pair<int, int>;
using Rabbit = Coord;


enum Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NOWHERE
};

using SnakePainter = std::function<void (Coord point, Direction d)>;

class Snake
{
	public:
		Snake ();
		
		Snake (const Snake& s):
		body (s.body),
		dir (s.dir)
		{}
		
		void ChangeDirection (Direction d);
		Snake& operator = (const Snake& s)
		{
			body = s.body;
			dir = s.dir;
			return *this;
		}
		void Move ();

//	private:
		list<Coord> body;
		Direction dir;
};

class Game
{
	public:
		void Visit (SnakePainter p);
		void Add (Snake *s);
		void Move ();

		list <Snake *> snakes;
		list <Rabbit> rabbits;
};


#endif //MODEL_H

