#ifndef DATA_H
#define DATA_H

#include <pulse/pulseaudio.h>

typedef struct {
    char name[512];
    uint32_t index;
    char description[256];
    pa_cvolume volume;
} pa_devicelist_t;

extern pa_devicelist_t devicelist[10];

static pa_context *context;
static pa_mainloop *mainloop;
static pa_mainloop_api *mainloop_api;
static pa_operation *operation;

pa_cvolume cvolume;

int ready = 0;
int sink_count = 0;

#endif
