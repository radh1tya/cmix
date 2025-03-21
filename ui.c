#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

#include "data.h"

void
print_sinks(void)
{
	if (pa_context_get_state(context) == PA_CONTEXT_READY) {
		printw("Pulseaudio Connected :)\n");
	}
	for (int i = 0; i < sink_count; i++) {
		printw("Sinks found: %d\n", sink_count);

		printw("Sinks Name: %s\n", devicelist[i].name);
		printw("Sinks Index: %d\n", devicelist[i].index);
		printw("Sinks Volume[0]: %d\n", devicelist[i].volume.values[0]);
		printw("Sinks Volume[1]: %d\n", devicelist[i].volume.values[1]);
	}	

}
void
interface(void)
{
	int ch;

	initscr();
	keypad(stdscr, TRUE);
	while(1)
	{
		ch = getch();
		switch (ch)
		{
			case KEY_UP:
			printw("Hello");
			break;

			case 65:	
			print_sinks();
			break;
			case 97:
			print_sinks();
			break;

			default:
			break;
		}
		refresh();
	}
	
	endwin();
}

