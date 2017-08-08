#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

ssize_t pread(int fd, void *buf, size_t count, off_t offset)
	{
	off_t pos = lseek(fd, 0, SEEK_SET);
	if(pos < 0) return -1;
	off_t err = lseek(fd, offset, SEEK_SET);
	if(err < 0) return -1;
	ssize_t rd = read(fd, buf, count);
	err = lseek(fd, pos, SEEK_SET);
	if((err < 0)||(rd < 0)) return -1;
	return rd;
	}

ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset)
	{
	off_t pos = lseek(fd, 0, SEEK_SET);
	if(pos < 0) return -1;
	off_t err = lseek(fd, offset, SEEK_SET);
	if(err < 0) return -1;
	ssize_t rd = write(fd, buf, count);
	err = lseek(fd, pos, SEEK_SET);
	if((err < 0)||(rd < 0)) return -1;
	return rd;
	}
