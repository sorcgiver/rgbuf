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
rgsize_t rgbuf_write(rgbuf_t* fd, const void* src, rgsize_t n)
{
	rgsize_t szfree = fd->base.size - fd->szfill;
	if(n > szfree)
		n = szfree;
	prgbase_write(&fd->base, fd->idx_start, src, n);
	fd->szfill += n;
	return n;
}

rgsize_t rgbuf_ovrwrite(rgbuf_t* fd, const void* src, rgsize_t n)
{
	rgsize_t szfree = fd->base.size - fd->szfill;
	if(n > fd->base.size)
		n = fd->base.size;
	prgbase_write(&fd->base, fd->idx_start, src, n);


}

rgsize_t rgbuf_read(rgbuf_t* fd, void* dst, rgsize_t n)
{
	if(n > fd->szfill)
		n = fd->szfill;
	prgbase_read(&fd->base, dst, fd->idx_start, n);
	fd->szfill -= n;
	return n;
}
