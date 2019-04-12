#include "view.h"


View *View::inst = nullptr;
int View::count = 0;

struct termios cur_mode, old_mode;

void handlerErr (int signal)
{
	View *v = View::Get ("tui");
	v->~View ();
	exit (EXIT_FAILURE);
}

void handlerWinCh (int signal)
{
	View *v = View::Get ("tui");
	v->Rebuild ();
	v->Draw ();
	v->Destroy ();
}

View::View ()
{
	signal (SIGINT, handlerErr);
	signal (SIGQUIT, handlerErr);
	signal (SIGTERM, handlerErr);
	signal (SIGSEGV, handlerErr);
	signal (SIGKILL, handlerErr);
	signal (SIGWINCH, handlerWinCh);

	tcgetattr (STDIN_FILENO, &cur_mode);
	old_mode = cur_mode;
	cfmakeraw (&cur_mode);
	tcsetattr (STDIN_FILENO, TCSANOW, &cur_mode);
	
	Rebuild ();
}

View::~View ()
{
	tcsetattr (STDIN_FILENO, TCSAFLUSH, &old_mode);
}

View *View::Get (const char* type)
{
	if (!strcmp (type, "tui"))
	{
		count++;
		if (inst)
			return inst;
		inst = new View;
		return inst;
	}
	else
	{
		printf ("Can't recognize type: %s\n", type);
		return nullptr;
	}
}

void View::Destroy ()
{
	count--;
	if (count <= 0)
	{
		delete inst;
		this->~View ();
	}
}

void View::Draw ()
{
	ClearScreen ();

	XHashLine (0);
	XHashLine (YSize () - 1);

	YHashLine (0);
	YHashLine (XSize () - 1);

	game->Visit (std::bind (&View::Paint, this, std::placeholders::_1, std::placeholders::_2));
	
	Gotoxy (XSize () / 2, YSize () / 2);
//	Gotoxy (0, YSize ());
	fflush (stdout);
}

void View::Paint (Coord point, Direction dir)
{
	Gotoxy (point.first, point.second);
	switch (dir)
	{
		case UP:
			printf ("^");
			break;
		
		case DOWN:
			printf ("v");
			break;

		case RIGHT:
			printf (">");
			break;

		case LEFT:
			printf ("<");
			break;

		case NOWHERE:
			printf ("0");
			break;
	
		default:
			printf ("?");
			break;
	}
}

void View::Rebuild ()
{
	struct winsize size;
	ioctl (STDOUT_FILENO, TIOCGWINSZ, &size);
	x = size.ws_col;
	y = size.ws_row;
}

void View::Run ()
{
	char cmd;
	while (0 < read (STDIN_FILENO, &cmd, 1))
	{
		if (onkey_delegate == nullptr)
		{
			fprintf (stderr, "No onkey_delegate\n");
			break;
		}
		
		onkey_delegate->OnKey (cmd);
	}
}

void View::ClearScreen ()
{
    printf("\e[H\e[J");
}

void View::Gotoxy (int x, int y)
{
    printf("\e[%d;%dH", y, x);
}

void View::XHashLine (int y)
{
	for (int i = 0; i < XSize (); i++)
	{
		Gotoxy (i, y);
		printf ("#");
	}
}

void View::YHashLine (int x)
{
	for (int i = 0; i < YSize (); i++)
	{
		Gotoxy (x, i);
		printf ("#");
	}
}

int View::XSize ()
{
	return x;
}

int View::YSize ()
{
	return y;
}

