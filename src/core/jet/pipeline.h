/*
 * pipeline.h
 *
 *  Created on: Jun 28, 2015
 *      Author: root
 */

#ifndef JET_PIPELINE_H_
#define JET_PIPELINE_H_

#include <jet/bin.h>


struct jpipeline_s {
    jbin_t parent;
};

struct jpipeline_class_s {
    jbin_class_t parent;

};


J_DECLARE_CLASS(pipeline);

#endif /* JET_PIPELINE_H_ */
