#ifndef RGBASE_H
#define RGBASE_H
#include <stdint.h>

typedef uint32_t rgsize_t;
typedef int rgindex_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	struct rgbase {
		uint8_t* buf;
		rgsize_t size;
	};

	void rgbase_init(struct rgbase* fd, void* buf, rgsize_t size);
	void prgbase_idxcq(const struct rgbase* fd, rgindex_t* index);
	void rgbase_idxcq(const struct rgbase* fd, rgindex_t* index);
	rgsize_t prgbase_write(struct rgbase* fd, rgindex_t index, const void* src, rgsize_t n);
	rgsize_t prgbase_read(struct rgbase* fd, rgindex_t index, const void* src, rgsize_t n);
	rgsize_t rgbase_write(struct rgbase* fd, rgindex_t index, const void* src, rgsize_t n);
	rgsize_t rgbase_read(const struct rgbase* fd, void* dst, rgindex_t index, rgsize_t n);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* ifndef RGBASE_H */
