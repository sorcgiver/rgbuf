#include "core/rgbase.h"
#include "rgbuf.h"
#include "rgpbuf.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>

using namespace std;

void complete()
{
	cout << "COMPLETE" << endl;
}

#define errval(str, val, corrctVal)                                                   \
	if (val != corrctVal) {                                                       \
		cout << "FAIL" << endl;                                               \
		cout << __FILE__ << ":" << __LINE__ << ": FAIL " << str << std::endl; \
		cout << ">>>>> " << str << std::endl;                                 \
		cout << ">>>>> " << val << " != " << corrctVal << std::endl;          \
		abort();                                                              \
	}

void test_rgbase_index()
{
	uint8_t buffer[5];
	int index;
	struct rgbase fd;
	rgbase_init(&fd, buffer, sizeof(buffer));

	index = -1;
	rgbase_idxcq(&fd, &index);
	errval("check correct index", index, 4);
	index = -19;
	rgbase_idxcq(&fd, &index);
	errval("check correct index", index, 1);
	index = 5;
	rgbase_idxcq(&fd, &index);
	errval("check correct index", index, 0);
	index = 6;
	rgbase_idxcq(&fd, &index);
	errval("check correct index", index, 1);
	index = 27;
	rgbase_idxcq(&fd, &index);
	errval("check correct index", index, 2);

	complete();
}

void test_prgbase_index()
{
	uint8_t buffer[5];
	int index;
	struct rgbase fd;
	rgbase_init(&fd, buffer, sizeof(buffer));

	index = 0;
	prgbase_idxcq(&fd, &index);
	errval("check correct index", index, 0);
	index = 5;
	prgbase_idxcq(&fd, &index);
	errval("check correct index", index, 0);
	index = 6;
	prgbase_idxcq(&fd, &index);
	errval("check correct index", index, 1);

	complete();
}

void test_pwrite()
{
	uint8_t buffer[5];
	const uint8_t checkbuf[5] = { 1, 2, 3, 4, 5 };
	struct rgbase fd;
	rgbase_init(&fd, buffer, sizeof(buffer));

	for (int i = 0; i < (int)fd.size; i++) {
		memset(buffer, 0, sizeof(buffer));
		prgbase_write(&fd, i, checkbuf, sizeof(checkbuf));
		/*printf("buff: ");*/
		/*for(int j = 0; j < (int)sizeof(buffer); j++)*/
		/*	printf("%d ", buffer[j]);*/
		/*printf("\n");*/
		for (int j = 0; j < (int)sizeof(checkbuf); j++) {
			rgindex_t index = i + j;
			rgbase_idxcq(&fd, &index);
			if (buffer[index] == checkbuf[j])
				continue;
			cout << "FAIL\n";
			printf("test: %d/%d\n", i, fd.size);
			printf("subtest: %d/%d\n", j, (int)sizeof(checkbuf));
			cout << ">>>>> " << (int)buffer[index] << " != " << (int)checkbuf[j] << std::endl;
			abort();
		}
	}
	complete();
}

void test_write()
{
	uint8_t buffer[5];
	const uint8_t checkbuf[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
	struct rgbase fd;
	rgbase_init(&fd, buffer, sizeof(buffer));

	for (int i = 0; i < (int)fd.size; i++) {
		memset(buffer, 0, sizeof(buffer));
		rgsize_t size = rgbase_write(&fd, i, checkbuf, sizeof(checkbuf));
		if (size != fd.size) {
			printf("FAIL");
			printf("not correct copy size: %lu != %lu\n", size, fd.size);
		}
		/*printf("buff: ");*/
		/*for(int j = 0; j < (int)sizeof(buffer); j++)*/
		/*	printf("%d ", buffer[j]);*/
		/*printf("\n");*/
		for (int j = 0; j < (int)sizeof(buffer); j++) {
			rgindex_t index = i + j;
			rgbase_idxcq(&fd, &index);
			if (buffer[index] == checkbuf[j])
				continue;
			cout << "FAIL\n";
			printf("test: %d/%d\n", i, fd.size);
			printf("subtest: %d/%d\n", j, (int)sizeof(checkbuf));
			cout << ">>>>> " << (int)buffer[index] << " != " << (int)checkbuf[j] << std::endl;
			abort();
		}
	}
	complete();
}

void test_pread()
{
	uint8_t buffer[5];
	uint8_t cpubuf[5];
	const uint8_t checkbuf[5] = { 1, 2, 3, 4, 5 };
	struct rgbase fd;
	rgbase_init(&fd, buffer, sizeof(buffer));

	for (int szcpy = 1; szcpy <= (int)fd.size; szcpy++) {
		for (int i = 0; i < (int)fd.size; i++) {
			memset(buffer, 0, sizeof(buffer));
			prgbase_write(&fd, i, checkbuf, szcpy);
			prgbase_read(&fd, cpubuf, i, szcpy);
			for (int j = 0; j < szcpy; j++) {
				if (cpubuf[j] == checkbuf[j])
					continue;
				cout << "FAIL\n";
				printf("number test: %d/%d\n", szcpy, (int)fd.size);
				printf("test: %d/%d\n", i, fd.size);
				printf("subtest: %d/%d\n", j, szcpy);
				cout << ">>>>> " << (int)cpubuf[j] << " != " << (int)checkbuf[j] << std::endl;
				abort();
			}
		}
	}

	complete();
}

void test_read()
{
	uint8_t buffer[5];
	uint8_t cpubuf[5];
	const uint8_t checkbuf[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
	struct rgbase fd;
	rgbase_init(&fd, buffer, sizeof(buffer));

	for (int szcpy = 1; szcpy <= (int)sizeof(checkbuf); szcpy++) {
		int checksize = szcpy;
		if (checksize > (int)fd.size)
			checksize = fd.size;
		for (int i = 0; i < (int)fd.size; i++) {
			memset(buffer, 0, sizeof(buffer));
			rgbase_write(&fd, i, checkbuf, szcpy);
			int retsize = rgbase_read(&fd, cpubuf, i, szcpy);
			if (retsize != checksize) {
				printf("FAIL");
				printf("not correct copy size: %d != %d\n", retsize, checksize);
			}
			for (int j = 0; j < checksize; j++) {
				if (cpubuf[j] == checkbuf[j])
					continue;
				cout << "FAIL\n";
				printf("number test: %d/%d\n", szcpy, (int)fd.size);
				printf("test: %d/%d\n", i, fd.size);
				printf("subtest: %d/%d\n", j, szcpy);
				cout << ">>>>> " << (int)cpubuf[j] << " != " << (int)checkbuf[j] << std::endl;
				abort();
			}
		}
	}
	complete();
}

void test_rgbufwrite()
{
	rgsize_t size;
	uint8_t buffer[5];
	const uint8_t checkbuf[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
	rgbuf_t rb;
	rgbuf_init(&rb, buffer, sizeof(buffer));

	rgbuf_clear(&rb);
	memset(buffer, 0, sizeof(buffer));
	size = rgbuf_write(&rb, checkbuf, 3);
	errval("write", size, 3);
	for (int i = 0; i < 3; i++) {
		if (buffer[i] == checkbuf[i])
			continue;
		cout << "FAIL" << endl;
		printf("not correct writen byte <%d>: 0x%02X != 0x%02X\n", i, buffer[i], checkbuf[i]);
		abort();
	}

	memset(buffer, 0, sizeof(buffer));
	size = rgbuf_write(&rb, checkbuf, 10);
	errval("write", size, 2);
	for (int i = 0; i < 2; i++) {
		if (buffer[i + 3] == checkbuf[i])
			continue;
		cout << "FAIL" << endl;
		printf("not correct writen byte <%d>: 0x%02X != 0x%02X\n", i, buffer[i + 3], checkbuf[i]);
		abort();
	}

	rgbuf_clear(&rb);
	memset(buffer, 0, sizeof(buffer));
	size = rgbuf_write(&rb, checkbuf, 5);
	errval("write", size, 5);
	for (int i = 0; i < 5; i++) {
		if (buffer[i] == checkbuf[i])
			continue;
		cout << "FAIL" << endl;
		printf("not correct writen byte <%d>: 0x%02X != 0x%02X\n", i, buffer[i], checkbuf[i]);
		abort();
	}

	complete();
}

#define errfor(str, arr, arr_check, size)                                                        \
	for (int i = 0; i < size; i++) {                                                         \
		if (arr[i] == arr_check[i])                                                      \
			continue;                                                                \
		cout << "FAIL" << endl;                                                          \
		cout << __FILE__ << ":" << __LINE__ << ": FAIL " << str << std::endl;            \
		cout << str << endl;                                                             \
		cout << "not correct " << i << ": " << arr[i] << " != " << arr_check[i] << endl; \
		abort();                                                                         \
	}

/*void check_correct_fun(void* arr, void* correct, rgsize_t n)*/
/*{*/
/*	uint8_t* arr8 = (uint8_t*)arr;*/
/*	uint8_t* correct8 = (uint8_t*)correct;*/
/*	for (rgsize_t i = 0; i < n; i++) {*/
/*		if (arr8[i] == correct8[i])*/
/*			continue;*/
/*		cout << "FAIL" << endl;*/
/*		cout << __FILE__ << ":" << __LINE__ << ": FAIL " << "error byte correct" << std::endl;*/
/*		cout << "test: " << i + 1 << "/" << n << endl;*/
/*		cout << (int)buffer[i] << " != " << (int)checkbuf[i] << endl;*/
/*		abort();*/
/*	}*/
/*}*/

#define check_correct(arr, correct, n)                                                                 \
	for (rgsize_t i = 0; i < n; i++) {                                                             \
		if (arr[i] == correct[i])                                                              \
			continue;                                                                      \
		cout << "FAIL" << endl;                                                                \
		cout << __FILE__ << ":" << __LINE__ << ": FAIL " << "error byte correct" << std::endl; \
		cout << "test: " << i + 1 << "/" << n << endl;                                         \
		cout << (uint32_t)(arr[i]) << " != " << (uint32_t)(correct[i]) << endl;                \
		abort();                                                                               \
	}

void test_rgbufwriteskip()
{
	rgsize_t size;
	uint8_t buffer[5];
	const uint8_t checkbuf[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	rgbuf_t rb;
	rgbuf_init(&rb, buffer, sizeof(buffer));

	rgbuf_write(&rb, checkbuf, 10);
	rgbuf_skip(&rb, 2);
	rgbuf_write(&rb, checkbuf + 7, 10);
	check_correct(buffer, (checkbuf + 7), 2);
	check_correct((buffer + 2), (checkbuf + 2), 3);

	memset(buffer, 0, sizeof(buffer));
	rgbuf_clear(&rb);
	rgbuf_write(&rb, checkbuf, 3);
	rgbuf_skip(&rb, 2);
	rgbuf_write(&rb, checkbuf, 10);
	check_correct(buffer, (checkbuf + 2), 2);
	check_correct((buffer + 2), (checkbuf + 2), 1);
	check_correct((buffer + 3), checkbuf, 2);

	memset(buffer, 0, sizeof(buffer));
	rgbuf_clear(&rb);
	rgbuf_write(&rb, checkbuf, 10);
	rgbuf_skip(&rb, 10);
	rgbuf_write(&rb, checkbuf, 10);
	check_correct(buffer, checkbuf, 5);
	rgbuf_skip(&rb, 4);
	rgbuf_write(&rb, checkbuf, 10);
	check_correct(buffer, checkbuf, 4);
	check_correct((buffer + 4), (checkbuf + 4), 1);
	rgbuf_skip(&rb, 10);
	rgbuf_write(&rb, checkbuf, 10);
	check_correct(buffer, (checkbuf + 1), 4);
	check_correct((buffer + 4), checkbuf, 1);

	rgbuf_clear(&rb);
	memset(buffer, 0, sizeof(buffer));
	size = rgbuf_write(&rb, checkbuf, sizeof(checkbuf));
	errval("not correct write size", size, 5);
	rgbuf_skip(&rb, 2);
	size = rgbuf_write(&rb, checkbuf, 1);
	errval("not correct write size", size, 1);
	size = rgbuf_write(&rb, checkbuf, 10);
	errval("not correct write size", size, 1);
	check_correct((buffer+0), (checkbuf+0), 1);
	check_correct((buffer+1), (checkbuf+0), 1);
	check_correct((buffer+2), (checkbuf+2), 3);

	complete();
}

void test_rgbufread()
{
	rgsize_t size;
	uint8_t buffer[5];
	uint8_t recvbuf[10];
	const uint8_t checkbuf[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	rgbuf_t rb;
	rgbuf_init(&rb, buffer, sizeof(buffer));

	size = rgbuf_read(&rb, recvbuf, 5);
	errval("not correct read size", size, 0);

	rgbuf_write(&rb, checkbuf, 2);
	size = rgbuf_read(&rb, recvbuf, 5);
	errval("not correct read size", size, 2);
	check_correct(recvbuf, checkbuf, 2);

	rgbuf_write(&rb, checkbuf, 10);
	size = rgbuf_read(&rb, recvbuf, 10);
	errval("not correct read size", size, 5);
	check_correct(recvbuf, checkbuf, 5);

	rgbuf_write(&rb, checkbuf, 10);
	size = rgbuf_read(&rb, recvbuf, 1);
	errval("not correct read size", size, 1);
	check_correct(recvbuf, checkbuf, 1);
	// next
	size = rgbuf_read(&rb, recvbuf, 2);
	errval("not correct read size", size, 2);
	check_correct(recvbuf, (checkbuf+1), 2);
	// next
	size = rgbuf_read(&rb, recvbuf, 5);
	errval("not correct read size", size, 2);
	check_correct(recvbuf, (checkbuf+3), 2);
	complete();
}

void test_rgbufoverwrite()
{
	rgsize_t size;
	uint8_t buffer[5];
	const uint8_t checkbuf[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	rgbuf_t rb;
	rgbuf_init(&rb, buffer, sizeof(buffer));
	memset(buffer, 0, sizeof(buffer));
	
	// default write
	rgbuf_clear(&rb);
	memset(buffer, 0, sizeof(buffer));
	size = rgbuf_overwrite(&rb, checkbuf, sizeof(checkbuf));
	errval("not correct write size", size, 5);
	rgbuf_skip(&rb, 2);
	size = rgbuf_overwrite(&rb, checkbuf, 1);
	errval("not correct write size", size, 1);
	size = rgbuf_overwrite(&rb, checkbuf, 1);
	errval("not correct write size", size, 1);
	check_correct((buffer+0), (checkbuf+0), 1);
	check_correct((buffer+1), (checkbuf+0), 1);
	check_correct((buffer+2), (checkbuf+2), 3);

	rgbuf_clear(&rb);
	memset(buffer, 0, sizeof(buffer));
	size = rgbuf_overwrite(&rb, checkbuf, 0);
	errval("not correct write size", size, 0);

	size = rgbuf_overwrite(&rb, checkbuf, 2);
	errval("not correct write size", size, 2);
	check_correct(buffer, checkbuf, 2);

	size = rgbuf_overwrite(&rb, checkbuf, 3);
	errval("not correct write size", size, 3);
	check_correct((buffer+2), (checkbuf+0), 3);
	check_correct((buffer+0), (checkbuf+0), 2);


	// overwrite
	size = rgbuf_overwrite(&rb, checkbuf, 3);
	errval("not correct write size", size, 3);
	errval("not correct fill size", rb.szfill, 5);
	check_correct((buffer+3), (checkbuf+1), 2);
	check_correct((buffer+0), (checkbuf+0), 3);

	size = rgbuf_overwrite(&rb, checkbuf, 10);
	errval("not correct write size", size, 5);
	errval("not correct index", rb.idx_start, 3);
	errval("not correct fill size", rb.szfill, 5);
	check_correct((buffer+3), (checkbuf+0), 2);
	check_correct((buffer+0), (checkbuf+2), 3);
	complete();
}

void test_rgbufpeek()
{
	rgsize_t size;
	uint8_t buffer[5];
	uint8_t recvbuf[10];
	const uint8_t checkbuf[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	rgbuf_t rb;
	rgbuf_init(&rb, buffer, sizeof(buffer));
	memset(buffer, 0, sizeof(buffer));

	size = rgbuf_peek(&rb, 0, recvbuf, 1);
	errval("not correct peek size", size, 0);

	rgbuf_write(&rb, checkbuf, 10);
	// next
	size = rgbuf_peek(&rb, 0, recvbuf, 2);
	errval("not correct peek size", size, 2);
	check_correct(recvbuf, checkbuf, 2);
	// next
	size = rgbuf_peek(&rb, 3, recvbuf, 2);
	errval("not correct peek size", size, 2);
	check_correct(recvbuf, (checkbuf+3), 2);
	// next
	size = rgbuf_peek(&rb, 4, recvbuf, 2);
	errval("not correct peek size", size, 1);
	check_correct(recvbuf, (checkbuf+4), 1);

	rgbuf_skip(&rb, 2);
	rgbuf_write(&rb, checkbuf, 10);
	rgbuf_overwrite(&rb, checkbuf, 10);
	// next
	size = rgbuf_peek(&rb, 0, recvbuf, 2);
	errval("not correct peek size", size, 2);
	check_correct(recvbuf, (checkbuf+0), 2);
	// next
	size = rgbuf_peek(&rb, 3, recvbuf, 2);
	errval("not correct peek size", size, 2);
	check_correct(recvbuf, (checkbuf+3), 2);
	// next
	size = rgbuf_peek(&rb, 4, recvbuf, 2);
	errval("not correct peek size", size, 1);
	check_correct(recvbuf, (checkbuf+4), 1);
	complete();
}

#define test_info(str) \
	cout << "CHECK: " << setw(50) << left << str;

int main(int argc, char* argv[])
{
	test_info("set index correct");
	test_rgbase_index();
	test_info("set pindex correct");
	test_prgbase_index();
	test_info("check pwrite correct");
	test_pwrite();
	test_info("check write correct");
	test_write();
	test_info("check pread correct");
	test_pread();
	test_info("check read correct");
	test_read();
	test_info("check rgbuf_t write");
	test_rgbufwrite();
	test_info("check rgbuf_t write + skip");
	test_rgbufwriteskip();
	test_info("check rgbuf_t read");
	test_rgbufread();
	test_info("check rgbuf_t overwrite");
	test_rgbufoverwrite();
	test_info("check rgbuf_t peek");
	test_rgbufpeek();
	return 0;
}
