#include "libs/helpers.h"

int main(int argc, char **argv){

	if(argc < 2){
		printf("Usage: %s <file>\n", argv[0]);
		return 0;
	}
	
	GMainLoop *loop;
	GstElement *pipeline, *audio_src, *audio_enc, *file_sink;
	GstBus *bus;

	gst_init(&argc, &argv);

	pipeline = gst_pipeline_new("pipeline");
	g_assert(pipeline);

	bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
	g_assert(bus);

	audio_src = gst_element_factory_make("autoaudiosrc", NULL);
	g_assert(audio_src);

	audio_enc = gst_element_factory_make("wavenc", NULL);
	g_assert(audio_enc);

	file_sink = gst_element_factory_make("filesink", NULL);
	g_assert(file_sink);
	g_object_set(file_sink, "location", argv[1], NULL);

	gst_bin_add_many(pipeline, audio_src, audio_enc, file_sink, NULL);
	gst_element_link_many(audio_src, audio_enc, file_sink, NULL);
	gst_element_set_state(pipeline, GST_STATE_PLAYING);

	loop = g_main_loop_new(NULL, FALSE);
	gst_bus_add_watch(bus, async_bus_cb, loop); // <--- Note that async_bus_cb is a function defined in libs/helpers.c !
	g_main_loop_run(loop);

	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(bus);

	return 0;
}