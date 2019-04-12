#include "snake.h"


int main ()
{
	View *v = View::Get ("tui");
	Snake s;
	Game new_game;
	Human h(&s, &new_game);
	new_game.Add (&s);
	
	v->SetModel (&new_game);
	v->Draw ();
	v->Run ();
	v->Destroy ();

	return 0;
}

