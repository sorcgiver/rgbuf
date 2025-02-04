#ifndef RGBUF_H
#define RGBUF_H

#include "core/rgbase.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct rgbuf rgbuf_t;

void rgbuf_init(rgbuf_t* fd, void* buf, rgsize_t size);
rgsize_t rgbuf_write(rgbuf_t* fd, const void* src, rgsize_t n);
rgsize_t rgbuf_ovrwrite(rgbuf_t* fd, const void* src, rgsize_t n);
rgsize_t rgbuf_read(rgbuf_t* fd, void* dst, rgsize_t n);

#ifdef PRIVATE_RGBUF
struct rgbuf {
	rgbase_t base;
	rgindex_t idx_start;
	rgsize_t szfill;
};
#endif /* PRIVATE_RGBUF */
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* ifndef RGBUF_H */
