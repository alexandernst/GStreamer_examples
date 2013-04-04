#ifndef helpers_h
#define helpers_h

#include <gst/gst.h>

/* Audio */
GstElement *create_fsioaudio_src();

GstElement *create_fsioaudio_sink();
/* /Audio */

/* Video */
GstElement *create_fsiovideo_src();

GstElement *create_fsiovideo_sink();
/* /Video */

/* Funcs */
void on_pad_added(GstElement *element, GstPad *pad, GstElement *pipeline);

gboolean async_bus_cb(GstBus *bus, GstMessage *message, gpointer user_data);
/* /Funcs*/

#endif