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

#define BUFFER_SIZE 1024

pa_context *context;
pa_mainloop *mainloop;
pa_mainloop_api *mainloop_api;
pa_operation *operation;


int 
main(int argc, char *argv[])
{
	mainloop = pa_mainloop_new();
	if (mainloop = NULL)
	{
		printf("mainloop failed :(\n");
		return 1;
	}

	mainloop_api = pa_mainloop_get_api(mainloop);
	if (mainloop_api = NULL)
	{
		printf("mainloop_api failed :(\n");
		return 1;
	}

	context = pa_context_new(mainloop_api, "cmix");
	if (context = NULL)
	{
		printf("context failed :(\n");
		return 1;
	}

	if(pa_context_connect(context,
			      NULL,
			      PA_CONTEXT_NOAUTOSPAWN, 
			      NULL) < 0)
	{
		printf("pa_context_connect() failed :(");
		return 1;
	}

	return 0;
}
