#include "libs/helpers.h"

int main(int argc, char **argv){
	GMainLoop *loop;
	GstElement *pipeline, *audio_src, *audioconvert, *audioresample, *audio_sink;
	GstBus *bus;

	gst_init(&argc, &argv);

	pipeline = create_pipeline();
	bus = get_bus(pipeline);
	audio_src = create_audio_src();
	audioconvert = create_audioconvert();
	audioresample = create_audioresample();
	audio_sink = create_audio_sink();

	gst_bin_add_many(pipeline, audio_src, audioconvert, audioresample, audio_sink, NULL);
	gst_element_link_many(audio_src, audioconvert, audioresample, audio_sink, NULL);

	gst_element_set_state(pipeline, GST_STATE_PLAYING);

	loop = g_main_loop_new(NULL, FALSE);
	gst_bus_add_watch(bus, async_bus_cb, loop);
	g_main_loop_run(loop);

	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(bus);

	return 0;
}