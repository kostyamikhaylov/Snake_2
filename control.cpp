#ifndef CONTROL_CPP
#define CONTROL_CPP

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <algorithm>

#include "view.h"

#include "control.h"

void Human::OnKey (char key)
{
	switch (key)
	{
		case 'q':
			exit (EXIT_SUCCESS);
			break;
		case 'w':
			snake->ChangeDirection (UP);
			break;
		case 'a':
			snake->ChangeDirection (LEFT);
			break;
		case 'd':
			snake->ChangeDirection (RIGHT);
			break;
		case 's':
			snake->ChangeDirection (DOWN);
			break;
		default:
			char str[22];
			sprintf (str, "Unknown command: '%c'\n", key);
			write (STDOUT_FILENO, str, strlen (str));

			break;
	}
}

Human::Human (Snake *s, Game *g):
	Control (s),
	game (g)
{
	View::Get ()->SetOnKey (this);
}

AI::AI (Snake *s, Game *g):
	Control (s),
	game (g)
{
	game->AddControl (this);
}

void AI::OnMove ()
{
	Direction d[2] = {SNAKE_BODY, SNAKE_BODY};
	int d_counter = 0;
	int dist_to_rabbit = 0;
	Coord head = snake->body.front ();
	experimental::optional <Coord> a = game->Nearest (head);

	if (!a)
	{
		snake->Move ();
		return ;
	}

	dist_to_rabbit = a->Distance (head);

	if (a->Distance ({head.first + 1, head.second}) < dist_to_rabbit)
		d[d_counter++] = RIGHT;
	
	if (a->Distance ({head.first, head.second + 1}) < dist_to_rabbit)
		d[d_counter++] = DOWN;
	
	if (a->Distance ({head.first - 1, head.second}) < dist_to_rabbit)
		d[d_counter++] = LEFT;
	
	if (a->Distance ({head.first, head.second - 1}) < dist_to_rabbit)
		d[d_counter++] = UP;

	random_shuffle (d, d + d_counter);

	snake->ChangeDirection (d[0]);
	
	return ;
}

#endif //CONTROL_CPP

