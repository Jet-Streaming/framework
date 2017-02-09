#include "element.h"
//#include "pad.h"
//#include "event.h"
#include "element_p.h"
#include "config.h"
#include "literal.h"
//#include <uv/src/uv-common.h> // uv__handle_init
//#include <mutex>

namespace jet {

element_private::element_private()
	: name(nullptr)
    , parent(nullptr)
    //, be_object(nullptr)
{}

element_private::~element_private()
{}

/////////////////////////////////////////////

//element::element() 
//	: object(new element_private)
//{
//    //uv__handle_init(uv_default_loop(), &close_handle, UV_HANDLE);
//}

element::element(element_private *dd)
	: object(dd ? dd : new element_private)
{
}


element::~element()
{
    //if (be_object) {
        // ������������
        // ���ͷ��ڴ棬��Ϊ�ڴ���be�Զ�����C++�ڶ�Ӧ������������delete be_object��
    //}
}

pad *
element::find_pad(const char *name)
{
	J_D(element);

	for (auto i = d->pads.begin(); i != d->pads.end(); ++i) {
		if (!strcmp((*i)->name, name)) {
			return *i;
		}
	}

    return nullptr;
}

void
element::add_pad(pad *p)
{
	J_D(element);
	
	assert(this == p->parent_element);
    d->pads.push_back(p);
}

int 
element::link(const char *mypad, element *dste, const char *dstpad)
{
    pad *pad1, *pad2;
    int rc;

    /*
    ���ڣ�ֻ����ֻ��һ��pad��2��element���ӣ���������
    ���ڣ���������δ���ӵ�pad���ɹ�1��������
    */
    //assert(1 == elm1->pads.nelts && 1 == elm2->pads.nelts);

    pad1 = find_pad(mypad);
    pad2 = dste->find_pad(dstpad);
    if (!pad1 || !pad2) {
        return -ENOENT;
    }
    rc = pad1->link(pad2);

    return rc;
}

/*
���ڷ����˲����޸����޷�����Ĵ���ʱ��
ͬ����ɣ�������pad����terminate event�������Լ���
�첽��ɣ���Ϊ����ջ����ĺ������ܻ����õ����ڴ棬���Բ����������ͷţ����ͷ��Լ�

���մ�����cancel���еģ����ڴ���ģ�push��Ȼ����ҵ����
*/
static void
terminate_cb(uv_handle_t *h)
{
    //element *self;

    //self = container_of(h, element, close_handle);
    //free(self);
}

void 
element::abort()
{
    //int i;
    //pad *pad;
    //jevent_t *term;
    //int rc;

    //pad = self->pads.elts;
    //for (i = 0; i != self->pads.nelts; ++i, ++pad) {
    //    // just notify, without waiting for the results
    //    //rc = jpad_push(pad, (jmbuf_t *)term, NULL);
    //}

    //jobject_fina((jobject_t *)self);

    //uv_close(&close_handle, terminate_cb);
}

//static std::mutex elms_mutex;
//static std::map<jet::literal, element::element_creator> elms_map;
//
//element*
//element::create(const char *klass)
//{
//    std::lock_guard<std::mutex> lock(elms_mutex);
//    auto iter = elms_map.find(klass);
//    if (elms_map.end() == iter) {
//        return nullptr;
//    }
//    
//    return iter->second();
//}
//
//int
//element::reg(const char *klass, element_creator creator)
//{
//    std::lock_guard<std::mutex> lock(elms_mutex);
//    if (elms_map.count(klass)) {
//        return -EEXIST;
//    }
//    elms_map[klass] = creator;
//    return EOK;
//}


} // namespace jet



/*
 *  url: icc://localhost/videotestsrc/srcpad
 *       itc://localhost/
 *       ipc://192.168.144.197/h264enc/sinkpad
 *
 *       localhost/...  (maybe more than one upstream per cluster)
 *       localcluster/rtmp-muxer/srcpad/www.cntv.com/live/stream (only one per cluster)
 *       localnetwork/... (only one upstream per CDN)
 */
//int
//jelement_link_url(jelement_t *self, const char *addr)
//{
//    //jsk_connect();
//    return EOK;
//}

//static int
//jelement_change_state(jelement_t *self, jelement_state_transition_t trans)
//{
//    jelement_class_t *cls;
////    int rc;
//
//    cls = self->parent._class;
//    return cls->change_state(self, trans);
//}

//int
//jelement_set_state(jelement_t *self, jelement_state_t state)
//{
//    int rc;
//    jelement_class_t *cls;
//    union jelement_transition_u trans;
//
//    trans.s.from = self->state;
//    trans.s.to = self->state + 1;
//    cls = self->parent._class;
//    while (trans.s.from != state) {
//        rc = cls->change_state(self, trans.t);
//        if (EOK != rc) {
//            return rc;
//        }
//        self->state = trans.t;
//        ++trans.s.from;
//        ++trans.s.to;
//    }
//
//    return EOK;
//}
//
//static int
//activate_pads(jelement_t *self, jlist_head_t *pads, int action)
//{
//    jpad_t *pad;
//    int rc;
//    jlist_head_t *lh;
//
//
////    for (lh = pads->next; lh != pads->prev; lh = lh->next) {
////        pad = container_of(lh, jpad_t, pads);
////    list_for_each_entry(pad, pads, pads) {
////        rc = jpad_activate(pad, action);
////        if (EOK != rc) {
////            return rc;
////        }
////    }
//
//    return EOK;
//}
//
//int
//jelement_change_state(jelement_t *self, jelement_transition_t trans)
//{
//    int rc;
//
//    // change srcs
//
//    // change self
//    switch (trans) {
//        case JELEMENT_STATE_NULL_TO_READY:
//            break;
//        case JELEMENT_STATE_READY_TO_PAUSED:
////            rc = activate_pads(self, &self->srcpads, 1);
////            rc = activate_pads(self, &self->sinkpads, 1);
//            break;
//        case JELEMENT_STATE_PAUSED_TO_PLAYING:
//            break;
//        default:
//            assert(0);
////            j_debug();
//            return -EINVAL;
//    }
//
//    // change sinks
//
//    return EOK;
//}

//int
//jelement_add_pad(jelement_t *self, jpad_t *pad)
//{
//    jpad_t **p;
//
//    p = jarray_push(&self->pads);
//    if (!p) {
//        return -ENOMEM;
//    }
//
//    *p = pad;
//    return EOK;
//}

//int
//jelement_register(jplugin_t *plugin, jelement_class_t *_class)
//{
//    return EOK;
//}



//void
//jelement_virtual(jelement_t *self)
//{
//    jelement_class.virtual(self);
//}


//void
//jelement_terminate(jelement_t *self)
//{
//}


//static void
//set_seginfo(jelement_t *self, jpad_t *pad, jpad_segfield_t field)
//{
//    jpad_t **pad2;
//    int i;
//    //uint32_t *v;
//
//    pad2 = self->pads.elts;
//    for (i = 0; i != self->pads.nelts; ++i, ++pad2) {
//        if (*pad2 == pad) {
//            continue;
//        }
//
//        //v = (uint32_t *)((uint8_t *)&(*pad2)->seginfo + field);
//        //*v = JPAD_SEGLEN_UNK;
//        jpad_set_seginfo(*pad2, field, JPAD_SEGLEN_UNK);
//    }
//}

// ��pad���˳����Ϊ��һ����$pad��pad��segment size�����û���ҵ�����ΪĬ��ֵ
//static int
//init_seginfo(jelement_t *self, jpad_t *pad)
//{
//    jpad_t **pad2, *peer;
//    int i, rc;
//
//    pad2 = self->pads.elts;
//    for (i = 0; i != self->pads.nelts; ++i, ++pad2) {
//        if (*pad2 == pad) {
//            continue;
//        }
//        peer = (*pad2)->peer;
//        if (!peer) {
//            continue;
//        }
//        if (rc = jpad_init_seginfo(peer)) {
//            return rc;
//        }
//        assert(peer->seginfo.valid);
//        pad->seginfo = peer->seginfo;
//        return EOK;
//    }
//
//    assert(jconf_data()->def_seginfo.valid);
//    pad->seginfo = jconf_data()->def_seginfo;
//    return EOK;
//}
//
//// ��$pad��peer��segment size�ѷ����仯
//// Ĭ����Ϊ�����ø�element����padΪinvalid
//static int
//update_seginfo(jelement_t *self, jpad_t *pad)
//{
//    jpad_t **pad2, *peer;
//    int i, rc;
//
//    pad2 = self->pads.elts;
//    for (i = 0; i != self->pads.nelts; ++i, ++pad2) {
//        if (*pad2 == pad) {
//            continue;
//        }
//        peer = (*pad2)->peer;
//        if (!peer) {
//            continue;
//        }
//        if (rc = jpad_update_seginfo(peer)) {
//            return rc;
//        }
//        assert(!peer->seginfo.valid);
//    }
//
//    return EOK;
//}