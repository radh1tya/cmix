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

static pa_context *context;
static pa_mainloop *mainloop;
static pa_mainloop_api *mainloop_api;
static pa_operation *operation;
static int ready = 0;

static void
context_state_callback_status(pa_context *context, void *userdata)
{
        int *pa_ready_status = userdata;

        switch (pa_context_get_state(context)) {
        case PA_CONTEXT_READY:
                *pa_ready_status = 1; 
                printf("Pulseaudio connected :)\n");
                break;
        case PA_CONTEXT_FAILED:
        case PA_CONTEXT_TERMINATED:
                *pa_ready_status = 2; 
                break;
        case PA_CONTEXT_UNCONNECTED:
        case PA_CONTEXT_CONNECTING:
        case PA_CONTEXT_AUTHORIZING:
        case PA_CONTEXT_SETTING_NAME:
        default:
                break; 
        }
}

void
destroy(void)
{
	if ( context != NULL ) {
	pa_context_unref(context);
	}
	
	if ( mainloop != NULL ) {
        pa_mainloop_free(mainloop);
	}
}

int
setup_server(void)
{
	mainloop = pa_mainloop_new();
	if (mainloop == NULL)
	{
		printf("mainloop failed :(\n");
		return 1;
	}

	mainloop_api = pa_mainloop_get_api(mainloop);
	if (mainloop_api == NULL)
	{
		printf("mainloop_api failed :(\n");
		return 1;
	}

	context = pa_context_new(mainloop_api, "cmix");
	if (context == NULL)
	{
		printf("context failed :(\n");
		return 1;
	}

	pa_context_set_state_callback(context,
				      context_state_callback_status,
				      &ready);


	if (pa_context_connect(context,
			       NULL,
			       0,
			       NULL) < 0)
	{
		printf("pa_context_connect() failed :(");
		return 1;
	}

	pa_mainloop_run(mainloop, NULL);

}

int 
main(int argc, char *argv[])
{

	setup_server();


	if (ready == 2) {
                printf("connection failed :(\n");
        }

	destroy();

	return (ready == 1) ? 0 : 1;
}
