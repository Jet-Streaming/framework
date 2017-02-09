#ifndef __CN_360_LIVE_RTMP_PUB_H_
#define __CN_360_LIVE_RTMP_PUB_H_

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* rtmp_session_t;

rtmp_session_t rtmp_create_session(const char* url);

void rtmp_write_h264(rtmp_session_t rtmp, void* data, size_t length, uint64_t timestamp);
void rtmp_write_h264_annexb(rtmp_session_t rtmp, void* data, size_t length, uint64_t timestamp);
void rtmp_write_adts(rtmp_session_t rtmp, void* data, size_t length, uint64_t timestamp);

void rtmp_destory_session(rtmp_session_t rtmp);


#ifdef __cplusplus
}
#endif
#endif  /* __CN_360_LIVE_RTMP_PUB_H_ */
