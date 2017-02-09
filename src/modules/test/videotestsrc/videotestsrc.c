

#include "videotestsrc.h"
#include <jet/plugin.h>
#include <jet/videocap_xraw.h>

/* "video/x-raw, "                                                     \
  "format = (string) " "{ I420, YV12, YUY2, UYVY, AYUV, RGBx, "  \
  "BGRx, xRGB, xBGR, RGBA, BGRA, ARGB, ABGR, RGB, BGR, Y41B, Y42B, "  \
  "YVYU, Y444, v210, v216, NV12, NV21, NV16, NV24, GRAY8, GRAY16_BE, GRAY16_LE, " \
  "v308, RGB16, BGR16, RGB15, BGR15, UYVP, A420, RGB8P, YUV9, YVU9, " \
  "IYU1, ARGB64, AYUV64, r210, I420_10LE, I420_10BE, I422_10LE, I422_10BE, " \
  " Y444_10LE, Y444_10BE, GBR, GBR_10LE, GBR_10BE, NV12_64Z32 }" ", "                                    \
  "width = " "(int) [ 1, max ]" ", "                                \
  "height = " "(int) [ 1, max ]" ", "                               \
  "framerate = " "(fraction) [ 0, max ]" ";" \
"video/x-bayer, format=(string) { bggr, rggb, grbg, gbrg }, "        \
"width = " "(int) [ 1, max ]" ", "                                 \
"height = " "(int) [ 1, max ]" ", "                                \
"framerate = " "(fraction) [ 0, max ]", \
*/


static jvideocap_xraw_t xraw_cap;
//static jvideocap_xbayer_t xbayer_cap;
static jcapability_t caps[] = { &xraw_cap, };


static jerr_t
inst_init(jvideotestsrc_t *self, va_list args)
{
    jelement_t *elm;

    elm = (jelement_t *)self;

    //jvideotestsrcpad_init(&self->srcpad);
    jelement_add_pad(elm, (jpad_t *)&self->srcpad);
    return J_OK;
}

static void
vtbl_init(jvideotestsrc_class_t *self)
{
    jelement_class_t *elm;

    elm = (jelement_class_t *)self;
    elm->name = "videotestsrc";
    elm->inst_size = sizeof(jvideotestsrc_t);
    elm->srccaps = caps;
    elm->nsrccaps = ARRAY_SIZE(caps);


    //
//    jvideocap_xraw_init(&xraw_cap);
}

J_DEFINE_CLASS(jvideotestsrc, jbasesrc, inst_init, vtbl_init);



////////////////////////////////////////////////////////////////////////////////
static jerr_t
plugin_init()
{
    jvideotestsrcpad_class_init();
    jvideotestsrc_class_init();

    return J_OK;
}

static jelement_class_t *elements[] = {
        (jelement_class_t *)&jvideotestsrc_class,
        NULL
};

jplugin_t jvideotestsrc_plugin = {
        .name = "videotestsrc",
        .licence = "LGPL",
        .elements = elements,
        .init = plugin_init,
};
