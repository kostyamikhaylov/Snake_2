#ifndef MODEL_CPP
#define MODEL_CPP

#include <stdlib.h>
#include <sys/time.h>

#include "view.h"
#include "control.h"

#include "model.h"

Game::Game ()
{
	AddWall ();
	View::Get ()->SetOnTimer ({TIME_PER_STEP_MILLI_SECS, std::bind (&Game::Move, this)});
}

void Game::Visit (SnakePainter p)
{
	for (const Snake *s : snakes)
	{
		bool flag = true;
		for (const Coord& c : s->body)
		{
			if (flag)
			{
				p (c, s->dir, s->color);
				flag = false;
				continue;
			}

			p (c, SNAKE_BODY, s->color);
		}
	}
	
	for (const Wall& w : walls)
		for (const Coord& c : w)
			p (c, WALL, WHITE);

	for (const Coord& r : rabbits)
	{
		p (r, RABBIT, BLUE);
	}
}

void Game::Add (Snake *s)
{
	snakes.push_back (s);
}

void Game::RemoveSnake (Snake *s)
{
	for (Snake *sn : snakes)
	{
		if (sn == s)
		{
			snakes.remove (s);
			return ;
		}
	}
}

void Game::Move ()
{
	list <Snake *> snakes_to_be_removed;
	for (Control *c : controls)
	{
		if (c->IsAI ())
			c->OnMove ();
	}

	for (Snake *s : snakes)
	{
		switch (CheckNewStep (s))
		{
			case OBJ_EMPTY_PLACE:
				s->Move ();
				break;

			case OBJ_RABBIT:
				s->EatRabbit ();
				break;

			case OBJ_SNAKE_BODY:
				s->Move ();
				break;

			case OBJ_SNAKE_HEAD:
				GameOver ();
				break;
			
			case OBJ_WALL:
				snakes_to_be_removed.push_back (s);
				break;
			
			default:
				GameOver ();
				break;	
		}
	}

	for (Snake *s : snakes_to_be_removed)
		snakes.remove (s);
	
	struct timeval t;
	gettimeofday (&t, nullptr);
	srand ((unsigned int) (t.tv_sec * t.tv_usec));
	if ((rand () % 1) == 0)
	{
		AddRabbit ();
	}
}
	
void Game::AddRabbit ()
{
	while (1)
	{
		Coord r ({rand () % (View::Get ())->XSize (), rand () % (View::Get ())->YSize ()});
		if (IsOccupied (r))
		{
			continue;
		}
		else
		{
			rabbits.push_back (r);
			break;
		}
	}
}

Object Game::CheckNewStep (Snake *s)
{
	Coord new_place = s->body.front ();
	switch (s->dir)
	{
		case UP:
			new_place.second--;
			break;
		case LEFT:
			new_place.first--;
			break;
		case RIGHT:
			new_place.first++;
			break;
		case DOWN:
			new_place.second++;
			break;
		default:
			break;
	}

	for (const Rabbit& r : rabbits)
	{
		if (new_place == r)
		{
			rabbits.remove (r);
			return OBJ_RABBIT;
		}
	}

	for (Snake *snk : snakes)
	{
		if (new_place == snk->body.front ())
			return OBJ_SNAKE_HEAD;

		for (const Coord& b : snk->body)
		{
			if (new_place == b)
			{
				snk->RemoveTailFromPosition (b);
				return OBJ_SNAKE_BODY;
			}
		}
	}

	for (Wall w : walls)
		for (Coord p : w)
			if (new_place == p)
				return OBJ_WALL;

	return OBJ_EMPTY_PLACE;
}

void Game::AddWall ()
{
	Wall border;
	Coord size ({(View::Get ())->XSize (), (View::Get ())->YSize ()});
	int i = 0;
	for (i = 1; i < size.first - 1; i++)
		border.push_back ({i, 1});
	for (i = 1; i < size.second - 1; i++)
		border.push_back ({size.first - 1, i});
	for (i = size.first - 1; i > 1; i--)
		border.push_back ({i, size.second - 1});
	for (i = size.second - 1; i > 1; i--)
		border.push_back ({1, i});

	walls.push_back (border);
}

void Game::AddControl (Control *c)
{
	controls.push_back (c);
}

int Game::IsOccupied (const Coord& p)
{
	if (p.first == 0 || p.second == 0)  //for walls
		return 1;						//???

	for (const Snake *s : snakes)
		for (const Coord& c : s->body)
			if (c == p)
				return 1;
	
	for (const Wall& w : walls)
		for (const Coord& c : w)
			if (c == p)
				return 1;

	for (const Coord& r : rabbits)
		if (r == p)
			return 1;

	return 0;
}

experimental::optional <Coord> Game::Nearest (const Coord& p)
{
	experimental::optional <Coord> nearest;
	experimental::optional <int> distance;
	int cur_distance;

	bool first = true;
	for (const Coord& r : rabbits)
	{
		if (first)
		{
			distance = 100000;
			first = false;
		}

		if ((cur_distance = p.Distance (r)) < distance)
		{
			distance = cur_distance;
			nearest = r;
		}
	}

	return nearest;
}

void Game::GameOver ()
{
	exit (EXIT_SUCCESS);
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
	x = {7, 12};
	body.push_back (x);
	x = {7, 13};
	body.push_back (x);
	x = {7, 14};
	body.push_back (x);
	x = {8, 14};
	body.push_back (x);
	x = {9, 14};
	body.push_back (x);
	x = {10, 14};
	body.push_back (x);

	dir = UP;
}

Snake::Snake (int num)
{
	Coord x;
	x = {30, num * 10};
	body.push_back (x);
	x = {31, num * 10};
	body.push_back (x);
	x = {32, num * 10};
	body.push_back (x);
	x = {33, num * 10};
	body.push_back (x);
	x = {34, num * 10};
	body.push_back (x);
	x = {35, num * 10};

	dir = UP;
	color = (Color) (30 + num);
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

void Snake::EatRabbit ()
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
}

void Snake::RemoveTailFromPosition (const Coord& c)
{
	while (1)
	{
		if (body.back () == c)
			break;
		else
			body.pop_back ();
	}
}

int Coord::Distance (const Coord& p) const
{
	return abs (first - p.first) + abs (second - p.second);
}

#endif //MODEL_CPP

