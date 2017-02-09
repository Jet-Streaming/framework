#pragma once
#include "packet.h"
#include "runtime_class.h"
//#include <memory>

//typedef enum {
//    JPAD_UNKNOWN = 0,
//    JPAD_SRC,
//    JPAD_SINK,
//} jpad_direction_t;

//typedef enum {
//    JPAD_UNKNOWN = 0,
//    JPAD_PULL,
//    JPAD_PUSH,
//} jpad_mode_t;
//

namespace jet {

class element;
class J_EXTERN pad
{
public:
    struct push_req;
    enum push_type;
private:
    // TODO jmbuf_t -> jobject_t
    int(*on_push)(pad *pad, push_req *req, push_type type);
    //void(*on_push_event)(pad *pad, push_req *req, push_event event);
    //void (*on_cancel_push)(pad *pad, push_req *req);

public:
    enum push_type {
        // framework control
        PACKET,
        ACK,
        CANCEL,
        UNLINK,
    };
    //typedef int(*push_callback)(pad *, push_req *, push_type);
    typedef decltype(on_push) push_callback;

    pad(const char *_name, element *parent,
        push_callback _on_push);
    ~pad();

    //typedef void(*push_cb)(push_req *push);
    struct push_req {
        int status;

        pad *sender;

        packet *pkt;
        //uint32_t refc : 8;
        uint32_t canceled : 1;
        //uint32_t acked : 1;
        //uint32_t pending : 1;

        push_req() 
            : pkt(nullptr)
            , sender(nullptr)
            , status(EOK)
            //, refc(1)
            , canceled(0)
            //, acked(0)
            //, pending(0)
        {}
        ~push_req() {
            // TODO free pkt
        }
    };

    friend class element;
    friend struct push_req;

    int link(pad *peerpad);
    int unlink(pad *peerpad);

    int push(push_req *req, packet *pkt = nullptr);
    static void ack_push(push_req *req);
    int cancel_push(push_req *req);
    //void set_parent(element *e) {
    //    parent_element = e;
    //}

    enum direction {
        DIN,
        DOUT,
    };
    int set_cap(direction dir, const rt_class *rtc);
    int set_cap(direction dir, const char *name);

private:
    char name[16];
    pad *peer;
    element *parent_element;
    const rt_class *in_cap;
    const rt_class *out_cap;

};

} // jet