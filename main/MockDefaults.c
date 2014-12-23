/*  Copyright (C) 2014  Adam Green (https://github.com/adamgreen)

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/
#include <Console.h>
#include <stdio.h>


/* Not using my test mocks in production so point hooks to Standard CRT functions. */
FILE*  (*hook_fopen)(const char* filename, const char* mode) = fopen;
int    (*hook_fclose)(FILE* stream) = fclose;
size_t (*hook_fread)(void* ptr, size_t size, size_t nitems, FILE* stream) = fread;
int    (*hook_fseek)(FILE* stream, long offset, int whence) = fseek;
long   (*hook_ftell)(FILE* stream) = ftell;
char*  (*hook_fgets)(char * str, int size, FILE * stream) = fgets;
void*  (*hook_malloc)(size_t size) = malloc;
void*  (*hook_realloc)(void* ptr, size_t size) = realloc;
void   (*hook_free)(void* ptr) = free;
int    (*hook_printf)(const char* pFormat, ...) = printf;


/* Provide different implementation of Console* functions depending on whether building for a Posix or Windows OS. */
#ifdef WIN32
/* Windows */

#else
/* Posix */

#include <sys/select.h>
#include <unistd.h>

int Console_HasStdInDataToRead()
{
    int            result = -1;
    struct timeval zeroTimeout = {0, 0};
    fd_set         readSet;

    FD_ZERO(&readSet);
    FD_SET(STDIN_FILENO, &readSet);
    result = select(STDIN_FILENO + 1, &readSet, NULL, NULL, &zeroTimeout);
    if (result == -1)
        __throw(fileException);
    return result;
}

int Console_ReadStdIn()
{
    ssize_t result = -1;
    char    c = 0;

    result = read(STDIN_FILENO, &c, sizeof(c));
    if (result == -1)
        __throw(fileException);
    return c;
}

void Console_WriteStdOut(int character)
{
    ssize_t result = -1;
    char    c = (char)character;

    result = write(STDOUT_FILENO, &c, sizeof(c));
    if (result == -1)
        __throw(fileException);
}

#endif /* WIN32 */
