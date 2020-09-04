#include "Commands.h"
#define MAX_LEN 1024
#define MAX_AR 50
int parseline(char *, char **);
void resetToken(char**,int);
char* myloc;
char line[MAX_LEN];
int main()
{
    long size;
    char *buffer;
    char *pdir;//Present Directory
    char *argv[MAX_AR];
    char* command="";
    int toknum=0;//Number of tokens
    myloc=curr_loc();
    do
    {
        //Method to get present directory
        pdir=curr_loc();
        printf("\n");
        printf("coolie~code=>%s>\n=>",pdir);
        //end of method
        fgets(line,MAX_LEN,stdin);
        toknum=parseline(line,argv);//used to parse the line and get the number of token
        command=" ";
        if(argv[0]!=NULL)
        {
            command=argv[0];
        }
        if(strcmp(argv[toknum-1],"&")==0)
        {
            int rc=fork();
            if(rc<0)
            {
                fprintf(stderr,"fork failed for %s %s\n",command,strerror(errno));
                exit(EXIT_FAILURE);
            }
            else if(rc==0)
            {
                argv[toknum-1]=NULL;
                toknum=toknum-1;
                printf("%s : running in background : PID: %d\n",command,(int)getpid());
                runcommand(command,argv,toknum,myloc);
                resetToken(argv,toknum);
                printf("PID: %d : completed\n",(int)getpid());
                break;
            }
            else
            {
                resetToken(argv,toknum);
                continue;   
            }
        }
        runcommand(command,argv,toknum,myloc);
        resetToken(argv,toknum);
    } while(1);
    return 0;
}

//This is used to empty the argv
void resetToken(char** argv,int argc)
{
    for(int i=0;i<argc;i++)
    {
        argv[i]='\0';
    }
}

//This is used to tokenaize
int parseline(char *line, char **argv)
{
    int f=0;
        //preperation to parse
        for(int i=0;1;i++)
        {
            if(f==0 && line[i]==' ')
            {
                line[i]='\'';
            }
            else if(line[i]=='\"')
            {
                line[i]='\'';
                if(f==0)
                {
                    f=1;
                }
                else
                {
                    f=0;
                }
            }
            if(line[i]=='\n')
            {
                break;
            }
        }
        if(line[strlen(line)-1]=='\n')
        {
            line[strlen(line)-1]='\0';
        }
    int number=0;
    char *token=strtok(line,"\'");
    while (token!=NULL)
    {
        argv[number]=token;
        number++;
        token =strtok(NULL,"\'");
    }
    argv[number]=NULL;
    return number;
}
