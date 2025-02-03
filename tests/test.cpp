#include "core/rgbase.h"
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
	uint8_t checkbuf[5] = { 1, 2, 3, 4, 5 };
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
	uint8_t checkbuf[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
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
	uint8_t checkbuf[5] = { 1, 2, 3, 4, 5 };
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
	uint8_t checkbuf[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
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
	return 0;
}
