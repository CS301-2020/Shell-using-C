#include "Commands.h"
#include "support.h"
void runcommand(char* command,char** argv,int toknum,char* myloc)
{
    if(strcmp(command,"exit")==0)
        {
            printf("Ready to exit\n");
            exit(EXIT_SUCCESS);
        }
        else if(strcmp(command,"ls")==0)
        {
            lscmd(argv,toknum); 
        }
        else if(strcmp(command,"grep")==0)
        {
            grepcmd(argv,toknum);
        }
        else if(strcmp(command,"cat")==0)
        {
            catcmd(argv,toknum);
        }
        else if(strcmp(command,"mv")==0)
        {
            mvcmd(argv,toknum);
        }
        else if(strcmp(command,"cp")==0)
        {
            cpcmd(argv,toknum);
        }
        else if(strcmp(command,"cd")==0)
        {
            cdcmd(argv,toknum,myloc);
        }
        else if(strcmp(command,"pwd")==0)
        {
            pwdcmd();
        }     
        else if(strcmp(command,"rm")==0)
        {
            rmcmd(argv,toknum);
        }
        else if(strcmp(command,"chmod")==0)
        {
            chmodcmd(argv,toknum);
        }
        else if(strcmp(command,"mkdir")==0)
        {
            mkdircmd(argv,toknum);
        }
        else
        {
            if(command!=" ")
            {
                int rc=fork();
                if(rc<0)
                {
                    fprintf(stderr,"fork failed for %s %s\n",command,strerror(errno));
                    exit(EXIT_FAILURE);
                }
                else if(rc==0)
                {
                    execvp(command,argv);   
                }
                else
                {
                    wait(0);
                }
            }
        }
}

//Function to get current address
char* curr_loc()
{
    char *currLoc;
    long siz;
    char* buf;
    siz = pathconf(".", _PC_PATH_MAX);
    if((buf=(char*)malloc((size_t)siz))!=NULL)
    {
        currLoc=getcwd(buf,(size_t)siz);
    }
    return currLoc;
}

// This is used to execute ls command
void lscmd(char **argv,int argc)
{
    if(argc==1)
    {
        char *pres=curr_loc();
        printls(pres);
    }
    else
    {
        for(int x=1;x<argc;x++)
        {
            char *path=curr_loc();
            strcat(path,"/");
            strcat(path,argv[x]);
            printf("%s :\n",argv[x]);
            printls(path);
            printf("\n");
        }
    }
}

//This is used to execute grep
void grepcmd(char **argv,int argc)
{
    if(argc<=2)
    {
        printf("grep : Not enough arguments");
    }
    else
    {
        for(int x=2;x<argc;x++)
        {
            char *path=curr_loc();
            strcat(path,"/");
            strcat(path,argv[x]);
            printf("%s :\n\n",argv[x]);
            printgrep(path,argv[1]);
        }    
    }
}

//This is used to cat
void catcmd(char **argv,int argc)
{
    if(argc==1)
    {
        printf("cat : No file given :");
    }
    else
    {
        for(int x=1;x<argc;x++)
        {
            char *path=curr_loc();
            strcat(path,"/");
            strcat(path,argv[x]);
            printf("%s : \n",argv[x]);
            printcat(path);
        }
    } 
}

//This is used to execute mv
void mvcmd(char **argv,int argc)
{
    if(argc==1)
    {
        printf("mv : missing file operand");
    }
    else if(argc==2)
    {
        printf("mv : missing destination after %s",argv[1]);
    }
    else if(argc==3)
    {
        DIR *dr;
        dr=opendir(argv[2]);
        if(dr)
        {
            char *dest=strcat(argv[2],"/");
            strcat(dest,argv[1]);
            moveFile(argv[1],dest);
            closedir(dr);
        }
        else
        {
            moveFile(argv[1],argv[2]);
        }
    }
    else
    {
        DIR *dr;
        dr=opendir(argv[argc-1]);
        if(dr)
        {
            for(int i=1;i<argc-1;i++)
            {
                char dest[100]="";
                strcat(dest,argv[argc-1]);
                strcat(dest,"/");
                strcat(dest,argv[i]);
                moveFile(argv[i],dest);
            }
            closedir(dr);
        }
        else
        {
            perror("mv : destination");
        }
    }
}

//This is used to execute cp
void cpcmd(char **argv,int argc)
{
    if(argc<=2)
    {
        printf("cp : not enough arguments");
    }
    else if(strcmp(argv[1],"-r")!=0)
    {
        if(argc==3)
        {
            if(strcmp(argv[1],argv[2])==0)
            {
                printf("cp : %s %s are same files",argv[1],argv[2]);
            }
            else
            {
                DIR *dr;
                dr=opendir(argv[2]);
                if(dr)
                {
                    char *dest=argv[2];
                    strcat(dest,"/");
                    strcat(dest,argv[1]);
                    copyFile(argv[1],dest);
                    closedir(dr);
                }
                else
                {
                    copyFile(argv[1],argv[2]);
                }
            }
        }
        else
        {
            DIR *dr;
            dr=opendir(argv[argc-1]);
            if(dr)
            {
                for(int i=1;i<argc-1;i++)
                {
                    char dest[100]="";
                    strcat(dest,argv[argc-1]);
                    strcat(dest,"/");
                    strcat(dest,argv[i]);
                    copyFile(argv[i],dest);
                }
                closedir(dr);    
            }
            else
            {
                perror("cp : destination ");
            }
        }
    }
    else
    {
        if(argc==4)
        {
            DIR *sc,*dt;
            sc=opendir(argv[2]);
            if(sc)
            {
                struct dirent *pd;
                dt=opendir(argv[3]);
                if(!dt)
                {
                    if(mkdir(argv[3],777)==0)
                    {
                        while ((pd=readdir(sc))!=NULL)
                        {
                            if(strcmp(pd->d_name,".")!=0 && strcmp(pd->d_name,"..")!=0)
                            {
                                char paths[100]="",pathd[100]="";
                                strcat(pathd,argv[3]);
                                strcat(pathd,"/");
                                strcat(pathd,pd->d_name);
                                strcat(paths,argv[2]);
                                strcat(paths,"/");
                                strcat(paths,pd->d_name);
                                FILE *ft;
                                if(ft=fopen(paths,"r"))
                                {
                                    fclose(ft);
                                    copyFile(paths,pathd);
                                }
                            }
                        }
                        closedir(dt);
                        closedir(dt);
                    }
                    else
                    {
                        perror("cp : destination ");
                    }
                }
                else
                {
                    char *original_loc=curr_loc();
                    char *togopath=curr_loc();
                    strcat(togopath,"/");
                    strcat(togopath,argv[3]);
                    chdir(togopath);
                    if(mkdir(argv[2],777)==0)
                    {
                        chdir(original_loc);
                        while ((pd=readdir(sc))!=NULL)
                        {
                            if(strcmp(pd->d_name,".")!=0 && strcmp(pd->d_name,"..")!=0)
                            {
                                char paths[100]="",pathd[100]="";
                                strcat(pathd,argv[3]);
                                strcat(pathd,"/");
                                strcat(pathd,argv[2]);
                                strcat(pathd,"/");
                                strcat(pathd,pd->d_name);
                                strcat(paths,argv[2]);
                                strcat(paths,"/");
                                strcat(paths,pd->d_name);
                                FILE *ft;
                                if(ft=fopen(paths,"r"))
                                {
                                    fclose(ft);
                                    copyFile(paths,pathd);
                                }
                            }
                        }
                        closedir(dt);
                        closedir(sc);
                    }
                    else
                    {
                        chdir(original_loc);
                        perror("cp : destination");
                    }
                }
            }
            else
            {
                perror("cp : source ");
            }
        }
        else
        {
            printf("cp : invalid number of argumemnts");
        } 
    }
}

//This is used to execute cd 
void cdcmd(char **argv,int argc,char *myloc) 
{
    if(argc>2)
    {
        printf("cd : To many arguments");
    }
    else
    {
        if(argc==2)
        {
            char *pres=curr_loc();
            if(argv[1][0]!='/')
            {
                strcat(pres,"/");
            }
            strcat(pres,argv[1]);
            if(chdir(pres)!=0)
            {
                perror("cd : Error");
            }
        }
        else
        {
            if(chdir(myloc)!=0)
            {
                perror("cd : Error");
            }
        }
    }
}

//This is used to execute pwd
void pwdcmd()
{
    char *path =curr_loc();
    printf("%s",path);
}

//This is used to execute rm
void rmcmd(char **argv,int argc)
{
    if(argc==1)
    {
        printf("rm : no file inputed");
    }
    else if(strcmp(argv[1],"-r")!=0)
    {
        for(int i=1;i<argc;i++)
        {
            if(unlink(argv[i]))
            {
                fprintf(stderr,"rm : %s : %s",argv[i],strerror(errno));   
            }
        }
    }
    else
    {
        char *pathd;
        for(int i=2;i<argc;i++)
        {
            pathd=argv[i];
            removeDirectory(pathd);
        }
    }
    
}

//This is used to execute chmod
void chmodcmd(char **argv,int argc)
{
    if(argc==3)
    {
        if(valid(argv[1]))
        {
            int mode = strtol(argv[1], NULL, 8);
            if(chmod(argv[2],mode))
            {
                perror("chmod ");
            }
        }
        else
        {
            printf("chmod : invalid mode");
        }
        
    }
    else
    {
        printf("chmod : invalid arguments");
    }
    
}

//This is used to execute
void mkdircmd(char **argv,int argc)
{
    if(argc==1)
    {
        printf("mkdir : Directory name missing");
    }
    else
    {
        for(int x=1;x<argc;x++)
        {
            makeDir(argv[x]);
        }
    }
}
