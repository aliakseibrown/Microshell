#include <stdio.h>
#include <stdbool.h> //for while(true)
#include <string.h>
#include <unistd.h> //for getcwd, chdir, fork
#include <stdlib.h>
#include <sys/wait.h> //wait
#include<signal.h>

#include <termios.h> //getch

char input[1024];
char* words[1024];
char cwd[1024];
int a, h = 0;
char str[20];

char* const tmp;

static void sigintHandler(int sig){
    write(STDERR_FILENO, " Caught SIGINT!\n", 15);
}
const char commands[6][20] =
        {"exit",
         "cd",
         "help",
         "ccolor",
         "./",
         "clear"
        };

const char color[8][30] =
        {"\x1B[0m", //reset 0
         "\x1B[32m", // green 1
         "\x1B[33m", // yellow 2
         "\x1B[34m",// blue 3
         "\x1B[35m", // magenta 4
         "\x1B[36m", // cyan 5
         "\x1B[37m", //white 6
         "\x1B[31m" //red 7
        };
void get_input();

int arraycolors[5];

void showcwd(){
    getcwd(cwd, sizeof(cwd));
    char* const tmp = getenv("USER");
    //printf("%s%s: %s[~%s]%s$%s ",color[1], tmp, color[2], cwd, color[7], color[0]);
    printf("%s%s: %s[~%s]%s$%s ",color[arraycolors[0]], tmp, color[arraycolors[1]], cwd, color[arraycolors[2]], color[arraycolors[3]]);
}

int getche(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}
char history[100][1024];

int ggetch();
void get_input() {
    printf("\b \b");
    printf(" ");
    //fgets(input, 1024, stdin);

    //history[h] = fgets(input, 1024, stdin);
    //if(history[h] != NULL){
    //    h++;
    //}
    //printf("%s", history[h]);
    //int j;
    //input[j] = "\0";
    char ch;
    int j = 0, k = 0, b = 0;
    b = h;
    for(j=0;(ch=ggetch())!='\n';){
        if(ch!= 127 ){
            input[j]=ch;
            printf("%c",input[j]);
            j++;
        }else{
            j--;
            if(j<0)
                j++;
            else
                printf("\b \b"); //the effect of backspace
        }
        input[j]='\0';
        strcpy(history[h], input);
        //ch = '\033';
        if( ch == '\033'){
            ggetch();
            ch=ggetch();
            switch(ch){
            case 'A':
                //printf("\n");
                //if (history[b] == NULL){
                //  history[b] == "";
                //}
                j=0;
                //printf ("\33[1B");
                printf("\33[2K\r"); // clear the screen
                if( b == 0 && h == 0){
                    j=0;
                    printf("");
                }else if( b == 0 ){
                    j=0;
                    b = h-1;
                }else{
                    b--;
                }
                printf("\r");
                showcwd();
                strcpy(input,history[b]);
                printf("%s", input);
                j = strlen(input);
                //k++;
                break;
            case 'B':
                j=0;
                //printf ("\33[1A");
                printf("\33[2K\r"); // clear the screen
                if( b == h){
                    printf("");
                    j=0;
                }
                if ( b > h){
                    b = h;
                    j=0;
                }else{
                    b++;
                }
                printf("\r");
                showcwd();
                strcpy(input, history[b]);
                printf("%s", input);
                j = strlen(input);
                break;
            //case 'C':
                //printf("\t");
                //break;
            //case 'D':
                //if((int k = (strlen(input))) == 0)
                //printf("\b");
                //break;
            }
        }
    }
    printf("\n");


    for(int i = 0; i<2; i++){
        str[i] = input[i];
    }
    int i = 0;
    char* string = strtok(input, " \n");
    while (string != NULL){
        words[i] = string;
        string = strtok(NULL, " \n");
        i++;
    }
    words[i] = NULL;
    if ( words[0] != NULL ){;
        h++;
    }
    //printf("%s\n",input);
}

void emptystr(){
    if (words[0] == NULL){
        words[0] = "s";
        a = 1;
    }
}
int ggetch(void){
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(0, &oldattr);
    newattr=oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( 0, TCSANOW, &newattr);
    ch=getchar();
    tcsetattr(0, TCSANOW, &oldattr);
    return(ch);
}

void wrongcmd(){
    if (a != 1) {
        printf("command not found: %s\n",words[0]);
    }
}

int main(){
    arraycolors[0] = 1; //basic color settings(will be changed in the future)
    arraycolors[1] = 2;
    arraycolors[2] = 7;
    arraycolors[3] = 0;
    chdir(getenv("HOME"));
    while(1) {
        signal(SIGINT, sigintHandler);

        showcwd();
        get_input();
        char* arg[] = {
                words[0],
                words[1],
                words[2],
                words[3],
                NULL
        };
        //printf("%s", history[h]);
        /*while(ggetch() == '\033'){
            ggetch();
            switch(ggetch()){
                case 'A':
                    if(h > 0){
                        printf("%s\n", history[h]);
                        h--;
                    }else{
                        printf("The history is empty");
                    }
                    printf("You pressed the up arrow key !!\n");
                    break;
                case 'B':
                    if(h < 10){
                        printf("%s\n", history[h]);
                        h++;
                    }else{
                        printf("The history is empty");
                    }
                    printf("You pressed the down arrow key !!\n");
                    break;
                case 'C':
                    printf("You pressed the right arrow key !!\n");
                    break;
                case 'D':
                    printf("You pressed the left arrow key !!\n");
                    break;
            }
        }
         */

        emptystr();

        if (strcmp(words[0], commands[3]) == 0) { //ccolor
            a = 1;
            if (words[5] != NULL) {
                printf("ccolor: Extra arguments\n");
            } else {
                if (words[1] == NULL) {
                    words[1] = "s";
                }
                if (words[2] == NULL) {
                    words[2] = "s";
                }
                if (words[3] == NULL) {
                    words[3] = "s";
                }
                if (words[4] == NULL) {
                    words[4] = "s";
                }

                emptystr();
                a = 1;
                printf("Write numbers of the colors in the order to change:\n"
                       "user: [path] pointer($) output\n"
                       "0 - reset\n"
                       "1 - green\n"
                       "2 - yellow\n"
                       "3 - blue\n"
                       "4 - magenta\n"
                       "5 - cyan\n"
                       "6 - white\n"
                       "7 - red\n"
                );
                arraycolors[0] = atoi(words[1]);
                arraycolors[1] = atoi(words[2]);
                arraycolors[2] = atoi(words[3]);
                arraycolors[3] = atoi(words[4]);
                //char* const tmp = getenv("USER");
                //printf("%s%s: %s[~%s]%s$%s \n",color[arraycolors[0]], tmp, color[arraycolors[1]], cwd, color[arraycolors[2]], color[arraycolors[0]]);

            }
        }//perror

        if (strcmp (words[0], commands[2]) == 0){                                   //HELP
            a = 1;
            printf("\n"
                   "*************  Bitches rule the world *****************\n"
                   "*******************************(c) Aliaksei Brown  ****\n"
                   "-ccolor\n"
                   "-cd\n"
                   "-ls\n"
                   "-pwd\n"
                   "-cp\n"
                   "-bash\n"
                   "-clear\n"
                   "-exit\n"
                   "-./\n"
                   "history\n"
                   "-help\n");
            // fflush(stdin);

        }

        if ((strcmp (words[0], commands[1]) == 0) && (words[1] != NULL)) {           //CWD /directory
            if (strcmp(words[1],"~") == 0){
                a = 1;
                chdir(getenv("HOME"));
            }
            else if (strcmp(words[1],"-") == 0){
                a = 1;
                if (chdir("..") !=0){
                    perror(words[0]);
                }
                getcwd(cwd, sizeof(cwd));
                printf("%s\n", cwd);

            }
            else if ((getcwd(cwd, sizeof(cwd))) != NULL){
                a = 1;
                if (chdir(words[1]) !=0){
                    //printf("%s \"%s\" :" ,words[0], words[1]);
                    perror(words[0]);
                }
            } else {
                printf("Not in directory");
            }
        }
        else if (strcmp (words[0], commands[1]) == 0){                                        //CWD
            if ((getcwd(cwd, sizeof(cwd))) != NULL){
                a = 1;
                chdir(getenv("HOME"));
            }else{
                perror("");
            }
            //fflush(stdin);
        } /*else {
                    printf("Not in directory");
                }*/





        if (strcmp (words[0], "pwd") == 0){                                      //PWD
            a = 1;

            int pid = fork();
            if (pid == 0){
                //Child process!
                if (execv("/bin/pwd", arg) == 0){
                    //printf("%s", words[1]);
                    exit(1);
                }else{
                    perror("");
                    _exit(1);
                }
            } else {
                wait(NULL);
            }
        }

        if (strcmp (words[0], "ls") == 0){                                //LS
            a = 1;

            int pid = fork();
            if (pid == 0){
                //Child process!
                if (execv("/bin/ls", arg) == 0){
                    //printf("%s", words[1]);
                    exit(1);
                }else{
                    perror("");
                    _exit(1);
                }
            } else {
                wait(NULL);
            }
        }
        if (strcmp (words[0], "bash") == 0){                                //bash
            a = 1;

            int pid = fork();
            if (pid == 0){
                //Child process!
                if (execv("/bin/bash", arg) == 0){
                    //printf("%s", words[1]);
                    exit(1);
                }else{
                    perror("");
                    _exit(1);
                }
            } else {
                wait(NULL);
            }
        }

        if (strcmp (words[0], "cp") == 0){                                 //cp
            a = 1;
            FILE *fp1, *fp2;
            //char cbuf[1024];
            int c;
            ssize_t count;
            fp1 = fopen(words[1], "r");
            if (fp1 == NULL){
                perror("");
            }
            fp2 = fopen(words[2], "w");
            while((c = fgetc(fp1)) != EOF){
                putc(c,fp2);
            }
            printf("The file%s was created.\n", words[2]);
            fclose(fp1);
            fclose(fp2);
        }

        if (strcmp (str, commands[4]) == 0){                                           // ./
            a = 1;
            int pid = fork();
            if (pid == 0){
                //Child process!
                if (execvp(words[0], arg) == 0){
                    //_exit(1);
                }else{
                    perror("");
                    //printf("Couldn't open the file: No such file\n");
                    _exit(0);
                }
            } else {
                wait(NULL);
            }
        }

        if (strcmp (words[0], commands[0]) == 0){                       //exit
            a = 1;
            // fflush(stdin);
            break;
        }

        if (strcmp (words[0], commands[5]) == 0){                       //clear
            a = 1;
            printf("\e[1;1H\e[2J");
        }

        wrongcmd();
        a = 0;
        //printf("%d", h);
        //printf("The end of the while process\n");
    }
}
