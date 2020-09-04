#ifndef COMAND_H_INCLUDED
#define COMAND_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <dirent.h>
char* curr_loc();
void lscmd(char **,int);
void grepcmd(char **,int);
void catcmd(char **,int);
void mvcmd(char **,int);
void cpcmd(char **,int);
void cdcmd(char **,int,char*);
void pwdcmd();
void rmcmd(char **,int);
void chmodcmd(char **,int);
void mkdircmd(char **,int);
void runcommand(char*,char**,int,char *);
#endif