/*
project: 01
author: Jay Buckwalter
email: rf29850@umbc.edu
student id: rf29850
date: 3/10/24
description: a simple linux shell designed to perform basic linux commands
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include "utils.h"

/*
In this project, you are going to implement a number of functions to 
create a simple linux shell interface to perform basic linux commands
*/

//DEFINE THE FUNCTION PROTOTYPES
int user_prompt_loop();
char* get_user_command();
char** parse_command(char* input);
int execute_command(char** command);
int execute_proc(char** command);
int readHistory(char* fileName);
int writeHistory(char* fileName, int len, char* str);

const int INIT_BUFFER_LEN = 16;
const int MAX_HISTORY = 10;

int main(int argc, char **argv) {
    if(argc > 1) {
        fprintf(stderr, "Too many arguments\n");
        return 1;
    } else {
        return user_prompt_loop();
    }
}

/*
user_prompt_loop():
Get the user input using a loop until the user exits, prompting the user for a command.
Gets command and sends it to a parser, then compares the first element to the two
different commands ("/proc", and "exit"). If it's none of the commands, 
send it to the execute_command() function. If the user decides to exit, then exit 0 or exit 
with the user given value. 
*/

int user_prompt_loop() {
    int historySize = 0;
    int exitCode = INT_MIN;
    char* historyFileName = strcat(getenv("HOME"), "/.421sh");
    char* input = NULL;
    char** tokens = NULL;

    // Clear previous history file, if exists
    FILE* history = fopen(historyFileName, "w");
    if(history == NULL) fprintf(stderr, "fopen error: %s\n", strerror(errno));
    fclose(history);

    while(exitCode == INT_MIN) {
        input = get_user_command();
        
        // Crash if memory allocation failed
        if(input == NULL) {
            free(input);
            return errno;
        }

        // Parse input
        tokens = parse_command(input);

        // Failed to unescape or nothing entered, get another input
        if(tokens == (char**) 1 || tokens[0] == NULL) {
            free(input);
            continue;
        // Crash if memory allocation failed
        } else if(tokens == NULL) {
            return errno;
        // Built in proc command
        } else if(!strcmp(tokens[0], "/proc")) {
            // Returns -1 in case of a malloc fail
            if(execute_proc(tokens) == -1) {
                goto deallocAndExit;
            }
        // Built in exit command
        } else if(!strcmp(tokens[0], "exit")) {
            if(tokens[1] == NULL) {
                exitCode = 0;
            } else if(isdigit(tokens[1][0])) {
                exitCode = atoi(tokens[1]);
            } else {
                fprintf(stderr, "Invalid exit code");
            }
        // Built in history command
        } else if(!strcmp(tokens[0], "history")) {
            readHistory(historyFileName);
        // Execute custom command
        } else {
            // Returns -1 if returning as child
            if(execute_command(tokens) == -1) {
                goto deallocAndExit;
            }
        }

        // Add input to history
        historySize = writeHistory(historyFileName, historySize, input);

        // Free used memory
        for(int ii = 0; tokens[ii] != NULL; ii++) {
            free(tokens[ii]);
        }
        free(tokens);
        free(input);
    }

    return exitCode;

    deallocAndExit:
    for(int ii = 0; tokens[ii] != NULL; ii++) {
        free(tokens[ii]);
    }
    free(tokens);
    free(input);
    return errno;
}

/*
get_user_command():
Take input of arbitrary size from the user and return to the user_prompt_loop()
*/
char* get_user_command() {
    int inputSize = INIT_BUFFER_LEN;
    char* input = NULL;
    int charsRead = 0;

    // Allocate memory for reading input
    if(!(input = (char*) malloc(inputSize * sizeof(char)))) {
        goto mallocError;
    }
    
    // Show prompt character
    printf("$ ");

    // Get characters, 1 at a time
    for(char nextChar = fgetc(stdin);
            nextChar != '\0' && nextChar != '\n' && nextChar != EOF;
            nextChar = fgetc(stdin)) {
        // If buffer is too small, double it
        if(charsRead + 1 >= inputSize) {
            inputSize *= 2;
            if(!(input = (char*) realloc(input, inputSize * sizeof(char)))) {
                goto mallocError;
            }
        }
        // Add character into buffer
        input[charsRead++] = nextChar;
    }

    // Add null character at end
    input[charsRead] = '\0';

    return input;

    // Print error and crash if malloc fails
    mallocError:
    fprintf(stderr, "shell error: %s\n", strerror(errno));
    free(input);
    return NULL;
}

/*
parse_command(char*):
Take command grabbed from the user and parse appropriately.
Example: 
    user input: "ls -la"
    parsed output: ["ls", "-la", NULL]
Example: 
    user input: "echo     hello                     world  "
    parsed output: ["echo", "hello", "world", NULL]
*/
char** parse_command(char* input) {
    int tokenStart = 0;
    int tokenCount = 1;
    char** parsed = NULL;
    input = unescape(input, stderr);

    // Could not unescape, invalid input
    if(input == NULL) {
        return (char**) 1;
    }

    // Allocate memory for parsed input
    if(!(parsed = (char**) malloc(sizeof(char*)))) {
        goto mallocError;
    }

    // First token may or may not have space in front of it
    while(isspace(input[tokenStart])) {
        tokenStart++;
    }

    // Loop through input
    for(int nextSpace = first_unquoted_space(input + tokenStart);
            nextSpace != -1;
            nextSpace = first_unquoted_space(input + tokenStart)) {
        nextSpace += tokenStart;

        // Allocate memory for new token pointer
        if(!(parsed = (char**) realloc(parsed, ++tokenCount * sizeof(char*)))) {
            goto mallocError;
        }
        // Allocate memory for new token
        if(!(parsed[tokenCount - 2] = (char*) malloc((nextSpace - tokenStart + 1) * sizeof(char)))) {
            goto mallocError;
        }

        // Loop through token, adding it to parsed
        int tokenIter = tokenStart;
        while(tokenIter < nextSpace) {
            // Add character into token
            parsed[tokenCount - 2][tokenIter - tokenStart] = input[tokenIter];
            tokenIter++;
        }

        // Add null character at end of token
        parsed[tokenCount - 2][tokenIter - tokenStart] = '\0';

        // Advance tokenStart through white space
        tokenStart = nextSpace;
        while(isspace(input[tokenStart])) {
            tokenStart++;
        }
    }

    // Last token may or may not have space behind it, if it does it's already been added
    if(input[tokenStart] != '\0') {
        // Allocate memory for new token pointer
        if(!(parsed = (char**) realloc(parsed, ++tokenCount * sizeof(char*)))) {
            goto mallocError;
        }
        // Allocate memory for new token
        if(!(parsed[tokenCount - 2] = (char*) malloc((strlen(input + tokenStart) + 1) * sizeof(char)))) {
            goto mallocError;
        }

        // Loop through token, adding it to parsed
        int tokenIter = tokenStart;
        while(input[tokenIter] != '\0') {
            // Add character into token
            parsed[tokenCount - 2][tokenIter - tokenStart] = input[tokenIter];
            tokenIter++;
        }
        // Add null character at end of token
        parsed[tokenCount - 2][tokenIter - tokenStart] = '\0';
    }

    // Add on NULL token to tail
    parsed[tokenCount - 1] = NULL;
    
    // Must free the memory allocated by unescape()
    free(input);

    return parsed;

    // Print error and crash if malloc fails
    mallocError:
    fprintf(stderr, "shell error: %s\n", strerror(errno));
    for(int ii = 0; ii < tokenCount - 1; ii++) free(parsed[ii]);
    free(input);
    free(parsed);
    return NULL;
}

/*
execute_command():
Execute the parsed command if the commands are neither /proc nor exit;
fork a process and execute the parsed command inside the child process
*/
int execute_command(char** command) {
    int child = fork();

    // Failed to fork
    if(child < 0) {
        fprintf(stderr, "shell error: fork fail\n");
    // In parent process
    } else if(child > 0) {
        while(!waitpid(child, NULL, WNOHANG)) {}
    // In child process
    } else {
        execvp(command[0], command);
        // If this line is reached, the exec failed
        fprintf(stderr, "execvp error: %s\n", strerror(errno));
        return -1;
    }

    return 0;
}

/*
execute_proc():
Execute the /proc command
Open the /proc file system to read from it
*/
int execute_proc(char** command) {
    // First check for a valid proc command
    if(command[1] == NULL) {
        fprintf(stderr, "Too few arguments [need 2]\n");
        return 1;
    } else if(command[2] != NULL) {
        fprintf(stderr, "Too many arguments [need 2]\n");
        return 1;
    } else if(command[1][0] != '/' || command[1][1] == '/') {
        fprintf(stderr, "Invalid proc file\n");
        return 1;
    }

    // Check second argument for valid proc
    int argLen = strlen(command[1]);
    int argIter = 1;

    // For cases where process id needed, find the index of the substring after that number
    while(argIter < argLen && command[1][argIter] >= '0' && command[1][argIter] <= '9') argIter++;

    // Check for one of the valid proc files
    if(strcmp(command[1], "/cpuinfo")
            && strcmp(command[1], "/loadavg")
            && strcmp(command[1], "/uptime")
            && strcmp(&(command[1][argIter]), "/status")
            && strcmp(&(command[1][argIter]), "/environ")
            && strcmp(&(command[1][argIter]), "/sched")) {
        fprintf(stderr, "Invalid proc file\n");
        return 1;
    }

    // Need to add memory to hold the file name in a single string
    if (!(command[0] = (char*) realloc(command[0], (strlen(command[0]) + strlen(command[1]) + 1) * sizeof(char)))) {
        fprintf(stderr, "shell error: %s\n", strerror(errno));
        return -1;
    }

    // Combine file name tokens
    strcat(command[0], command[1]);

    // Attempt to open the file
    FILE* file = fopen(command[0], "r");
    if(file == NULL) {
        fprintf(stderr, "shell error: %s\n", strerror(errno));
    } else {
        // Read each character in the file to the screen
        for(char nextChar = fgetc(file); nextChar != EOF; nextChar = fgetc(file)) printf("%c", nextChar);
    }
    fclose(file);

    return 0;
}

/*
readHistory(char*):
Opens the history file and displays it to the user
*/
int readHistory(char* fileName) {
    FILE* history = fopen(fileName, "r");

    // Check that the file opened
    if(history == NULL) {
        fprintf(stderr, "fopen error: %s\n", strerror(errno));
        return errno;
    }

    // Write all characters to terminal
    for(char nextChar = getc(history); nextChar != EOF; nextChar = getc(history)) {
        printf("%c", nextChar);
    }

    fclose(history);
    return 0;
}

/*
execute_proc():
Opens the history file and adds a new line to it
Or removes the oldest line and adds a new line
Depending on if len < MAX_HISTORY
*/
int writeHistory(char* fileName, int len, char* str) {
    // If history isn't full can just add new line to end of file
    if(len < MAX_HISTORY) {
        FILE* history = fopen(fileName, "a");

        // Check that the file opened
        if(history == NULL) {
            fprintf(stderr, "fopen error: %s\n", strerror(errno));
            return len;
        }

        // Add string to end of file
        fputs(str, history);
        fputc('\n', history);
        fclose(history);
        return len + 1;
    }

    // Otherwise, need to remove the oldest line
    FILE* history = fopen(fileName, "r");

    // Check that the file opened
    if(history == NULL) {
        fprintf(stderr, "fopen error: %s\n", strerror(errno));
        return len;
    }

    // Allocate memory to hold file contents
    char* temp = NULL;
    fseek(history, 0, SEEK_END);
    if(!(temp = (char*) malloc(ftell(history) * sizeof(char)))) {
        fclose(history);
        return len;
    }
    temp[0] = '\0';

    // Skip the first line
    fseek(history, 0, SEEK_SET);
    for(char nextChar = getc(history); nextChar != '\n'; nextChar = getc(history)) {}

    // Copy history to temp
    int ii = 0;
    while(!feof(history)) {
        temp[ii++] = fgetc(history);
    }
    temp[ii] = '\0';

    // Clear history file
    fclose(history);
    history = fopen(fileName, "w");

    // Check that the file opened
    if(history == NULL) {
        fprintf(stderr, "fopen error: %s\n", strerror(errno));
        free(temp);
        return len;
    }

    // Copy temp back to history
    fputs(temp, history);

    // Add new string on to end of history
    fseek(history, -1, SEEK_END);
    fputs(str, history);
    fputc('\n', history);

    // Cleanup
    fclose(history);
    free(temp);

    return len;
}