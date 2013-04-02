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

	pipeline = create_pipeline();
	bus = get_bus(pipeline);
	audio_src = create_audio_src();
	audio_enc = create_audio_enc("wavenc");
	file_sink = create_file_sink(argv[1]);

	gst_bin_add_many(pipeline, audio_src, audio_enc, file_sink, NULL);
	gst_element_link_many(audio_src, audio_enc, file_sink, NULL);
	gst_element_set_state(pipeline, GST_STATE_PLAYING);

	loop = g_main_loop_new(NULL, FALSE);
	gst_bus_add_watch(bus, async_bus_cb, loop);
	g_main_loop_run(loop);

	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(bus);

	return 0;
}