/*
 * pipeline.h
 *
 *  Created on: Jun 28, 2015
 *      Author: root
 */

#ifndef JET_PIPELINE_H_
#define JET_PIPELINE_H_

#include "bin.h"
//#include <st/st.h>

struct jpipeline_s {
    jbin_t parent;

//    st_task_t task;
//    st_fd_t fd;
};

struct jpipeline_class_s {
    jbin_class_t parent;

};


J_DECLARE_CLASS(jpipeline);


static int
jpipeline_add(jpipeline_t *p, jelement_t *e)
{
    return jbin_add((jbin_t *)p, e);
}

static int
jpipeline_set_state(jpipeline_t *p, jelement_state_t s)
{
    return jelement_set_state((jelement_t *)p, s);
}


#endif /* JET_PIPELINE_H_ */
