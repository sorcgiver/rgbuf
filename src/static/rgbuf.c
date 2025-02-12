#define PRIVATE_RGBASE
#define PRIVATE_RGBUF
#include "rgbuf.h"
#include "core/rgbase.h"

void rgbuf_init(rgbuf_t* fd, void* buf, rgsize_t size)
{
	rgbase_init(&fd->base, buf, size);
	fd->idx_start = 0;
	fd->szfill = 0;
}

void rgbuf_clear(rgbuf_t* fd)
{
	fd->idx_start = 0;
	fd->szfill = 0;
}

rgsize_t rgbuf_write(rgbuf_t* fd, const void* src, rgsize_t n)
{
	rgsize_t szfree = fd->base.size - fd->szfill;
	if (n > szfree)
		n = szfree;
	prgbase_write(&fd->base, fd->idx_start + fd->szfill, src, n);
	fd->szfill += n;
	return n;
}

rgsize_t rgbuf_overwrite(rgbuf_t* fd, const void* src, rgsize_t n)
{
	rgsize_t szfree = fd->base.size - fd->szfill;
	rgsize_t skip = 0;

	if (n > fd->base.size)
		n = fd->base.size;
	if (szfree < n) {
		skip = n - szfree;
		fd->szfill -= skip;
		fd->idx_start += skip;
		prgbase_idxcq(&fd->base, &fd->idx_start);
	}
	prgbase_write(&fd->base, fd->idx_start + fd->szfill, src, n);
	fd->szfill += n;
	return n;
}

rgsize_t rgbuf_read(rgbuf_t* fd, void* dst, rgsize_t n)
{
	if (n > fd->szfill)
		n = fd->szfill;
	prgbase_read(&fd->base, dst, fd->idx_start, n);
	fd->szfill -= n;
	fd->idx_start += n;
	prgbase_idxcq(&fd->base, &fd->idx_start);
	return n;
}

rgsize_t rgbuf_peek(const rgbuf_t* fd, rgsize_t skip_n, void* dst, rgsize_t n)
{
	rgindex_t idx_start;
	rgsize_t szfill;

	if (skip_n > fd->szfill)
		return 0;
	szfill = fd->szfill - skip_n;
	idx_start = fd->idx_start + skip_n;
	prgbase_idxcq(&fd->base, &idx_start);

	if (n > szfill)
		n = szfill;
	prgbase_read(&fd->base, dst, idx_start, n);
	return n;
}

rgsize_t rgbuf_skip(rgbuf_t* fd, rgsize_t n)
{
	if (n > fd->szfill)
		n = fd->szfill;
	fd->szfill -= n;
	fd->idx_start += n;
	prgbase_idxcq(&fd->base, &fd->idx_start);
	return n;
}
