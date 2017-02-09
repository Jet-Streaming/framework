#pragma once
//#include "../../jet/jet.h"
#include <stdint.h>

namespace jet {

class packet
{
public:
    packet();
    ~packet() {
    }

    // current packet data pointer
    void *data() {
    }
    // sum of data lengths over all packet segments.
    uint32_t len() {
    }

    // start address of the first segment.
    void *buf_addr() {
    }
    // sum of buffer lengths over all packet segments.
    uint32_t buf_len() {
    }

    // ref

    // nb_segs
    //virtual uint32_t;

    //virtual void *user_data() = 0;

    int alloc(uint32_t s) {
    }
    packet *clone() {
        return nullptr;
    }

    //virtual packet *prev() = 0;
    //virtual packet *next() = 0;
    //virtual packet *last() = 0;

private:
};

} // jet