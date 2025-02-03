#include "rgbase.h"
#include <string.h>

void rgbase_init(struct rgbase* fd, void* buf, rgsize_t size)
{
	fd->buf = (uint8_t*)buf;
	fd->size = size;
}

inline void prgbase_idxcq(const struct rgbase* fd, rgindex_t* index)
{
	if ((*index) >= (rgindex_t)fd->size)
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

inline void prgbase_cpycalc(const struct rgbase* fd, rgindex_t index, rgsize_t n, rgsize_t (*size)[2])
{
	(*size)[0] = fd->size - index;
	if ((*size)[0] > n)
		(*size)[0] = n;
	(*size)[1] = n - (*size)[0];
}

void prgbase_write(struct rgbase* fd, rgindex_t index, const void* src, rgsize_t n)
{
	rgsize_t size[2];
	prgbase_cpycalc(fd, index, n, &size);
	memcpy(fd->buf + index, src, size[0]);
	memcpy(fd->buf, (uint8_t*)src + size[0], size[1]);
}

void prgbase_read(const struct rgbase* fd, void* dst, rgindex_t index, rgsize_t n)
{
	rgsize_t size[2];
	prgbase_cpycalc(fd, index, n, &size);
	memcpy(dst, fd->buf + index, size[0]);
	memcpy((uint8_t*)dst + size[0], fd->buf, size[1]);
}

rgsize_t rgbase_write(struct rgbase* fd, rgindex_t index, const void* src, rgsize_t n)
{
	if (n > fd->size)
		n = fd->size;
	rgbase_idxcq(fd, &index);
	prgbase_write(fd, index, src, n);
	return n;
}

rgsize_t rgbase_read(const struct rgbase* fd, void* dst, rgindex_t index, rgsize_t n)
{
	if (n > fd->size)
		n = fd->size;
	rgbase_idxcq(fd, &index);
	prgbase_read(fd, dst, index, n);
	return n;
}
