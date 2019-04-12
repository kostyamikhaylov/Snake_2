#include "model.h"

void Game::Visit (SnakePainter p)
{

	for (auto s : snakes)
	{
		bool flag = true;
		for (auto c : s->body)
		{
			if (flag)
			{
				p (c, s->dir);
				flag = false;
				continue;
			}

			p (c, NOWHERE);
		}
	}
}

void Game::Add (Snake *s)
{
	snakes.push_back (s);
}

void Game::Move ()
{
	for (auto s : snakes)
		s->Move ();
}

Snake::Snake ()
{
	Coord x;
	x = {5, 9};
	body.push_back (x);
	x = {5, 10};
	body.push_back (x);
	x = {5, 11};
	body.push_back (x);
	x = {6, 11};
	body.push_back (x);
	x = {7, 11};
	body.push_back (x);

	dir = UP;
}

void Snake::ChangeDirection (Direction d)
{
	dir = d;
}

void Snake::Move ()
{
	Coord a = body.front ();
	switch (dir)
	{
		case UP:
			a.second--;
			break;
		case LEFT:
			a.first--;
			break;
		case RIGHT:
			a.first++;
			break;
		case DOWN:
			a.second++;
			break;
		default:
			break;
	}

	body.push_front (a);
	body.pop_back ();
}

