/*
 * video.h
 *
 *  Created on: Jul 4, 2015
 *      Author: root
 */

#ifndef SRC_CORE_JET_VIDEO_H_
#define SRC_CORE_JET_VIDEO_H_

#include <jet/capability.h>


enum jpixel_format_e {
   J_PIX_FMT_NONE = 0,
   J_PIX_FMT_I420,
   J_PIX_FMT_YV12,
   J_PIX_FMT_YUY2,
   J_PIX_FMT_UYVY,
   J_PIX_FMT_AYUV,
   J_PIX_FMT_RGBx,
};

struct jvideocap_s {
    jcapability_t parent;
    jpixel_format_t *formats;
    int width[2];
    int height[2];
    int framerate[2];
};

struct jvideocap_class_s {
    jcapability_class_t parent;
};

#endif /* SRC_CORE_JET_VIDEO_H_ */
