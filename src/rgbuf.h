#ifndef RGBUF_H
#define RGBUF_H

#include "core/rgbase.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
	struct rgbase base;
	rgindex_t idx_start;
	rgsize_t szfill;
} rgbuf_t;

void rgbuf_init(rgbuf_t* fd, void* buf, rgsize_t size);
void rgbuf_clear(rgbuf_t* fd);
rgsize_t rgbuf_write(rgbuf_t* fd, const void* src, rgsize_t n);
rgsize_t rgbuf_overwrite(rgbuf_t* fd, const void* src, rgsize_t n);
rgsize_t rgbuf_read(rgbuf_t* fd, void* dst, rgsize_t n);
rgsize_t rgbuf_peek(const rgbuf_t* fd, rgsize_t skip_n, void* dst, rgsize_t n);
rgsize_t rgbuf_skip(rgbuf_t* fd, rgsize_t n);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* ifndef RGBUF_H */
