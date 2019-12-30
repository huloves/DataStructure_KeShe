objects = hfm_tar.o zip.o huffman.o file_operation.o my_error.o
cc = gcc
_tar : $(objects)
	cc -o  _tar -g $(objects)

hfm_tar.o : hfm_tar.c
zip.o : zip.c zip.h
huffman.o : huffman.c huffman.h
file_operation.o : file_operation.c file_operation.h
my_error : my_error.c my_error.h
.PHONY : clean
clean :
	rm _tar hfm_tar.o zip.o file_operation.o my_error.o huffman.o
