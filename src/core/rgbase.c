#include "rgbase.h"
#include <string.h>

void rgbase_init(struct rgbase* fd, void* buf, rgsize_t size)
{
	fd->buf = (uint8_t*)buf;
	fd->size = size;
}

inline void prgbase_idxcq(const struct rgbase* fd, rgindex_t* index)
{
	if((*index) >= (rgindex_t)fd->size)
		(*index) -= fd->size;
}

inline void rgbase_idxcq(const struct rgbase* fd, rgindex_t* index)
{
	if ((*index) >= 0 && (*index) < (rgindex_t)fd->size)
		return;
	if ((*index) < 0) {
		(*index) *= (-1);
		(*index) %= fd->size;
		(*index) = fd->size - (*index);
	}
	(*index) %= fd->size;
}

rgsize_t prgbase_write(struct rgbase* fd, rgindex_t index, const void* src, rgsize_t n)
{
	rgsize_t sze = index + n;
	if(sze >= fd->size) {
		sze = fd->size - index;
		n -= sze;
		memcpy(fd->buf + index, src, sze);
		src = (uint8_t*)src + sze;
		index = 0;
	} else {
		sze = 0;
	}
	memcpy(fd->buf + index, src, n);
	return sze + n;
}

rgsize_t rgbase_write(struct rgbase* fd, rgindex_t index, const void* src, rgsize_t n)
{
	if(n > fd->size)
		n = fd->size;
	rgbase_idxcq(fd, &index);
	return prgbase_write(fd, index, src, n);
}

rgsize_t rgbase_read(const struct rgbase* fd, void* dst, rgindex_t index, rgsize_t n)
{
	rgbase_idxcq(fd, &index);
	if(n > fd->size)
		n = fd->size;
}
