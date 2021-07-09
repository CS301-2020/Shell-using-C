#ifndef SUPPORT_H_INCLUDED
#define SUPPORT_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <dirent.h>
void printls(char*);
void printgrep(char*,char *);
void moveFile(char *,char *);
void copyFile(char *,char *);
void makeDir(char *);
void printcat(char *);
void removeDirectory(char*);
int valid(char *);
#endif