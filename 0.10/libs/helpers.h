#ifndef helpers_h
#define helpers_h

#include <gst/gst.h>

/* Audio */
GstElement *create_audio_src();

GstElement *create_fsioaudio_src();

GstElement *create_audiotest_src();

GstElement *create_audio_sink();

GstElement *create_fsioaudio_sink();

GstElement *create_audio_enc(gchar *enc);

GstElement *create_audioconvert();

GstElement *create_audioresample();
/* /Audio */

/* Video */
GstElement *create_video_src();

GstElement *create_fsiovideo_src();

GstElement *create_videotest_src();

GstElement *create_video_sink();

GstElement *create_fsiovideo_sink();

GstElement *create_video_caps();

GstElement *create_video_enc(gchar *enc);

GstElement *create_video_mux(gchar *mux);

GstElement *create_ffmpegcolorspace();

GstElement *create_videoscale();

GstElement *create_videorate();
/* /Video */

/* File */
GstElement *create_file_src();

GstElement *create_file_sink();
/* /File */

/* Utils */
GstBus *get_bus(GstElement *pipeline);

GstElement *create_pipeline();

GstElement *create_queue();

GstElement *create_decodebin();

GstElement *create_uridecodebin();
/* /Utils*/

/* Funcs */
void on_pad_added(GstElement *element, GstPad *pad, GstElement *pipeline);

gboolean async_bus_cb(GstBus *bus, GstMessage *message, gpointer user_data);
/* /Funcs*/

#endif