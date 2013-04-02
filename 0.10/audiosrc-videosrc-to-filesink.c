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

	pipeline = create_pipeline();
	bus = get_bus(pipeline);
	video_src = create_video_src();
	queue1 = create_queue("queue1");
	video_enc = create_video_enc("theoraenc");
	queue2 = create_queue("queue2");
	audio_src = create_audio_src();
	queue3 = create_queue("queue3");
	audio_enc = create_audio_enc("vorbisenc");
	queue4 = create_queue("queue4");
	mux = create_video_mux("oggmux");
	queue5 = create_queue("queue5");
	file_sink = create_file_sink(argv[1]);

	gst_bin_add_many(pipeline, queue1, queue2, queue3, queue4, queue5, audio_enc, audio_src, video_enc, video_src, file_sink, mux, NULL);
	gst_element_link_many(video_src, queue1, video_enc, queue2, mux, NULL);
	gst_element_link_many(audio_src, queue3, audio_enc, queue4, mux, NULL);
	gst_element_link_many(mux, queue5, file_sink, NULL);

	gst_element_set_state(pipeline, GST_STATE_PLAYING);

	loop = g_main_loop_new(NULL, FALSE);
	gst_bus_add_watch(bus, async_bus_cb, loop);
	g_main_loop_run(loop);

	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(bus);

	return 0;
}