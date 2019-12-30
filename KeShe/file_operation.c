#include "file_operation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "huffman.h"
#include "my_error.h"

#define FILENAME_SIZE 256
#define BUFFER_SIZE 1024

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

void TouchZipFile(char *filename, char *tarfilename) {
    printf("C\n");
    //创建目标文件
    int fd;
    char buf[BUFFER_SIZE];
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
    close(fd);
}

void SourceToCode(char *sourcefile, char *targetfile, huffman_code hc) {
    int fd_target;
    int fd_source;
    char buf_read[BUFFER_SIZE];
    char buf_write[BUFFER_SIZE];
    fd_source = open(sourcefile, O_RDONLY);
    if(fd_source == -1) {
        my_error("open", __LINE__-2);
    }
    fd_target = open(targetfile, O_WRONLY);
    if(fd_target == -1) {
        my_error("open", __LINE__-2);
    }
    if(lseek(fd_target, 0, SEEK_END) == -1) {
        my_error("lseek", __LINE__-1);
    }

    
    int ret = read(fd_source, buf_read, 1);
    if(ret == -1) {
        my_error("read", __LINE__-2);
    }
    while(ret) {
        unsigned char ch = buf_read[0];
        strcpy(buf_write, hc[ch]);
        /*if(lseek(fd_target, 0, SEEK_END) == -1) {
            my_error("lseek", __LINE__-1);
        }*/
        int ret_w = write(fd_target, buf_write, strlen(buf_write));
        if(ret_w == -1) {
            my_error("write", __LINE__-2);
            exit(1);
        }
        ret = read(fd_source, buf_read, 1);
        if(ret == -1) {
            my_error("read", __LINE__-2);
            exit(1);
        }
    }
    
    close(fd_source);
    close(fd_target);
}
