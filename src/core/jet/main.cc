
#include <stdio.h>
#include <jet/jet.h>
//#include <jet/pipeline.h>
//#include <jet/basesrc.h>
//#include <jet/plugin.h>
#ifdef _WIN32
#else
#include <netinet/in.h>
#endif
//#include "../src/core/st/handle.h"

//#include <plugins/jet/coreplugin/tcp.h>

using namespace jet;

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

//int pl1(jpipeline_t *pipeline)
//{
//    jelement_t *vtsrc, *tcpsrc;
//    jelement_t *filter;
//    jelement_t *sink;
//    st_fd_t fd;
//    int rc;
//    struct sockaddr_in inaddr;
//
////    fd = st_socket(AF_INET, SOCK_DGRAM, 0);
//
//    memset(&inaddr, 0, sizeof(inaddr));
//    inaddr.sin_family = AF_INET;
//    inaddr.sin_port = htons(8888);
//    inaddr.sin_addr.s_addr = htonl(INADDR_ANY);
//    rc = st_bind(fd, (struct sockaddr *)&inaddr, sizeof(inaddr));
//
//
////    vtsrc = jplugin_create_element("videotestsrc");
//    tcpsrc = jplugin_create_element("tcpsrc");
//    rc = jobject_set_property(tcpsrc, "fd", &fd);
//
//    filter = jplugin_create_element("identity");
//    sink = jplugin_create_element("fakesink");
//
//    jpipeline_add(pipeline, /*vtsrc*/tcpsrc);
//    jpipeline_add(pipeline, filter);
//    jpipeline_add(pipeline, sink);
//
//    jelement_link(/*vtsrc*/tcpsrc, filter);
//    jelement_link(filter, sink);
//
//    return rc;
//}
//
//int pl_queue1(jpipeline_t *pipeline)
//{
//    jelement_t *vtsrc, *q, *sink;
//    int rc;
//
//    rc = jplugin_load("/media/workspace/jsf/out/Debug/lib.target/libvideotestsrc.so");
//    vtsrc = jplugin_create_element("videotestsrc");
//    q = jplugin_create_element("queue");
//    sink = jplugin_create_element("fakesink");
//
//    jpipeline_add(pipeline, vtsrc);
//    jpipeline_add(pipeline, q);
//    jpipeline_add(pipeline, sink);
//
//    jelement_link(vtsrc, q);
//    jelement_link(q, sink);
//
//    return rc;
//
//}


//static void
//connection_cb(jtcp_t* server, int status)
//{
//	int rc;
//	assert(status == 0);
//	
//	jelement_t *tcp = jplugin_create_element("tcp");
//	rc = jtcp_accept(tcp, server);
//
//    //jelement_t *rtmp = jplugin_create_element("rtmp");
//	//jelement_link(tcp, NULL, rtmp, "rtmp");
//
//    jelement_t *http = jplugin_create_element("http");
//    jelement_link(tcp, NULL, http, "http");
//
//    rc = jtcp_read_start(tcp);
//}
//
//int pl_tcp(jpipeline_t *pipeline)
//{
//    int rc;
//    jelement_t *tcp;
//    //struct sockaddr_in addr;
//
//    tcp = jplugin_create_element("tcp");
//
//    //memset(&addr, 0, sizeof(addr));
//    //addr.sin_family = AF_INET;
//    //addr.sin_addr.s_addr = inet_addr("0.0.0.0");
//    //addr.sin_port = ntohs(8081);
//    rc = jtcp_listen(tcp, "0.0.0.0", 8081, 10, connection_cb);
//
//    //jrtmp_dummy();
//	//jhttp_dummy();
//
//    return rc;
//}

static uv_tcp_t conn;
static uv_connect_t connect_req;
static void close_cb(uv_handle_t* handle){
    ;
}
static void connect_cb(uv_connect_t* req, int status) {
    //ASSERT(req == &connect_req);
    //ASSERT(status == UV_ECANCELED);
    //connect_cb_called++;
    uv_close((uv_handle_t *)&conn, close_cb);
}

static void test_tcp()
{
    struct sockaddr_in addr;
    int r;

    uv_ip4_addr("192.168.144.197", 9999, &addr);

    r = uv_tcp_init(uv_default_loop(), &conn);

    r = uv_tcp_connect(&connect_req,
        &conn,
        (const struct sockaddr*) &addr,
        connect_cb);

    r = uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

int main(int argc, char *argv[])
{
    //jpipeline_t pipeline;
    int rc;

    //test_tcp();

    rc = jet::init();
    if (rc) {
        goto out;
    }

    //rc = jpipeline_init(&pipeline);

//    rc = pl1(&pipeline);
//    rc = pl_tcp(&pipeline);
//    rc = pl_queue1(&pipeline);
//
//    jpipeline_set_state(&pipeline, JELEMENT_STATE_READY);
//    jpipeline_set_state(&pipeline, JELEMENT_STATE_PAUSED);
//    jpipeline_set_state(&pipeline, JELEMENT_STATE_PLAYING);

    //if (argc > 1) {
    //    printf("run %s\n", argv[1]);
    //}


    printf("jet_run\n");
    rc = jet::run(argc, argv);

out:
    printf("app exited, press any key to exit\n");
    getchar();
    return 0;
}
