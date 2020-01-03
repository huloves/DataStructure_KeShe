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
    sprintf(tarfilename, "%s.zip", buf);
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

void TouchCodingFile(char *filename, char *tarfilename, int *times) {
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
    sprintf(untarfilename, "%s.unzip", buf);
    printf("tarfilename:%s\n", untarfilename);
    fd = open(untarfilename, O_WRONLY | O_CREAT, 0777);
    if(fd == -1) {
        my_error("open", __LINE__-2);
        exit(1);
    }
    close(fd);
}

void TouchDecodeFile(char *filename, char *decodefliename) {
    //创建目标文件
    int fd;
    char buf[BUFFER_SIZE];
    for(int i=0; filename[i]!='.'; i++) {
        buf[i] = filename[i];
    }
    sprintf(decodefliename, "%s.decode", buf);
    printf("tarfilename:%s\n", decodefliename);
    fd = open(decodefliename, O_WRONLY | O_CREAT, 0777);
    if(fd == -1) {
        my_error("open", __LINE__-2);
        exit(1);
    }
    close(fd);
}

void SourceToCode(char *sourcefile, char *targetfile, huffman_code hc) {
    unsigned char save = 0; //每次保存一个字节的长度
    int len = 0; //每次保存的一个字节已经存了多少长度
    int fd_target;
    int fd_source;
    char buf_read[1024];
    //打开源文件和目标文件
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

    //压缩
    int ret;
    while((ret = read(fd_source, buf_read, 1024)) && ret > 0) {
        for(int i=0; i<ret; i++) {
            if(buf_read[i] >= 0 && buf_read[i] < 126) {
                int j=0; 
                while(1) {
                    if(buf_read[i] == j) {
                        int hc_len = strlen(hc[j]);
                        for(int k=0; k<hc_len; k++) {
                            if(len != 0) save = save << 1;

                            save = save | (hc[j][k]-'0');
                            len++;
                            //一个字节已存满,写入文件
                            
                            if(len == 8) {
                                write(fd_target, &save, sizeof(save));
                                save = 0;
                                len = 0;
                            }
                        }
                        break;
                    }
                    j++;
                }
            }
            else {
                if((ret-i) >= 3) {
                    char temp[3];
                    memset(temp, 0, sizeof(temp));
                    strncpy(temp, buf_read+i, 3);
                    int j=0;
                    while(1) {
                        if(strncmp(temp, hc[j], 3) == 1) {
                            int hc_len = strlen(hc[j]);
                            for(int k=0; k<hc_len; k++) {
                                //按位操作保存编码
                                if(len != 0) save = save << 1;

                                save = save | hc[j][k] - '0';
                                len++;
                                //一个字节已存满
                                if(len == 8) {
                                    write(fd_target, &save, sizeof(save));
                                    save = 0;
                                    len = 0;
                                }
                            }
                            break;
                        }
                        j++;
                    }
                    i += 2;
                }
                else {
                    lseek(fd_source, ret+i, SEEK_CUR);
                    break;
                }
            }
        }
    }
    if(len != 0) {
        write(fd_target, &save, sizeof(save));
    }
    
    close(fd_source);
    close(fd_target);
}

//编码
void SourceToCode_(char *sourcefile, char *targetfile, huffman_code hc) {
    //打开源文件和目标文件
    int fd_target;
    int fd_source;
    unsigned char buf_read[1];
    char buf_write[BUFFER_SIZE];
    fd_source = open(sourcefile, O_RDONLY);
    if(fd_source == -1) {
        my_error("open", __LINE__-2);
    }
    fd_target = open(targetfile, O_WRONLY);
    if(fd_target == -1) {
        my_error("open", __LINE__-2);
    }
    //源文件指针指向文件尾部
    if(lseek(fd_target, 0, SEEK_END) == -1) {
        my_error("lseek", __LINE__-1);
    }

    //向目标文件写入编码，从源文件读取一字节，写入目标文件
    int ret = read(fd_source, buf_read, 1);
    if(ret == -1) {
        my_error("read", __LINE__-2);
    }

    unsigned char ch;
    while(ret) {
        ch = buf_read[0];
        int choice = ch;
        strcpy(buf_write, hc[ch]);
        if(write(fd_target, buf_write, strlen(buf_write)) == -1) {
            my_error("write", __LINE__-1);
            exit(1);
        }
        ret = read(fd_source, buf_read, 1);
        if(ret == -1) {
            my_error("read", __LINE__-2);
            exit(1);
        }
    }

    //关闭文件
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

char GetByte(int sourcefd) {
    char buf[2];
    read(sourcefd, buf, 1);
    //printf("buf:%c\n", buf[0]);
    char choice = buf[0];
    return choice;
}

