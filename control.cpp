#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "view.h"

#include "control.h"

void Human::OnKey (char key)
{
	View *v = View::Get ("tui");
	
	switch (key)
	{
		case 'q':
			delete v;
			exit (EXIT_SUCCESS);
			break;
		case 'w':
			printf ("UP");
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
	game->Move ();
	v->Draw ();
	v->Destroy ();
}

Human::Human (Snake *s, Game *g):
	Control (s),
	game (g)
{
	View *v = View::Get ("tui");
	v->SetOnKey (this);
	v->Destroy ();
}

