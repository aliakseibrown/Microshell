#include <sys/wait.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#define MAX 1024
#define HND 100
#define MIN 30

char arrayHistoryCommands[HND][MAX];
char arrayFirstTwoCharacters[MIN];
char *arrayCommands[MAX];
char arrayDirectory[MAX];
char arrayInput[MAX];
char letters;

int counterHistoryActive;
int counterHistoryTotal;
int arrayColors[5];
int counterLetters;
int counterErrors;

const char color[8][MIN] =
        {"\x1B[0m",  // reset 0
         "\x1B[32m", // green 1
         "\x1B[33m", // yellow 2
         "\x1B[34m", // blue 3
         "\x1B[35m", // magenta 4
         "\x1B[36m", // cyan 5
         "\x1B[37m", // white 6
         "\x1B[31m"  // red 7
        };


static void sigintHandler();
int functionGetch();

void getCurrentDirection();
void settingsHomeColors();
void inputFromKeyboard();
void changeEmptyString();
void errorNotFound();
void keyArrows();

void commandPrintWorkingDirection(char **arg);
void commandMakeDirectory(char **arg);
void commandCopyingFiles(char **arg);
void commandListOfFiles(char **arg);
void commandScripts(char **arg);
void commandBash(char **arg);
void commandChangeDirectory();
void commandChangeColor();
void commandClear();
void commandHelp();

int main(){
    settingsHomeColors();
    while(1) {
        signal(SIGINT, sigintHandler);

        getCurrentDirection();
        inputFromKeyboard();

        char *arg[] = { //arguments for fork
                arrayCommands[0],
                arrayCommands[1],
                arrayCommands[2],
                arrayCommands[3],
                NULL
        };

        changeEmptyString();

        commandPrintWorkingDirection(arg);
        commandMakeDirectory(arg);
        commandChangeDirectory();
        commandCopyingFiles(arg);
        commandListOfFiles(arg);
        commandChangeColor();
        commandScripts(arg);
        commandBash(arg);
        commandClear();
        commandHelp();

        if (strcmp (arrayCommands[0], "exit") == 0){
            counterErrors = 1;
            break;
        }

        errorNotFound();
        counterErrors = 0;
    }
}




static void sigintHandler(int sig){
    write(STDERR_FILENO, " Caught SIGINT!\n", 15);
}

void getCurrentDirection(){
    getcwd(arrayDirectory, sizeof(arrayDirectory));
    char *const tmp = getenv("USER");
    printf("%s%s: %s[~%s]%s$%s ",
           color[arrayColors[0]], tmp,
           color[arrayColors[1]], arrayDirectory,
           color[arrayColors[2]],
           color[arrayColors[3]]);
}

void inputFromKeyboard() {
    printf("\b \b");
    printf(" ");
    counterHistoryActive = counterHistoryTotal;
    memset(arrayInput, 0, MAX);
    for(counterLetters = 0; (letters = functionGetch()) != '\n';){
        if(letters != 127 ){
            arrayInput[counterLetters]=letters;
            printf("%c",arrayInput[counterLetters]);
            counterLetters++;
            //input[counterLetters]=NULL;
        }else{
            counterLetters--;
            if(counterLetters < 0)
                counterLetters++;
            else
                printf("\b \b"); //the effect of backspace
        }
        arrayInput[counterLetters] = '\0';
        strcpy(arrayHistoryCommands[counterHistoryTotal], arrayInput);
       keyArrows();
    }
    printf("\n");

    for(int i = 0; i<2; i++){
        arrayFirstTwoCharacters[i] = arrayInput[i];
    }
    int i = 0;
    char delim = '\"';

    if(strchr(arrayInput, delim ) == 0){
        char *string = strtok(arrayInput, " \n");
        while (string != NULL){
            arrayCommands[i] = string;
            string = strtok(NULL, " \n");
            i++;
        }
        arrayCommands[i] = NULL;
        if (arrayCommands[0] != NULL ){;
            counterHistoryTotal++;
        }
    }else{
        char *string = strtok(arrayInput, " \n");
        while (string != NULL){
            arrayCommands[i] = string;
            string = strtok(NULL, "\"\n");
            i++;
        }
        arrayCommands[i] = NULL;
        if (arrayCommands[0] != NULL ){;
            counterHistoryTotal++;
        }
    }
}

void changeEmptyString(){
    if (arrayCommands[0] == NULL){
        arrayCommands[0] = "s";
        counterErrors = 1;
    }
}

int functionGetch(void){
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

void errorNotFound(){
    if (counterErrors != 1) {
        printf("command not found: %s\n", arrayCommands[0]);
    }
}

void commandChangeColor(){
    if (strcmp(arrayCommands[0], "ccolor") == 0) {
        counterErrors = 1;
        if (arrayCommands[5] != NULL) {
            printf("ccolor: Extra arguments\n");
        } else {
            if (arrayCommands[1] == NULL) {
                arrayCommands[1] = "s";
            }
            if (arrayCommands[2] == NULL) {
                arrayCommands[2] = "s";
            }
            if (arrayCommands[3] == NULL) {
                arrayCommands[3] = "s";
            }
            if (arrayCommands[4] == NULL) {
                arrayCommands[4] = "s";
            }

            changeEmptyString();
            counterErrors = 1;
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
            arrayColors[0] = atoi(arrayCommands[1]);
            arrayColors[1] = atoi(arrayCommands[2]);
            arrayColors[2] = atoi(arrayCommands[3]);
            arrayColors[3] = atoi(arrayCommands[4]);
        }
    }
}

void commandHelp(){
    if (strcmp (arrayCommands[0], "help") == 0){
        counterErrors = 1;
        printf("\n"
               "********  Bitches rule the world ***********\n"
               "******************(c) Aliaksei Brown  ******\n"
               "-history\n"
               "-ccolor\n"
               "-mkdir\n"
               "-clear\n"
               "-help\n"
               "-bash\n"
               "-exit\n"
               "-pwd\n"
               "-cp\n"
               "-cd\n"
               "-ls\n"
               "-./\n");
    }
}

void commandChangeDirectory(){
    if ((strcmp (arrayCommands[0], "cd") == 0) && (arrayCommands[1] != NULL)) {
        if (strcmp(arrayCommands[1], "~") == 0){
            counterErrors = 1;
            chdir(getenv("HOME"));
        }
        else if (strcmp(arrayCommands[1], "-") == 0){
            counterErrors = 1;
            if (chdir("..") != 0){
                perror(arrayCommands[0]);
            }
            getcwd(arrayDirectory, sizeof(arrayDirectory));
            printf("%s\n", arrayDirectory);

        }
        else if ((getcwd(arrayDirectory, sizeof(arrayDirectory))) != NULL){
            counterErrors = 1;
            if (chdir(arrayCommands[1]) != 0){
                perror(arrayCommands[0]);
            }
        } else {
            printf("Not in directory");
        }
    }
    else if (strcmp (arrayCommands[0], "cd") == 0){
        if ((getcwd(arrayDirectory, sizeof(arrayDirectory))) != NULL){
            counterErrors = 1;
            chdir(getenv("HOME"));
        }else{
            perror("");
        }
    }
}
void settingsHomeColors(){
    chdir(getenv("HOME"));
    arrayColors[0] = 1; //basic color settings(will be changed in the future by command)
    arrayColors[1] = 2;
    arrayColors[2] = 7;
    arrayColors[3] = 0;
}
void commandPrintWorkingDirection(char **arg){
    if (strcmp (arrayCommands[0], "pwd") == 0){
        counterErrors = 1;

        int pid = fork();
        if (pid == 0){
            if (execv("/bin/pwd", arg) == 0){
                exit(1);
            }else{
                perror("");
                _exit(1);
            }
        } else {
            wait(NULL);
        }
    }
}

void commandListOfFiles(char **arg){
    if (strcmp (arrayCommands[0], "ls") == 0){
        counterErrors = 1;

        int pid = fork();
        if (pid == 0){
            if (execv("/bin/ls", arg) == 0){
                exit(1);
            }else{
                perror("");
                _exit(1);
            }
        } else {
            wait(NULL);
        }
    }
}

void commandBash(char **arg){
    if (strcmp (arrayCommands[0], "bash") == 0){
        counterErrors = 1;

        int pid = fork();
        if (pid == 0){
            if (execv("/bin/bash", arg) == 0){
                exit(1);
            }else{
                perror("");
                _exit(1);
            }
        } else {
            wait(NULL);
        }
    }
}

void commandCopyingFiles(char **arg){
    if (strcmp (arrayCommands[0], "cp") == 0){
        counterErrors = 1;
        FILE *fp1, *fp2;
        int c;
        ssize_t count;
        fp1 = fopen(arrayCommands[1], "r");
        if (fp1 == NULL){
            perror("");
        }
        fp2 = fopen(arrayCommands[2], "w");
        while((c = fgetc(fp1)) != EOF){
            putc(c,fp2);
        }
        printf("The file%s was created.\n", arrayCommands[2]);
        fclose(fp1);
        fclose(fp2);
    }
}
void commandScripts(char **arg) {
    if (strcmp (arrayFirstTwoCharacters, "./") == 0){
        counterErrors = 1;
        int pid = fork();
        if (pid == 0){
            if (execvp(arrayCommands[0], arg) == 0){
            }else{
                perror("");
                _exit(0);
            }
        } else {
            wait(NULL);
        }
    }
}

void commandClear(){
    if (strcmp (arrayCommands[0], "clear") == 0){
        counterErrors = 1;
        printf("\e[1;1H\e[2J");
    }
}

void keyArrows(){
    if(letters == '\033'){
        functionGetch();
        letters= functionGetch();
        switch(letters){
            case 'A':
                //printf("A ARROW");
                counterLetters=0;
                printf("\33[2K\r"); // clear the screen
                if(counterHistoryActive == 0 && counterHistoryTotal == 0){
                    counterLetters=0;
                    printf("");
                }else if(counterHistoryActive == 0 ){
                    counterLetters=0;
                    counterHistoryActive = counterHistoryTotal - 1;
                }else{
                    counterHistoryActive--;
                }
                printf("\r");
                getCurrentDirection();
                strcpy(arrayInput, arrayHistoryCommands[counterHistoryActive]);
                printf("%s", arrayInput);
                counterLetters = strlen(arrayInput);
                //k++;
                break;
            case 'B':
                counterLetters=0;
                printf("\33[2K\r"); // clear the screen
                if(counterHistoryActive == counterHistoryTotal){
                    printf("");
                    counterLetters=0;
                }
                if (counterHistoryActive > counterHistoryTotal){
                    counterHistoryActive = counterHistoryTotal;
                    counterLetters=0;
                }else{
                    counterHistoryActive++;
                }
                printf("\r");
                getCurrentDirection();
                strcpy(arrayInput, arrayHistoryCommands[counterHistoryActive]);
                printf("%s", arrayInput);
                counterLetters = strlen(arrayInput);
                break;
                //case 'C':
                //printf("\t");
                //break;
                //case 'D':
                //printf("\counterHistoryActive");
                //break;
        }
    }
}

void commandMakeDirectory(char **arg){
    if (strcmp (arrayCommands[0], "mkdir") == 0){
        counterErrors = 1;

        int pid = fork();
        if (pid == 0){
            if (execv("/bin/mkdir", arg) == 0){
                exit(1);
            }else{
                perror("");
                _exit(1);
            }
        } else {
            wait(NULL);
        }
    }
}