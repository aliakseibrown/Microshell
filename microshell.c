#include <stdio.h>
#include <stdbool.h> //for while(true)
#include <string.h>
#include <unistd.h> //for getcwd, chdir fork
#include <stdlib.h>
#include <sys/wait.h> //wait

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"


char input[1024];
char* words[1024];
char cwd[1024];
int a = 0;
char str[20];
char green[10]= "\x1B[32m";
//char user_name[50];
char user_name[] = "alexeybrown";

const char commands[5][20] =
        {"exit",
         "cd",
         "help",
         "..",
         "./"
        };

void showcwd(){
    getcwd(cwd, sizeof(cwd));
    printf("%s%s:" YEL " [~%s]" RED "$ ",green, user_name, cwd);

}

void get_input() {
    int i = 0;
    fgets(input, 1024, stdin);
    for(int i = 0; i<2; i++){
        str[i] = input[i];
    }
    char* string = strtok(input, " \n");
    while (string != NULL){
        words[i] = string;
        string = strtok(NULL, " \n");
        i++;
    }
    words[i] = NULL;

}

void emptystr(){
    if (words[0] == NULL){
        words[0] = "s";
        a = 1;
    }
}


void wrongcmd(){
    if (a != 1) {
        printf("command not found: %s\n",words[0]);
    }
}

int main () {
    chdir(getenv("HOME"));
    while(1) {
        showcwd();
        get_input();
        emptystr();
        printf("%s", getenv("USER"));

        if (strcmp (words[0], commands[2]) == 0){
            a = 1;
            printf("\n*******************************************************\n"
                   "*******************************************************\n"
                   "*************  Bitches rule the world *****************\n"
                   "*******************************(c) Aliaksei Brown  ****\n"
                   "*******************************************************\n\n"
                   "-cd\n"
                   "-exit\n"
                   "-help\n");
            // fflush(stdin);

        }

        if ((strcmp (words[0], commands[1]) == 0) && (words[1] != NULL)) {
            if ((getcwd(cwd, sizeof(cwd))) != NULL){
                a = 1;
                if (chdir(words[1]) !=0){
                    //printf("%s \"%s\" :" ,words[0], words[1]);
                    perror(words[0]);
                }
            } else {
                printf("Not in directory");
            }
        }
        else if (strcmp (words[0], commands[1]) == 0){
            if ((getcwd(cwd, sizeof(cwd))) != NULL){
                a = 1;
                chdir(getenv("HOME"));
            }
            //fflush(stdin);
        } /*else {
                    printf("Not in directory");
                }*/

        if (strcmp (str, "./") == 0){
            a = 1;
            int pid = fork();
            if (pid == 0){
                //Child process!
                if (execvp(words[0], NULL) == 0){
                    _exit(1);
                }else{
                    //perror("");
                    _exit(1);
                }
            } else {
                wait(NULL);
            }
        }
        if (strcmp (words[0], commands[0]) == 0){
            a = 1;
            // fflush(stdin);
            break;
        }
        wrongcmd();
        a = 0;
        //printf("The end of the while proccess\n");
    }
}

