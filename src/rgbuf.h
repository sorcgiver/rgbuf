#ifndef RGBUF_H
#define RGBUF_H

#include <cstdint>

typedef uint32_t _rgsize_t;
typedef int _rgindex_t;

/*! \class rgbuf
 *  \brief Brief class description
 *
 *  Detailed description
 */
/*class rgbuf*/
/*{*/
/*public:*/
/*	rgbuf();*/
/*	virtual ~rgbuf();*/
/*};*/

template <_rgsize_t bufferSize>
class _rgbufBase {
    protected:
	uint8_t buffer[bufferSize];

	_rgsize_t copy(_rgindex_t index, const void* src, _rgsize_t n);
	_rgsize_t copy(void* dst, _rgindex_t index, _rgsize_t n);
	void indexSetCorrect(_rgindex_t& index);
};

template <_rgsize_t sz>
_rgsize_t _rgbufBase<sz>::copy(_rgindex_t index, const void* src, _rgsize_t n)
{
	return 0;
}

template <_rgsize_t sz>
_rgsize_t _rgbufBase<sz>::copy(void* dst, _rgindex_t index, _rgsize_t n)
{
	return 0;
}

template <_rgsize_t sz>
void _rgbufBase<sz>::indexSetCorrect(_rgindex_t& index)
{
	if (index >= 0 && index < sz)
		return;
	if (index < 0) {
		index *= (-1);
		index %= sz;
		index = sz - index;
	}
	index %= sz;
}

#endif /* ifndef RGBUF_H */
