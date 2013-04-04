#include "libs/helpers.h"
 
int main(int argc, char **argv){

	if(argc < 2){
		printf("Usage: %s <file>\n", argv[0]);
		return 0;
	}

	GMainLoop *loop;
	GstElement *pipeline, *queue1, *queue2, *queue3, *queue4, *queue5, *audio_enc, *audio_src, *video_enc, *video_src, *file_sink, *mux;
	GstBus *bus;

	gst_init(&argc, &argv);

	pipeline = gst_pipeline_new("pipeline");
	g_assert(pipeline);

	bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
	g_assert(bus);

	video_src = gst_element_factory_make("autovideosrc", NULL);
	g_assert(video_src);

	queue1 = gst_element_factory_make("queue", "queue1");
	g_assert(queue1);

	video_enc = gst_element_factory_make("theoraenc", NULL);
	g_assert(video_enc);

	queue2 = gst_element_factory_make("queue", "queue2");
	g_assert(queue2);

	audio_src = gst_element_factory_make("autoaudiosrc", NULL);
	g_assert(audio_src);

	queue3 = gst_element_factory_make("queue", "queue3");
	g_assert(queue3);

	audio_enc = gst_element_factory_make("vorbisenc", NULL);
	g_assert(audio_enc);

	queue4 = gst_element_factory_make("queue", "queue4");
	g_assert(queue4);

	mux = gst_element_factory_make("oggmux", NULL);
	g_assert(mux);

	queue5 = gst_element_factory_make("queue", "queue5");
	g_assert(queue5);

	file_sink = gst_element_factory_make("filesink", NULL);
	g_assert(file_sink);
	g_object_set(file_sink, "location", argv[1], NULL);

	gst_bin_add_many(pipeline, queue1, queue2, queue3, queue4, queue5, audio_enc, audio_src, video_enc, video_src, file_sink, mux, NULL);
	gst_element_link_many(video_src, queue1, video_enc, queue2, mux, NULL);
	gst_element_link_many(audio_src, queue3, audio_enc, queue4, mux, NULL);
	gst_element_link_many(mux, queue5, file_sink, NULL);

	gst_element_set_state(pipeline, GST_STATE_PLAYING);

	loop = g_main_loop_new(NULL, FALSE);
	gst_bus_add_watch(bus, async_bus_cb, loop); // <--- Note that async_bus_cb is a function defined in libs/helpers.c !
	g_main_loop_run(loop);

	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(bus);

	return 0;
}