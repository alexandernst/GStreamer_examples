#include "helpers.h"

/* Audio */
GstElement *create_fsioaudio_src(){
	GstElement *fsioaudio_src;

	fsioaudio_src = gst_element_factory_make("fsioaudiosrc", NULL);
	g_assert(fsioaudio_src);

	if(g_getenv("AUDIOSRC")){
		g_object_set(fsioaudio_src, "source-pipeline", g_getenv("AUDIOSRC"), NULL);
	}

	return fsioaudio_src;
}

GstElement *create_fsioaudio_sink(){
	GstElement *fsioaudio_sink, *bin, *audioconvert, *audioresample, *audio_sink;
	GstPad *pad;

	fsioaudio_sink = gst_element_factory_make("fsioaudiosink", NULL);
	g_assert(fsioaudio_sink);

	if(g_getenv("AUDIOSINK")){
		g_object_set(fsioaudio_sink, "sink-pipeline", g_getenv("AUDIOSINK"), NULL);
	}else{
		bin = gst_bin_new("bin");
		g_assert(bin);

		audioconvert = gst_element_factory_make("audioconvert", NULL);
		g_assert(audioconvert);

		audioresample = gst_element_factory_make("audioresample", NULL);
		g_assert(audioresample);

		audio_sink = gst_element_factory_make("autoaudiosink", NULL);
		g_assert(audio_sink);

		gst_bin_add_many(GST_BIN(bin), audioconvert, audioresample, audio_sink, NULL);
		gst_element_link_many(audioconvert, audioresample, audio_sink, NULL);

		pad = gst_element_get_static_pad(audioconvert, "sink");
		gst_element_add_pad(bin, gst_ghost_pad_new("audiosink", pad));

		g_object_set(fsioaudio_sink, "sink-element", GST_BIN(bin), NULL);
	}

	return fsioaudio_sink;

}
/* /Audio */

/* Video */
GstElement *create_fsiovideo_src(){
	GstElement *fsiovideo_src;

	fsiovideo_src = gst_element_factory_make("fsiovideosrc", NULL);
	g_assert(fsiovideo_src);

	if(g_getenv("VIDEOSRC")){
		g_object_set(fsiovideo_src, "source-pipeline", g_getenv("VIDEOSRC"), NULL);
	}

	return fsiovideo_src;
}

GstElement *create_fsiovideo_sink(){
	GstElement *fsiovideo_sink, *bin, *video_caps, *ffmpegcolorspace, *videoscale, *video_sink;
	GstCaps *caps;
	GstPad *pad;

	fsiovideo_sink = gst_element_factory_make("fsiovideosink", NULL);
	g_assert(fsiovideo_sink);

	if(g_getenv("VIDEOSINK")){
		g_object_set(fsiovideo_sink, "sink-pipeline", g_getenv("VIDEOSINK"), NULL);
	}else{
		bin = gst_bin_new("bin");
		g_assert(bin);

		ffmpegcolorspace = gst_element_factory_make("ffmpegcolorspace", NULL);
		g_assert(ffmpegcolorspace);

		videoscale = gst_element_factory_make("videoscale", NULL);
		g_assert(videoscale);

		video_caps = gst_element_factory_make("capsfilter", NULL);
		g_assert(video_caps);
		caps = gst_caps_new_simple(
			"video/x-raw-rgb",
			"width", G_TYPE_INT, 800,
			"height", G_TYPE_INT, 480,
			"framerate", GST_TYPE_FRACTION, 15, 1,
			"bpp", G_TYPE_INT, 24,
			"depth", G_TYPE_INT, 24,
			NULL
		);
		g_object_set(video_caps, "caps", caps, NULL);
		gst_caps_unref(caps);

		video_sink = gst_element_factory_make("autovideosink", NULL);
		g_assert(video_sink);

		gst_bin_add_many(GST_BIN(bin), ffmpegcolorspace, videoscale, video_caps, video_sink, NULL);
		gst_element_link_many(ffmpegcolorspace, videoscale, video_caps, video_sink, NULL);

		pad = gst_element_get_static_pad(ffmpegcolorspace, "sink");
		gst_element_add_pad(bin, gst_ghost_pad_new("videosink", pad));

		g_object_set(fsiovideo_sink, "sink-element", GST_BIN(bin), NULL);
	}

	return fsiovideo_sink;
}
/* /Video */

/* Funcs */
void on_pad_added(GstElement *element, GstPad *pad, GstElement *pipeline){
	GstCaps *caps = gst_pad_get_caps(pad);
	gchar *prefix = gst_caps_to_string(caps);

	gchar *queuetype;
	if(g_str_has_prefix(prefix, "audio")){
		queuetype = "queuea";
	}else if(g_str_has_prefix(prefix, "video")){
		queuetype = "queuev";
	}else{
		return;
	}

	GstElement *queue = gst_bin_get_by_name(GST_BIN(pipeline), queuetype);
	if(queuetype != NULL){
		GstPad *sink = gst_element_get_pad(queue, "sink");
		gst_pad_link(pad, sink);
	}

	gst_caps_unref(caps);
}

gboolean async_bus_cb(GstBus *bus, GstMessage *message, gpointer user_data){
	switch (GST_MESSAGE_TYPE(message)){

		case GST_MESSAGE_ERROR: {
			GError *error = NULL;
			gchar *debug_str = NULL;
			gst_message_parse_error(message, &error, &debug_str);
			g_print("Got gst message: %s\n", error->message);
			GMainLoop *loop = user_data;
			g_main_loop_quit(loop);
		}
		break;

		case GST_MESSAGE_EOS:
			g_print("End-of-stream\n");
			GMainLoop *loop = user_data;
			g_main_loop_quit(loop);
		break;

		case GST_MESSAGE_WARNING: {
			GError *error = NULL;
			gchar *debug_str = NULL;
			gst_message_parse_warning(message, &error, &debug_str);
			g_print("Got gst message: %s\n", error->message);
		}
		break;

		default:
		break;

	}

	return TRUE;
}
/* /Funcs */