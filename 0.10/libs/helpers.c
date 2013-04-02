#include "helpers.h"

/* Audio */
GstElement *create_audio_src(){
	GstElement *audio_src;

	if(g_getenv("AUDIOSRC")){
		audio_src = gst_element_factory_make(g_getenv("AUDIOSRC"), NULL);
	}else{
		audio_src = gst_element_factory_make("autoaudiosrc", NULL);
	}
	g_assert(audio_src);

	return audio_src;
}

GstElement *create_fsioaudio_src(){
	GstElement *fsioaudio_src;

	fsioaudio_src = gst_element_factory_make("fsioaudiosrc", NULL);
	g_assert(fsioaudio_src);

	if(g_getenv("AUDIOSRC")){
		g_object_set(fsioaudio_src, "source-pipeline", g_getenv("AUDIOSRC"), NULL);
	}

	return fsioaudio_src;
}

GstElement *create_audiotest_src(){
	GstElement *audio_src;

	audio_src = gst_element_factory_make("audiotestsrc", NULL);
	g_assert(audio_src);

	return audio_src;
}

GstElement *create_audio_sink(){
	GstElement *audio_sink;

	if(g_getenv("AUDIOSINK")){
		audio_sink = gst_element_factory_make(g_getenv("AUDIOSINK"), NULL);
	}else{
		audio_sink = gst_element_factory_make("autoaudiosink", NULL);
	}
	g_assert(audio_sink);

	return audio_sink;
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

		audioconvert = create_audioconvert();
		audioresample = create_audioresample();
		audio_sink = create_audio_sink();

		gst_bin_add_many(GST_BIN(bin), audioconvert, audioresample, audio_sink, NULL);
		gst_element_link_many(audioconvert, audioresample, audio_sink, NULL);

		pad = gst_element_get_static_pad(audioconvert, "sink");
		gst_element_add_pad(bin, gst_ghost_pad_new("audiosink", pad));

		g_object_set(fsioaudio_sink, "sink-element", GST_BIN(bin), NULL);
	}

	return fsioaudio_sink;

}

GstElement *create_audio_enc(gchar *enc){
	GstElement *audio_enc;

	audio_enc = gst_element_factory_make(enc, enc);
	g_assert(audio_enc);

	return audio_enc;
}

GstElement *create_audioconvert(){
	GstElement *audioconvert;

	audioconvert = gst_element_factory_make("audioconvert", NULL);
	g_assert(audioconvert);

	return audioconvert;

}

GstElement *create_audioresample(){
	GstElement *audioresample;

	audioresample = gst_element_factory_make("audioresample", NULL);
	g_assert(audioresample);

	return audioresample;

}
/* /Audio */

/* Video */
GstElement *create_video_src(){
	GstElement *video_src;

	if(g_getenv("VIDEOSRC")){
		video_src = gst_element_factory_make(g_getenv("VIDEOSRC"), NULL);
	}else{
		video_src = gst_element_factory_make("autovideosrc", NULL);
	}
	g_assert(video_src);

	return video_src;
}

GstElement *create_fsiovideo_src(){
	GstElement *fsiovideo_src;

	fsiovideo_src = gst_element_factory_make("fsiovideosrc", NULL);
	g_assert(fsiovideo_src);

	if(g_getenv("VIDEOSRC")){
		g_object_set(fsiovideo_src, "source-pipeline", g_getenv("VIDEOSRC"), NULL);
	}

	return fsiovideo_src;
}

GstElement *create_videotest_src(){
	GstElement *video_src;

	video_src = gst_element_factory_make("videotestsrc", NULL);
	g_assert(video_src);

	return video_src;
}

GstElement *create_video_sink(){
	GstElement *video_sink;

	if(g_getenv("VIDEOSINK")){
		video_sink = gst_element_factory_make(g_getenv("VIDEOSINK"), NULL);
	}else{
		video_sink = gst_element_factory_make("autovideosink", NULL);
	}
	g_assert(video_sink);

	return video_sink;
}

GstElement *create_fsiovideo_sink(){
	GstElement *fsiovideo_sink, *bin, *video_caps, *ffmpegcolorspace, *videoscale, *video_sink;
	GstPad *pad;

	fsiovideo_sink = gst_element_factory_make("fsiovideosink", NULL);
	g_assert(fsiovideo_sink);

	if(g_getenv("VIDEOSINK")){
		g_object_set(fsiovideo_sink, "sink-pipeline", g_getenv("VIDEOSINK"), NULL);
	}else{
		bin = gst_bin_new("bin");
		g_assert(bin);

		ffmpegcolorspace = create_ffmpegcolorspace();
		videoscale = create_videoscale();
		video_caps = create_video_caps();
		video_sink = create_video_sink();

		gst_bin_add_many(GST_BIN(bin), ffmpegcolorspace, videoscale, video_caps, video_sink, NULL);
		gst_element_link_many(ffmpegcolorspace, videoscale, video_caps, video_sink, NULL);

		pad = gst_element_get_static_pad(ffmpegcolorspace, "sink");
		gst_element_add_pad(bin, gst_ghost_pad_new("videosink", pad));

		g_object_set(fsiovideo_sink, "sink-element", GST_BIN(bin), NULL);
	}

	return fsiovideo_sink;
}

GstElement *create_video_caps(){
	GstElement *capsfilter;
	GstCaps *caps;

	int width		= g_getenv("WIDTH")		== NULL ? 320	: g_ascii_strtoll(g_getenv("WIDTH"), NULL, 10);
	int height		= g_getenv("HEIGHT")	== NULL ? 240	: g_ascii_strtoll(g_getenv("HEIGHT"), NULL, 10);
	int framerate	= g_getenv("FRAMERATE")	== NULL ? 15	: g_ascii_strtoll(g_getenv("FRAMERATE"), NULL, 10);
	int bpp			= g_getenv("BPP")		== NULL ? 24	: g_ascii_strtoll(g_getenv("BPP"), NULL, 10);
	int depth		= g_getenv("DEPTH")		== NULL ? 24	: g_ascii_strtoll(g_getenv("DEPTH"), NULL, 10);

	capsfilter = gst_element_factory_make("capsfilter", NULL);
	g_assert(capsfilter);

	caps = gst_caps_new_simple("video/x-raw-rgb",
		"width", G_TYPE_INT, width,
		"height", G_TYPE_INT, height,
		"framerate", GST_TYPE_FRACTION, framerate, 1,
		"bpp", G_TYPE_INT, bpp,
		"depth", G_TYPE_INT, depth,
		NULL
	);

	g_object_set(capsfilter, "caps", caps, NULL);
	gst_caps_unref(caps);

	return capsfilter;
}

GstElement *create_video_enc(gchar *enc){
	GstElement *video_enc;

	video_enc = gst_element_factory_make(enc, enc);
	g_assert(video_enc);

	return video_enc;
}

GstElement *create_video_mux(gchar *mux){
	GstElement *video_mux;

	video_mux = gst_element_factory_make(mux, mux);
	g_assert(video_mux);

	return video_mux;
}

GstElement *create_ffmpegcolorspace(){
	GstElement *ffmpegcolorspace;

	ffmpegcolorspace = gst_element_factory_make("ffmpegcolorspace", NULL);
	g_assert(ffmpegcolorspace);

	return ffmpegcolorspace;
}

GstElement *create_videoscale(){
	GstElement *videoscale;

	videoscale = gst_element_factory_make("videoscale", NULL);
	g_assert(videoscale);

	return videoscale;
}

GstElement *create_videorate(){
	GstElement *videorate;

	videorate = gst_element_factory_make("videorate", NULL);
	g_assert(videorate);

	return videorate;	
}
/* /Video */

/* File */
GstElement *create_file_src(gchar *location){
	GstElement *file_src;

	file_src = gst_element_factory_make("filesrc", NULL);
	g_assert(file_src);

	g_object_set(file_src, "location", location, NULL);

	return file_src;
}

GstElement *create_file_sink(gchar *location){
	GstElement *file_sink;

	file_sink = gst_element_factory_make("filesink", NULL);
	g_assert(file_sink);

	g_object_set(file_sink, "location", location, NULL);

	return file_sink;
}
/* /File */

/* Utils */
GstBus *get_bus(GstElement *pipeline){
	GstBus *bus;

	bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
	g_assert(bus);

	return bus;
}

GstElement *create_pipeline(){
	GstElement *pipeline;

	pipeline = gst_pipeline_new("pipeline");
	g_assert(pipeline);

	return pipeline;
}

GstElement *create_queue(gchar *queuename){
	GstElement *queue;

	queue = gst_element_factory_make("queue", queuename);
	g_assert(queue);

	return queue;
}

GstElement *create_decodebin(){
	GstElement *decodebin;

	decodebin = gst_element_factory_make("decodebin2", NULL);
	g_assert(decodebin);

	return decodebin;
}

GstElement *create_uridecodebin(){
	GstElement *uridecodebin;

	uridecodebin = gst_element_factory_make("uridecodebin", NULL);
	g_assert(uridecodebin);

	return uridecodebin;
}
/* /Utils */

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