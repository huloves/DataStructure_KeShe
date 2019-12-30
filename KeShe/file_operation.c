#include "file_operation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <math.h>

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
            exit(1);
        }
    }
    close(fd);
}

void TouchZipFile(char *filename, char *tarfilename, int *times) {
    //创建目标文件
    int fd;
    char buf[BUFFER_SIZE];
    struct stat stat_struct;
    stat(filename, &stat_struct); //获得文件属性
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
    sprintf(buf, "%s\n%ld\n", filename, stat_struct.st_size);
    printf("buf:%s", buf);
    int ret = write(fd, buf, strlen(buf));
    if(ret == -1) {
        my_error("write", __LINE__-2);
        exit(1);
    }
    for(int i=0; i<256; i++) {
        sprintf(buf, "%d ", times[i]);
        if(write(fd, buf, strlen(buf)) == -1) {
            my_error("write", __LINE__-1);
            exit(1);
        }
    }
    sprintf(buf, "\n");
    if(write(fd, buf, strlen(buf)) == -1) {
        my_error("write", __LINE__-1);
        exit(1);
    }
    close(fd);
}

void TouchUnZipFile(char *filename, char *untarfilename) {
    //创建目标文件
    int fd;
    char buf[BUFFER_SIZE];
    for(int i=0; filename[i]!='.'; i++) {
        buf[i] = filename[i];
    }
    sprintf(untarfilename, "%s.uncode", buf);
    printf("tarfilename:%s\n", untarfilename);
    fd = open(untarfilename, O_WRONLY | O_CREAT, 0777);
    if(fd == -1) {
        my_error("open", __LINE__-2);
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
    unsigned char bit[8] = {128, 64, 32, 16, 8, 4, 2, 1};
    while(ret != 0) {
        unsigned char ch = buf_read[0];
        unsigned char wb = 0;
        int len = strlen(hc[ch]);
        for(int i=0; i<len; i++) {
            if(hc[ch][i] != 0) {
                wb &= bit[i];
            }
        }
        buf_write[0] = wb;
        if(write(fd_target, buf_write, 1) == -1) {
            my_error("write", __LINE__-1);
        }
        ret = read(fd_source, buf_read, 1);
        if(ret == -1) {
            my_error("read", __LINE__-2);
        }
    }

    close(fd_source);
    close(fd_target);
}

int MyGetLine(int fd, char *buf) {
    char buf_char[1];
    int read_size = 0;
    
    int ret = read(fd, buf_char, 1);
    if(ret == -1) {
        my_error("read", __LINE__-2);
        exit(1);
    }
    if(ret == 0) {
        return -1;
    }
    strcpy(buf, buf_char);
    while(buf[read_size] != '\n') {
        read_size++;
        ret = read(fd, buf_char, 1);
        if(ret == -1) {
            my_error("read", __LINE__-2);
            return -1;
        }
        strcat(buf, buf_char);
    }
    int len = strlen(buf);
    buf[len-1] = '\0';
    return read_size;
}

void GetTimes(char *filename, int *times) {
    int fd;
    fd = open(filename, O_RDONLY);
    if(fd == -1) {
        my_error("open", __LINE__-2);
        exit(1);
    }
    char buf[1024];
    //读取两行，文件指针指向第行
    MyGetLine(fd, buf);
    MyGetLine(fd, buf);
    char read_buf[1];
    if(read(fd, read_buf, 1) == -1) {
        my_error("read", __LINE__-1);
        exit(1);
    }
    //printf("%c\n", read_buf[0]);
    int i=0;
    while(read_buf[0] != '\n') {
        int k=0;
        while(read_buf[0] != ' ' && read_buf[0] != '\n') {
            buf[k++] = read_buf[0];
            if(read(fd, read_buf, 1) == -1) {
                my_error("read", __LINE__-1);
                exit(1);
            }
        }
        if(read_buf[0] == ' ') {
            if(read(fd, read_buf, 1) == -1) {
                my_error("read", __LINE__-1);
                exit(1);
            }
        }
        buf[k] = '\0';
        int temp = atoi(buf);
        times[i++] = temp;
    }
    close(fd);
    /*for(int i=0; i<256; i++) {
        printf("%d-----------%d\n", i, times[i]);
    }*/

}

