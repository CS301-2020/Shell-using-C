#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <dirent.h>
#include "support.h"
//this is used to print all the files in the directory
void printls(char* pathf)
{
    struct dirent *present_directory;
    DIR *directory=opendir(pathf);
    if(!directory)
    {
        perror("ls : Error");
    }
    else
    {
        while ((present_directory=readdir(directory))!=NULL)
        {
            if(strcmp(present_directory->d_name,".")!=0 && strcmp(present_directory->d_name,"..")!=0)
            {
                printf("%s  ",present_directory->d_name);
            }
        }   
    }
    printf("\n");
    closedir(directory);
}

void printgrep(char* path,char *find)
{
    FILE *fl;
    char fline[200];
    if(fl=fopen(path,"r"))
        {
            while (fgets(fline,200,fl)!=NULL)
            {
                if(fline[strlen(fline)-1]=='\n')
                {
                    fline[strlen(fline)-1]='\0';
                }
                if(strstr(fline,find)!=NULL)
                {
                    printf("%s\n",fline);
                }
            }
            fclose(fl);    
        }
        else
        {
            perror("grep : Error");
        }
}

void moveFile(char *source,char *destination)
{
    FILE *source_file;
    if(source_file=fopen(source,"r"))
    {
        if(rename(source,destination))
            {
                perror("mv : error");
            }
        fclose(source_file);    
    }
    else
    {
        perror("mv : source ");   
    }
}

//Copy file
void copyFile(char *source,char *destination)
{
    FILE *sc,*dt;
    char fline[300];
    if(sc=fopen(source,"r"))
    {
        if(dt=fopen(destination,"w"))
        {
            while(fgets(fline,300,sc)!=NULL)
            {
                fputs(fline,dt);
            }
            fclose(sc);
            fclose(dt);
        }
        else
        {
            perror("cp : destination ");
        }
    }
    else
    {
        perror("cp : source ");
    }
}

void makeDir(char *nam)
{
    if(mkdir(nam,0777)!=0)
    {
        fprintf(stderr,"mkdir : %s : %s\n",nam,strerror(errno));
    }
}

//Print cat files
void printcat(char *fpath)
{
    FILE *fl;
    char fline[300];
    if(fl=fopen(fpath,"r"))
    {
        while (fgets(fline,300,fl)!=NULL)
        {
            printf("%s",fline);
        }
        printf("\n\n");
        fclose(fl);    
    }
    else
    {
        perror("cat : source ");
    }
}

void removeDirectory(char* path)
{
    struct dirent *present_directory;
    DIR *directory=opendir(path);
    if(!directory)
    {
        perror("rm : Source ");
    }
    else
    {
        while ((present_directory=readdir(directory))!=NULL)
        {
            if(strcmp(present_directory->d_name,".")!=0 && strcmp(present_directory->d_name,"..")!=0)
            {
                char filepath[100]="";
                strcat(filepath,path);
                strcat(filepath,"/");
                strcat(filepath,present_directory->d_name);
                if(remove(filepath))
                {
                    removeDirectory(filepath);
                }
            }
        } 
        rmdir(path);  
    }
}

//this is to check if its a valid mode or not
int valid(char *mod)
{
    if(strlen(mod)!=3)
    {
        printf("3");
        return 0;
    }
    for(int i=0;i<strlen(mod);i++)
    {
        if(!isdigit(mod[i]))
        {
            return 0;
        }
        else
        {
            int a=(int)(mod[i]);
            if(a<49 || a>55)
            {
                return 0;
            }
        }
    }   
    return 1;
}