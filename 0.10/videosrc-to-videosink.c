#include "libs/helpers.h"

int main(int argc, char **argv){
	GMainLoop *loop;
	GstElement *pipeline, *video_src, *ffmpegcolorspace, *videoscale, *videorate, *video_caps, *ffmpegcolorspace2, *video_sink;
	GstBus *bus;

	gst_init(&argc, &argv);

	pipeline = create_pipeline();
	bus = get_bus(pipeline);
	video_src = create_video_src();
	ffmpegcolorspace = create_ffmpegcolorspace();
	videoscale = create_videoscale();
	videorate = create_videorate();
	video_caps = create_video_caps();
	ffmpegcolorspace2 = create_ffmpegcolorspace();
	video_sink = create_video_sink();

	gst_bin_add_many(pipeline, video_src, ffmpegcolorspace, videoscale, videorate, video_caps, ffmpegcolorspace2, video_sink, NULL);
	gst_element_link_many(video_src, ffmpegcolorspace, videoscale, videorate, video_caps, ffmpegcolorspace2, video_sink, NULL);

	gst_element_set_state(pipeline, GST_STATE_PLAYING);

	loop = g_main_loop_new(NULL, FALSE);
	gst_bus_add_watch(bus, async_bus_cb, loop);
	g_main_loop_run(loop);

	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(bus);

	return 0;
}