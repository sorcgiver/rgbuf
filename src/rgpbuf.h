#ifndef RGPBUF_H
#define RGPBUF_H

#include <stdbool.h>
#include "core/rgbase.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
	struct rgbase base;
	rgindex_t idx_start;
	rgsize_t szfill;
} rgpbuf_t;

typedef enum {
	RGP_TYPE_8,  // size:       0x01..0x3f
	RGP_TYPE_16, // size:       0x40..0x403e
	RGP_TYPE_32, // size:     0x403f..0x4000403d
	RGP_TYPE_64  // size: 0x4000403e..0x400000004000403c
} rgp_type;

void rgpbuf_init(rgpbuf_t* fd, void* buf, rgsize_t size);
void rgpbuf_clear(rgpbuf_t* fd);
bool rgpbuf_write8(rgpbuf_t* fd, const void* data, uint8_t n);
bool rgpbuf_write16(rgpbuf_t* fd, const void* data, uint16_t n);
bool rgpbuf_write32(rgpbuf_t* fd, const void* data, uint32_t n);
bool rgpbuf_write64(rgpbuf_t* fd, const void* data, uint64_t n);
rgp_type rgpbuf_rtype(rgpbuf_t* fd);
uint8_t rgpbuf_rsize8(rgpbuf_t* fd);
uint16_t rgpbuf_rsize16(rgpbuf_t* fd);
uint32_t rgpbuf_rsize32(rgpbuf_t* fd);
uint64_t rgpbuf_rsize64(rgpbuf_t* fd);

uint64_t rgpbuf_read64(rgpbuf_t* fd, void* dst);

/* ===== Format pack =====
 * type:2 - (2 bit) size type
 * size - data size
 * data[size]
 * =======================
 *
 * type = 0b00
 * size = 6 bit
 *
 * type = 0b01
 * size = 14 bit
 *
 * type = 0b10
 * size = 30 bit
 *
 * type = 0b11
 * size = 62 bit
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* ifndef RGPBUF_H */
