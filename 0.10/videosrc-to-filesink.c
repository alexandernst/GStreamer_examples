#include "libs/helpers.h"

int main(int argc, char **argv){

	if(argc < 2){
		printf("Usage: %s <file>\n", argv[0]);
		return 0;
	}

	GMainLoop *loop;
	GstElement *pipeline, *video_src, *queue1, *ffmpegcolorspace, *videoscale, *videorate, *video_enc, *video_mux, *file_sink;
	GstBus *bus;

	gst_init(&argc, &argv);

	pipeline = create_pipeline();
	bus = get_bus(pipeline);
	video_src = create_video_src();
	queue1 = create_queue("queue1");
	ffmpegcolorspace = create_ffmpegcolorspace();
	videoscale = create_videoscale();
	videorate = create_videorate();
	video_enc = create_video_enc("theoraenc");
	video_mux = create_video_mux("oggmux");
	file_sink = create_file_sink(argv[1]);

	gst_bin_add_many(pipeline, video_src, queue1, ffmpegcolorspace, videoscale, videorate, video_enc, video_mux, file_sink, NULL);
	gst_element_link_many(video_src, queue1, ffmpegcolorspace, videoscale, videorate, video_enc, video_mux, file_sink, NULL);

	gst_element_set_state(pipeline, GST_STATE_PLAYING);

	loop = g_main_loop_new(NULL, FALSE);
	gst_bus_add_watch(bus, async_bus_cb, loop);
	g_main_loop_run(loop);

	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(bus);

	return 0;
}