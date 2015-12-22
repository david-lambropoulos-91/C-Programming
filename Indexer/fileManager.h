#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <stdio.h>
#include <sys/types.h> 
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

/**
*	isRegularFile takes in a string name of a path and will
*	return an integer value 0 if it is not a regular file 
*	and an integer vale 1 if it is a regular file.
**/
int isRegularFile(const char* path);

/**
*	isDirectory takes in a string name of a path and will
*	return an integer value 0 if it is not a directory 
*	and an integer vale 1 if it is a directory.
**/
int isDirectory(const char* path);

/**
*	isCharacterDevice takes in a string name of a path and will
*	return an integer value 0 if it is not a character device 
*	and an integer vale 1 if it is a character device.
**/
int isCharacterDevice(const char* path);

/**
*	isBlockDevice takes in a string name of a path and will
*	return an integer value 0 if it is not a block device 
*	and an integer vale 1 if it is a block device.
**/
int isBlockDevice(const char* path);

/**
*	isPipe takes in a string name of a path and will
*	return an integer value 0 if it is not a FIFO (pipe) 
*	and an integer vale 1 if it is a FIFO (pipe).
**/
int isPipe(const char* path);

/**
*	isSymbolicLink takes in a string name of a path and will
*	return an integer value 0 if it is not a symbolic link 
*	and an integer vale 1 if it is a symbolic link.
**/
int isSymbolicLink(const char* path);

/**
*	isSocket takes in a string name of a path and will
*	return an integer value 0 if it is not a socket 
*	and an integer vale 1 if it is a socket.
**/
//int isSocket(const char* path);

/**
*	fileToString will take in a pointer to a FILE and will
*	construct a single array out of the contents of the file.
**/
char* fileToString(const char* path);

/**
*	bufferSize determines the amount of characters contained
*	in a file and returns the size of type long int.
**/
long int bufferSize(FILE* fp);

/**
*	Walks through a directory and returns an integer value
*	that is the count of files in that directory. Returns
*	0 if the file is empty or does not exist.
**/
int numFilesInDirectory(const char* path);

/**
*	Returns 1 if a file exists, returns 0 if it does not.
**/
int Exists(const char* path);

/**
*	Creates an array that is of the size of the number of
*	files in the directory specified. At each index ine of
*	the array a string containing the contents of that file
*	from the directory is stored.
**/
char** directoryToString(const char* path);

/**
*	Given a specified path, open a directory and print the 
*	names of the files in that directory.	
**/
void printFilesInDirectory(const char* path);

/**
*	Returns 0 if a file is not binary and a 1 if a file is binary.
**/
int isBinary(const char* path);

/**
*	Returns an integer count of the amount of binary files
*	contained in a directory.
**/
int numBinaryFileInDirectory(const char* path);

#endif
