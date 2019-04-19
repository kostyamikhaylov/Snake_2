#include "snake.h"


int main ()
{
	View *v = View::Get ("tui");
	Snake s1;
	Game new_game;
	Human h(&s1, &new_game);
	new_game.Add (&s1);


	Snake s2(1);
	AI ai1(&s2, &new_game);
	new_game.Add (&s2);


	Snake s3(2);
	AI ai2(&s3, &new_game);
	new_game.Add (&s3);
	
	v->SetModel (&new_game);
	v->Draw ();
	v->Run ();

	return 0;
}

