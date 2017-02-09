#pragma once
#include "rtmp.h"
#include <jet/seqpacket.h>

#ifdef _WIN32
# if defined(JET_BUILDING_RTMP)
#   define JRTMP_EXTERN __declspec(dllexport)
# elif defined(JET_USING_RTMP)
#   define JRTMP_EXTERN __declspec(dllimport)
# else
#   define JRTMP_EXTERN /* nothing */
# endif
#elif __GNUC__ >= 4
# define JRTMP_EXTERN __attribute__((visibility("default")))
#else
# define JRTMP_EXTERN /* nothing */
#endif

namespace jet {
namespace rtmp {

enum {
    // RTMP
    RTMP_MSG_USER_CONTROL = 4,
    RTMP_MSG_EDGE = 7,
    RTMP_MSG_AUDIO = 8,
    RTMP_MSG_VIDEO = 9,
    RTMP_MSG_AMF3_META = 15,
    RTMP_MSG_AMF3_SHARED = 16,
    RTMP_MSG_AMF3_CMD = 17,
    RTMP_MSG_AMF_META = 18,
    RTMP_MSG_AMF_SHARED = 19,
    RTMP_MSG_AMF_CMD = 20,
    RTMP_MSG_AGGREGATE = 22,
    RTMP_MSG_MAX = 22,
};

class JRTMP_EXTERN rtmp_message : public seqpacket {
    JET_DECLARE_RTCLASS(rtmp_message);

public:
    //rtmp_message(int chunk_size);
    //~rtmp_message();

    uint32_t type : 8;
    uint32_t mlen : 24;
    uint32_t timestamp;
    //uint32_t csid : 24;
    uint32_t msid : 8;
};

} // rtmp
} // jet