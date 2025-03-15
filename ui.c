#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

#include "data.h"

void
interface(void)
{
	initscr();
	printw("Sinks Volume[0]: %d\n", devicelist[0].volume.values[0]);
	printw("Sinks Volume[1]: %d\n", devicelist[0].volume.values[1]);
	refresh();
	getch();
	endwin();
}

