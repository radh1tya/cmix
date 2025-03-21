#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <signal.h>
#include <assert.h>
#include <pulse/pulseaudio.h>

#define BUFFER_SIZE 1024
#define MAX_SINKS 10

#include "ui.c"
#include "data.h"

pa_devicelist_t devicelist[10];

static void
context_state_callback_status(pa_context *context, void *userdata)
{
    int *pa_ready_status = userdata;

    switch (pa_context_get_state(context)) {
    case PA_CONTEXT_READY:
        *pa_ready_status = 1;
        break;
    case PA_CONTEXT_FAILED:
	printf("PA_CONTEXT_FAILED\n");
	break;
    case PA_CONTEXT_TERMINATED:
        printf("PA_CONTEXT_TERMINATED\n");
	*pa_ready_status = 2;
        break;
    default:
	//printf("pa_context_get_state: %d\n", pa_context_get_state(context)); 
        break;
    }
    //printf("Current ready: %d\n", *pa_ready_status);
}

static void
sinklist_callback(pa_context *context, 
		  const pa_sink_info *sink_info, 
		  int eol, 
		  void *userdata)
{
    pa_devicelist_t *devicelist = userdata;

    if (eol > 0) {
        return;
    }

    if (sink_info == NULL) {
        return;
    }

    if (sink_count >= MAX_SINKS) {
        printf("Sink list full\n");
        return;
    }

    strncpy(devicelist[sink_count].name, sink_info->name, 511);
    devicelist[sink_count].index = sink_info->index;
    strncpy(devicelist[sink_count].description, sink_info->description, 255);
    devicelist[sink_count].volume = sink_info->volume;
    sink_count++;
}

static void
get_sinks()
{
    if (context == NULL || ready != 1) {
        printf("Context not ready :(\n");
        return;
    }

    operation = pa_context_get_sink_info_list(context, 
					      sinklist_callback, 
					      devicelist);
    if (operation == NULL) {
        printf("Operation null :(\n");
        return;
    }

    enum pa_operation_state state = pa_operation_get_state(operation);

    while (state == PA_OPERATION_RUNNING) {
        if (pa_mainloop_iterate(mainloop, 1, NULL) < 0) {
            printf("Mainloop iterate error :(\n");
            pa_operation_unref(operation);
            return;
        }
        state = pa_operation_get_state(operation);
    }

    if (state == PA_OPERATION_CANCELLED) {
        printf("Operation cancelled :(\n");
        pa_operation_unref(operation);
        return;
    }

    pa_operation_unref(operation);
    
}

	void
destroy(void)
{
	if (context != NULL) {
		pa_context_unref(context);
	}
	if (mainloop != NULL) {
		pa_mainloop_free(mainloop);
	}
}

	int
setup_server(void)
{
	mainloop = pa_mainloop_new();
	if (mainloop == NULL) {
		printf("Mainloop failed :(\n");
		return 1;
	}

	mainloop_api = pa_mainloop_get_api(mainloop);
	if (mainloop_api == NULL) {
		printf("Mainloop API failed :(\n");
		pa_mainloop_free(mainloop);
		return 1;
	}

	context = pa_context_new(mainloop_api, "cmix");
	if (context == NULL) {
		printf("Context failed :(\n");
		destroy();
		return 1;
	}

	pa_context_set_state_callback(context, 
				      context_state_callback_status, 
				      &ready);

	if (pa_context_connect(context, NULL, 0, NULL) < 0) {
		printf("Context connect failed :(\n");
		destroy();
		return 1;
	}

	while ( ready == 0 ) {
		pa_mainloop_iterate(mainloop, 1, NULL);
		enum pa_context_state state = pa_context_get_state(context);
		if (state == PA_CONTEXT_FAILED || state == PA_CONTEXT_TERMINATED)
		{
			printf("connection failed early :(\n");
			return 1;
		}
	}
	return (ready == 1) ? 0 : 1;
}

	int
main(int argc, char *argv[])
{
	if (setup_server() == 1) {
		printf("Setup server failed :(\n");
		return 1;
	}

	if (ready != 1) {
		printf("Connection failed :(\n");
		destroy();
		return 1;
	}
	get_sinks();
	interface();
	destroy();
	return 0;
}
