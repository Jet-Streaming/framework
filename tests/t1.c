
#include <jet/pipeline.h>
//#include <jet/basesrc.h>
#include <jet/plugin.h>

//#include <videotestsrc.h>
//#include <identity.h>
//#include <fakesink.h>

//void make_pipeline(void *loop)
//{
//    GstElement *pipeline;
//    GstElement *source, *filter, *sink;
//    GstBus *bus;
//    guint bus_watch_id;
//    GstPad *pad;
//
//    /* create pipeline */
//    pipeline = gst_pipeline_new ("my-pipeline");
//
//    /* create elements */
////    source = gst_element_factory_make ("fakesrc", "source");
////    source = gst_element_factory_make ("filesrc", "source");
//    source = gst_element_factory_make ("videotestsrc", "source");
////    source = gst_element_factory_make ("tcpserversrc", "source");
//    filter = gst_element_factory_make ("identity", "filter");
////    filter = gst_element_factory_make ("rtmp-server", "filter");
//    sink = gst_element_factory_make ("fakesink", "sink");
//
////    g_signal_connect (source, "pad-added", G_CALLBACK (pad_added), NULL);
//
////    pad = gst_pad_new("aaaa", GST_PAD_SINK);
////    gst_element_add_pad(source, &pad);
//
////    g_object_set(G_OBJECT(source), "location", "/bin/ls", NULL);
//
//    /* must add elements to pipeline before linking them */
//    gst_bin_add_many (GST_BIN (pipeline), source, filter, sink, NULL);
//
//    /* link */
//    if (!gst_element_link_many (source, filter, sink, NULL)) {
//      g_warning ("Failed to link elements!");
//    }
//
////    bus = gst_element_get_bus (pipeline);
////    bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
////    g_object_unref (bus);
//
//
////    gst_element_set_state (pipeline, GST_STATE_NULL);
//    gst_element_set_state (pipeline, GST_STATE_READY);
//    gst_element_set_state (pipeline, GST_STATE_PAUSED);
//    gst_element_set_state (pipeline, GST_STATE_PLAYING);
//}


int main(int argc, char *argv[])
{
    jpipeline_t pipeline;
//    jvideotestsrc_t vtsrc;
//    jidentity_t filter;
//    jfakesink_t sink;
    jelement_t *vtsrc;
    jelement_t *filter;
    jelement_t *sink;
    jerr_t rc;

    jet_init();

    jpipeline_init(&pipeline);

//    jvideotestsrc_init(&vtsrc);
//    jidentity_init(&filter);
//    jfakesink_init(&sink);

    rc = jplugin_load("/media/workspace/jsf/out/Default/lib.target/libvideotestsrc.so");
    vtsrc = jplugin_create_element("videotestsrc");

    filter = jplugin_create_element("identity");
    sink = jplugin_create_element("fakesink");

    jbin_add((jbin_t *)&pipeline, vtsrc);
    jbin_add((jbin_t *)&pipeline, filter);
    jbin_add((jbin_t *)&pipeline, sink);

    jelement_link((jelement_t *)vtsrc, (jelement_t *)filter);
    jelement_link((jelement_t *)filter, (jelement_t *)sink);

    jelement_set_state((jelement_t *)&pipeline, JELEMENT_STATE_READY);
//    jelement_set_state(J_STATE_PAUSED);
//    jelement_set_state(J_STATE_PLAYING);

    return 0;
}
