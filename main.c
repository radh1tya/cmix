#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include <signal.h>
#include <assert.h>

#include <pulse/pulseaudio.h>
#include <pulse/rtclock.h>
#include <pulse/simple.h>
#include <pulse/error.h>

static pa_context *context = NULL;
static pa_mainloop *mainloop = NULL;
static pa_operation *operation = NULL;

void
motd(void)
{
	const char *motd_inside =
		"cmix - pulse audio volume control\n"
		"radhitya.org\n";
	printf("%s", motd_inside);
}
int main(void)
{
	motd();
}
