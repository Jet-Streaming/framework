/*
 * buffer.h
 *
 *  Created on: Jul 19, 2015
 *      Author: root
 */

#ifndef SRC_CORE_JET_BUFFER_H_
#define SRC_CORE_JET_BUFFER_H_

#include <jet/object.h>

/*
 * +--------------------------------------------------------+
 * | MAC header | IPv4/v6 header | TCP/UDP header | payload |
 * +--------------------------------------------------------+
 *
 */

struct jbuffer_s {
    jobject_t parent;

    uint8_t *end;
    uint8_t *pos;
    uint8_t *last;

    // last member
    uint8_t data[0];
};

struct jbuffer_class_s {
    jobject_class_t parent;

};

J_DECLARE_CLASS(buffer);


jbuffer_t *jbuf_create(size_t len);

#endif /* SRC_CORE_JET_BUFFER_H_ */
