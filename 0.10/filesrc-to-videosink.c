#include "libs/helpers.h"
 
int main(int argc, char **argv){

	if(argc < 2){
		printf("Usage: %s <file>\n", argv[0]);
		return 0;
	}

	GMainLoop *loop;
	GstElement *pipeline, *file_src, *decodebin2, *queuev, *ffmpegcolorspace, *videoscale, *videorate, *video_caps, *ffmpegcolorspace2, *video_sink;
	GstCaps *caps;
	GstBus *bus;

	gst_init(&argc, &argv);

	pipeline = gst_pipeline_new("pipeline");
	g_assert(pipeline);

	bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
	g_assert(bus);

	file_src = gst_element_factory_make("filesrc", NULL);
	g_assert(file_src);
	g_object_set(file_src, "location", argv[1], NULL);

	decodebin2 = gst_element_factory_make("decodebin2", NULL);
	g_assert(decodebin2);

	queuev = gst_element_factory_make("queue", "queuev");
	g_assert(queuev);

	ffmpegcolorspace = gst_element_factory_make("ffmpegcolorspace", NULL);
	g_assert(ffmpegcolorspace);

	videoscale = gst_element_factory_make("videoscale", NULL);
	g_assert(videoscale);

	videorate = gst_element_factory_make("videorate", NULL);
	g_assert(videorate);

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

	ffmpegcolorspace2 = gst_element_factory_make("ffmpegcolorspace", NULL);
	g_assert(ffmpegcolorspace2);

	video_sink = gst_element_factory_make("autovideosink", NULL);
	g_assert(video_sink);

	gst_bin_add_many(pipeline, file_src, decodebin2, queuev, ffmpegcolorspace, videoscale, videorate, video_caps, ffmpegcolorspace2, video_sink, NULL);
	gst_element_link_many(file_src, decodebin2, NULL);
	gst_element_link_many(queuev, ffmpegcolorspace, videoscale, videorate, video_caps, ffmpegcolorspace2, video_sink, NULL);

	g_signal_connect(decodebin2, "pad-added", G_CALLBACK(on_pad_added), pipeline); // <--- Note that on_pad_added is a funciton defined in libs/helpers.c !

	gst_element_set_state(pipeline, GST_STATE_PLAYING);

	loop = g_main_loop_new(NULL, FALSE);
	gst_bus_add_watch(bus, async_bus_cb, loop); // <--- Note that async_bus_cb is a function defined in libs/helpers.c !
	g_main_loop_run(loop);

	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(bus);

	return 0;
}