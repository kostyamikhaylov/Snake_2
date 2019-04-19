#ifndef MODEL_H
#define MODEL_H

#include <utility>
#include <list>
#include <functional>
#include <experimental/optional>

#define TIME_PER_STEP_MILLI_SECS 200

using namespace std;

class Coord : public pair <int, int>
{
	public:
		using Base = pair <int, int>;
		int Distance (const Coord& p) const;
		using Base::Base;
};

using Rabbit = Coord;
using Wall = list <Coord>;

enum Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	SNAKE_BODY,
	RABBIT,
	WALL,
};

enum Object
{
	OBJ_EMPTY_PLACE,
	OBJ_WALL,
	OBJ_RABBIT,
	OBJ_SNAKE_BODY,
	OBJ_SNAKE_HEAD,
};

enum Color
{
	BLACK 	= 30,
	RED		= 31,
	GREEN	= 32,
	YELLOW 	= 33,
	BLUE	= 34,
	MAGENTA	= 35,
	CYAN 	= 36,
	WHITE	= 37,
};

using SnakePainter = std::function<void (Coord point, Direction d, Color color)>;

class Snake
{
	public:
		Snake ();
		
		Snake (const Snake& s):
		body (s.body),
		dir (s.dir),
		color (s.color)
		{}

		Snake (int num);
		
		void ChangeDirection (Direction d);
		Snake& operator = (const Snake& s)
		{
			body = s.body;
			dir = s.dir;
			return *this;
		}
		void Move ();
		void EatRabbit ();
		void RemoveTailFromPosition (const Coord& c);

//	private:
		list<Coord> body;
		Direction dir;

		Color color;
};

class Control;

class Game
{
	public:
		Game ();
		void Visit (SnakePainter p);
		void Add (Snake *s);
		void RemoveSnake (Snake *s);
		void Move ();
		void AddRabbit ();
		void AddWall ();
		void AddControl (Control *c);
		Object CheckNewStep (Snake *s);
		void GameOver ();
		int IsOccupied (const Coord& p);
		experimental::optional <Coord> Nearest (const Coord& p);

		list <Control *> controls;
		list <Snake *> snakes;
		list <Rabbit> rabbits;
		list <Wall> walls;
};


#endif //MODEL_H

