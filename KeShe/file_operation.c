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
    unsigned char buf_read[1];
    unsigned char buf_write[1];
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
    char buf_overflow[8];
    while(ret != 0) {
        unsigned char ch = buf_read[0];
        //构建8位的wb，用于以位为单位写入文件
        unsigned char wb = 0;
        int i=0;
        while(i<8) {
            int len = strlen(hc[ch]);
            int j;
            for(j=0; j<len && i+j<8; j++) {
                if(hc[ch][j] != 0) {
                    wb &= bit[i+j];
                }
                ret = read(fd_source, buf_read, 1);
            }
            i += j;
            int tmp=0;
            int k;
            for(k=j; k<len; k++) {
                buf_overflow[tmp++] = hc[ch][k];
            }
            buf_overflow[k] = '\0';
            ret = read(fd_source, buf_read, 1);
            if(ret == -1) {
                my_error("read", __LINE__-2);
                exit(1);
            }
            ch = buf_read[0];
        }
        //将构建的wb写入文件
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
    int source_size = atoi(buf);
    char read_buf[1];
    if(read(fd, read_buf, 1) == -1) {
        my_error("read", __LINE__-1);
        exit(1);
    }
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
}

int GetSourceSize(char *filename) {
    int fd = open(filename, O_RDONLY);
    if(fd == -1) {
        my_error("open", __LINE__-2);
        exit(1);
    }
    char buf[1024];
    MyGetLine(fd, buf);
    MyGetLine(fd, buf);
    int size = atoi(buf);
    close(fd);
    printf("buf:%s\n", buf);
    return size;
}

int GetBit(int sourcefd) {
    static int i=7;
    static unsigned char Bchar;
    int x;
    unsigned char bit[8] = {128, 64, 32, 16, 8, 4, 2, 1};
    i++;
    char buf[1];
    if(i == 8) {
        read(sourcefd, buf, 1);
        Bchar = buf[0];
        i = 0;
    }
    return (Bchar&bit[i]);
}
