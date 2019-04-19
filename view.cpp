#ifndef VIEW_CPP
#define VIEW_CPP

#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>

#include "view.h"

View *View::inst = nullptr;

struct termios cur_mode, old_mode;

int ControlTimer ();

void handlerErr (int signal)
{
	fprintf (stderr, "Program received signal %d\n", signal);
	fflush (stdout);
	exit (EXIT_FAILURE);
}

void handlerWinCh (int signal)
{
	View *v = View::Get ("tui");
	v->Rebuild ();
	v->Draw ();
}

View::View ()
{
	signal (SIGINT, handlerErr);
	signal (SIGQUIT, handlerErr);
	signal (SIGTERM, handlerErr);
	signal (SIGSEGV, handlerErr);
	signal (SIGKILL, handlerErr);
	signal (SIGWINCH, handlerWinCh);
	siginterrupt (SIGWINCH, 0);

	tcgetattr (STDIN_FILENO, &cur_mode);
	old_mode = cur_mode;
	cfmakeraw (&cur_mode);
	tcsetattr (STDIN_FILENO, TCSANOW, &cur_mode);
	
	poll_stdin_master = new struct pollfd [nfds];
	poll_stdin_set = new struct pollfd [nfds];	

	atexit (Destroy);

	Rebuild ();
}

void View::Destroy ()
{
	delete inst;
}

View::~View ()
{
	tcsetattr (STDIN_FILENO, TCSAFLUSH, &old_mode);
	delete [] poll_stdin_master;
	delete [] poll_stdin_set;
	printf ("Goodbye\n");
}

View *View::Get (const char* type)
{
	if (!inst)
	{
		inst = new View;
		return inst;
	}
	else
		return inst;
}

void View::Draw ()
{
	ClearScreen ();

	game->Visit (std::bind (&View::Paint, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	
	Gotoxy (0, YSize ());
	fflush (stdout);
}

void View::Paint (Coord point, Direction dir, Color color)
{
	Gotoxy (point.first, point.second);
	switch (dir)
	{
		case UP:
			printf ("\e[%dm^\e[0m", color);
			break;
		
		case DOWN:
			printf ("\e[%dmv\e[0m", color);
			break;

		case RIGHT:
			printf ("\e[%dm>\e[0m", color);
			break;

		case LEFT:
			printf ("\e[%dm<\e[0m", color);
			break;

		case SNAKE_BODY:
			printf ("\e[%dm0\e[0m", color);
			break;
	
		case RABBIT:
			printf ("\e[%dm@\e[0m", color);
			break;
	
		case WALL:
			printf ("\e[%dm#\e[0m", color);
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
	pair <int, Timeoutable> cur_timer = timer_;
	int fds_ready = -1;
	poll_stdin_master[0].fd = STDIN_FILENO;
	poll_stdin_master[0].events = POLLIN;

	while (1)
	{
/*
		for (pair <int, Timeoutable>& x : timer_)
		{
			if (x.first <= 0)
			{
				//timer_.remove (x);
			}
		}

		//time_passed = ControlTimer ();

		for (pair <int, Timeoutable>& x : timer_)
		{
			x.first -= time_passed;
			if (x.first <= 0)
			{
				x.second ();
				Draw ();
			}
		}
*/
/*
		(timer_.front ()).first -= time_passed;
		if ((timer_.front ()).first < 0)
		{
			pair <int, Timeoutable> p = timer_.front ();
			timer_.pop_front ();
			p.second ();
			Draw ();
			//printf ("here\n");
			//fflush (stdout);
		}
		(timer_.back ()).first -= time_passed;
*/
/*
		for (pair <int, Timeoutable>& x : timer_)
		{
			x.first -= time_passed;
			if (x.first <= 0)
			{
				pair <int, Timeoutable> tmp = x;
				cur_time.pop_front ();			//????
				tmp.second ();
				Draw ();
			}
		}
*/

		cur_timer.first -= ControlTimer ();
		if (cur_timer.first <= 0)
		{
			cur_timer.second ();
			cur_timer.first = timer_.first;
			Draw ();
		}

		for (unsigned int i = 0; i < nfds; i++)
			poll_stdin_set[i] = poll_stdin_master[i];
		
		
		while (-1 == (fds_ready = poll (poll_stdin_set, nfds, timer_.first )))
		{
			if (errno == EINTR)
			{
				cur_timer.first -= ControlTimer ();
				if (cur_timer.first <= 0)
				{
					cur_timer.second ();
					cur_timer.first = timer_.first;
					Draw ();
				}
			
				continue;
			}
			else
			{
				fprintf (stderr, "Poll syscall error\n");
				exit (EXIT_FAILURE);
			}
		}


		if (fds_ready == 0)
		{
			continue;
		}
		for (unsigned int i = 0; i < nfds; i++)
		{
			if (poll_stdin_set[i].revents == POLLIN)
			{
				if (-1 == read (poll_stdin_set[i].fd, &cmd, 1))
				{
					fprintf (stderr, "Read syscall error\n");
					break;
				}

				if (onkey_delegate)
					onkey_delegate->OnKey (cmd);
			}
		}
	}
}

int ControlTimer ()
{
	static struct timeval t1, t0 = { .tv_sec = 0, .tv_usec = 0 };
	
	gettimeofday (&t1, NULL);
	int ret_value = (t1.tv_sec	- t0.tv_sec) 	* 1000 +
					(t1.tv_usec	- t0.tv_usec)	/ 1000;
	gettimeofday (&t0, NULL);
	return ret_value;
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

#endif //VIEW_CPP

