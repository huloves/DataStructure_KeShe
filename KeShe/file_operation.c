#include "file_operation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "my_error.h"

#define FILENAME_SIZE 256

void FileCharCount(char *filename, int *times) {
    int fd = open(filename, O_RDONLY);
    if(fd == -1) {
        my_error("open", __LINE__-2);
    }
    char buf[2];
    int ret = read(fd, buf, 1);
    if(ret == -1) {
        my_error("read", __LINE__-2);
    }
    unsigned char ch;
    while(ret != 0) {
        ch = buf[0];
        times[ch]++;
        ret = read(fd, buf, 1);
        if(ret == -1) {
            my_error("read", __LINE__-2);
        }
    }
    close(fd);
}

void TouchZipFile(char *filename) {
    //创建目标文件
    int fd;
    char buf[1024];
    char tarfilename[FILENAME_SIZE];
    for(int i=0; filename[i]!='.'; i++) {
        buf[i] = filename[i];
    }
    sprintf(tarfilename, "%s.code", buf);
    printf("tarfilename:%s\n", tarfilename);
    fd = open(tarfilename, O_WRONLY | O_CREAT, 0777);
    if(fd == -1) {
        my_error("open", __LINE__-2);
        exit(1);
    }
    //在目标文件中写入源文件信息
    sprintf(buf, "%s\n", filename);
    printf("buf:%s", buf);
    int ret = write(fd, buf, strlen(buf));
    if(ret == -1) {
        my_error("write", __LINE__-2);
        exit(1);
    }
}
