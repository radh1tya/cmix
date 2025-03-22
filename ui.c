#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>

#include "data.h"

float
turn_to_percent(void)
{
	cvolume.channels = 2;
cvolume.values[0] = devicelist[0].volume.values[0];
cvolume.values[1] = devicelist[0].volume.values[1];
	float vol;
	vol = pa_cvolume_avg(&cvolume) / (float)PA_VOLUME_NORM * 100;
	return (float)round(vol);
}

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
	printw("Volume Percent: %.0f\n", turn_to_percent());
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

			case 73:	
			case 105:
			clear();
			print_sinks();
			break;

			case 81:
			case 113:
			clear();
			endwin();
			exit(0);
			break;

			default:
			refresh();
			break;
		}
		refresh();
	}
	
	endwin();
}

