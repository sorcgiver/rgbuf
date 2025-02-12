#define PRIVATE_RGPBUF
#include "rgpbuf.h"
#include "core/rgbase.h"
#include <stdbool.h>

#define RGP_SZNULL16 0x3f
#define RGP_SZNULL32 0x403e
#define RGP_SZNULL64 0x4000403d
#define RGP_SZNULL128 0x400000004000403c

static const rgsize_t szheader[4] = {
	0x01,
	0x02,
	0x04,
	0x08
};

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

inline bool rgpbuf_szcalc64(rgpbuf_t* fd, rgp_type *type, uint64_t n)
{
	rgsize_t szfree = fd->base.size - fd->szfill;
	if (n == 0) {
		return false;
	} else if (n <= RGP_SZNULL16) {
		if (n + sizeof(uint8_t) > szfree)
			return false;
		(*type) = RGP_TYPE_8;
		uint8_t header
		    = n;
		prgbase_write(&fd->base, fd->idx_start + fd->szfill, &header, sizeof(header));
	} else if (n <= RGP_SZNULL32) {
		if (n + sizeof(uint16_t) > szfree)
			return false;
		(*type) = RGP_TYPE_16;
		uint16_t header
		    = (uint16_t)RGP_TYPE_16 << 14;
		header &= (n - RGP_SZNULL16);
		prgbase_write(&fd->base, fd->idx_start + fd->szfill, &header, sizeof(header));
	} else if (n <= RGP_SZNULL64) {
		if (n + sizeof(uint32_t) > szfree)
			return false;
		(*type) = RGP_TYPE_32;
		uint32_t header
		    = (uint32_t)RGP_TYPE_32 << 30;
		header &= (n - RGP_SZNULL16);
		prgbase_write(&fd->base, fd->idx_start + fd->szfill, &header, sizeof(header));
	} else if (n <= RGP_SZNULL128) {
		if (n + sizeof(uint64_t) > szfree)
			return false;
		(*type) = RGP_TYPE_64;
		uint64_t header
		    = (uint64_t)RGP_TYPE_64 << 62;
		header &= (n - RGP_SZNULL16);
		prgbase_write(&fd->base, fd->idx_start + fd->szfill, &header, sizeof(header));
	} else {
		return false;
	}
	return true;
}

bool rgpbuf_write64(rgpbuf_t* fd, const void* src, uint64_t n)
{
	rgp_type type;
	if(!rgpbuf_szcalc64(fd, &type, n))
		return false;
	prgbase_write(&fd->base, fd->idx_start + fd->szfill, src, n);
	fd->szfill += szheader[type] + n;
	return true;
}

rgp_type rgpbuf_rtype(rgpbuf_t* fd)
{
	return (rgp_type)(fd->base.buf[fd->idx_start] & 0xc0);
}

uint64_t rgpbuf_rsize64(rgpbuf_t* fd)
{
	uint64_t size;
	if (sizeof(size) < fd->szfill)
		return 0;
	rgbase_read(&fd->base, &size, fd->idx_start, sizeof(size));
	size = size & 0x3fffffffffffffff;
	size += RGP_SZNULL64;
	return size;
}

uint64_t rgpbuf_read64(rgpbuf_t* fd, void* dst)
{
	uint64_t size = rgpbuf_rsize64(fd);
	if (size == 0)
		return 0;
	fd->idx_start += szheader[RGP_TYPE_64];
	prgbase_idxcq(&fd->base, &fd->idx_start);
	prgbase_read(&fd->base, dst, fd->idx_start, size);

	fd->szfill -= (size + szheader[RGP_TYPE_64]);
	fd->idx_start += szheader[RGP_TYPE_64];
	prgbase_idxcq(&fd->base, &fd->idx_start);
	return size;
}
