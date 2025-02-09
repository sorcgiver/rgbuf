#define PRIVATE_RGBASE
#define PRIVATE_RGPBUF
#include <stdbool.h>
#include "rgpbuf.h"
#include "core/rgbase.h"

#define RGP_SZNULL16 0x3f
#define RGP_SZNULL32 0x403e
#define RGP_SZNULL64 0x4000403d

void rgpbuf_init(rgpbuf_t* fd, void* buf, rgsize_t size)
{
	rgbase_init(&fd->base, buf, size);
	fd->idx_start = 0;
	fd->szfill = 0;
}
void rgpbuf_clear(rgpbuf_t* fd)
{
	fd->idx_start = 0;
	fd->szfill = 0;
}

bool rgpbuf_write64(rgpbuf_t* fd, const void* src, uint64_t n)
{
	rgsize_t szfree = fd->base.size - fd->szfill;
	if(n == 0)
		return false;

	if(n <= RGP_SZNULL16) {
		if(n + sizeof(uint8_t) > szfree)
			return false;
		uint8_t header = n;
		prgbase_write(&fd->base, fd->idx_start + fd->szfill, &header, sizeof(header));
	} else if(n <= RGP_SZNULL32) {
		if(n + sizeof(uint16_t) > szfree)
			return false;
		uint16_t header = (uint16_t)RGP_TYPE_16 << 14;
		header &= (n - RGP_SZNULL16);
		prgbase_write(&fd->base, fd->idx_start + fd->szfill, &header, sizeof(header));
	} else if(n <= RGP_SZNULL64) {
		if(n + sizeof(uint32_t) > szfree)
			return false;
		uint32_t header = (uint32_t)RGP_TYPE_32 << 30;
		header &= (n - RGP_SZNULL16);
		prgbase_write(&fd->base, fd->idx_start + fd->szfill, &header, sizeof(header));
	} else if(n <= 0x3fffffffffffffff) {
		if(n + sizeof(uint64_t) > szfree)
			return false;
		uint64_t header = (uint64_t)RGP_TYPE_64 << 62;
		header &= (n - RGP_SZNULL16);
		prgbase_write(&fd->base, fd->idx_start + fd->szfill, &header, sizeof(header));
	} else {
		return false;
	}
	prgbase_write(&fd->base, fd->idx_start + fd->szfill, src, n);
	return true;
}
