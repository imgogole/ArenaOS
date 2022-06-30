#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "gamemanager.h"

#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

Node* CurrentDirectory;
Node* RootDirectory;
char* CurrentDirectoryName;
char* UserName;
char* ReadLine(void);
char **SplitLine(char *line);
int ExecuteCommand(char **args);

int main()
{
    system("clear");

    LoopGame();

    return EXIT_SUCCESS;
}

/*
Function Declarations for builtin shell commands:
*/
int ArenaOS_help(char **args);
int ArenaOS_exit(char **args);
int ArenaOS_cd(char **args);
int ArenaOS_tree(char **args);
int ArenaOS_ls(char **args);

/*
List of builtin commands, followed by their corresponding functions.
*/
char *builtin_str[] =
{
    "help",
    "exit",
    "cd",
    "tree",
    "ls"
};

int (*builtin_func[]) (char **) =
{
    &ArenaOS_help,
    &ArenaOS_exit,
    &ArenaOS_cd,
    &ArenaOS_tree,
    &ArenaOS_ls
};

int lsh_num_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

/*
Builtin function implementations.
*/

int ArenaOS_help(char **args)
{
    int i;
    printf("The following are built in:\n");

    for (i = 0; i < lsh_num_builtins(); i++)
    {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int ArenaOS_exit(char **args)
{
    return 0;
}

int ArenaOS_cd(char **args)
{
    Node* current = CurrentDirectory;

    if (args[1] == NULL)
    {
        CurrentDirectory = RootDirectory;
    }
    else if (strcmp(args[0], "..") == 0)
    {
        CurrentDirectory = GetParent(CurrentDirectory);
    }
    else
    {
        LinkedList_NodeChain* item = GetFirstChild(current);
        while (item != NULL)
        {
            if (strcmp(GetNameLLNC(item), args[1]) == 0)
            {
                CurrentDirectory = GetNodeValue(item);
                return 1;
            }
            item = GetNext(item);
        }

        fprintf(stderr, "ArenaOS: No such file of directory.\n");
        return 1;
    }

    return 1;
}

int ArenaOS_tree(char **args)
{
    ShowTree(CurrentDirectory, 0);
    return 1;
}

int ArenaOS_ls(char **args)
{
    Node* current = CurrentDirectory;

    LinkedList_NodeChain* item = GetFirstChild(current);
    while (item != NULL)
    {
        printf("%s      ", GetNameLLNC(item));
        item = GetNext(item);
    }

    printf("\n");

    return 1;
}

// Loop Game

void LoopGame(void)
{
    Node* n1;
    Node* n2;
    Node* n3;
    Node* n4;
    Node* n5;
    Node* n6;
    Node* n7;
    Node* n8;
    Node* n9;
    Node* n10;
    Node* n11;

    n1 = CreateNode(NULL, NULL, "root");
    n3 = CreateNode(NULL, n1, "lib");
    n2 = CreateNode(NULL, n1, "bin");
    n4 = CreateNode(NULL, n2, "minecraft");
    n5 = CreateNode(NULL, n2, "blockbench");
    n6 = CreateNode(NULL, n2, "tilesII");
    n7 = CreateNode(NULL, n5, "project");
    n8 = CreateNode(NULL, n7, "collections");
    n9 = CreateNode(NULL, n7, "textures");
    n10 = CreateNode(NULL, n1, "locales");
    n11 = CreateNode(NULL, n6, "Records.txt");

    RootDirectory = n1;
    CurrentDirectory = n2;
    UserName = "imgogole";

    char *line;
    char **args;
    int status;


    do
    {
        CurrentDirectoryName = GetName(CurrentDirectory);

        printf("[%s@%s]$ ", UserName, CurrentDirectoryName);
        line = ReadLine();
        args = SplitLine(line);
        status = ExecuteCommand(args);

        free(line);
        free(args);
    }
    while (status);

    free(n1);
    free(n2);
    free(n3);
    free(n4);
    free(n5);
    free(n6);
    free(n7);
    free(n8);
    free(n9);
}

char* ReadLine(void)
{
    char *line = NULL;
    size_t bufsize = 0;

    if (getline(&line, &bufsize, stdin) == -1)
    {
        if (feof(stdin))
        {
            exit(EXIT_SUCCESS);
        }
        else
        {
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }

    return line;
}

char **SplitLine(char *line)
{
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "ArenaOS: Allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens)
            {
                fprintf(stderr, "ArenaOS: Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, LSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int Launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            perror("ArenaOS");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("ArenaOS");
    }
    else
    {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        }
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}


int ExecuteCommand(char **args)
{
    int i;

    if (args[0] == NULL)
    {
        return 1;
    }

    for (i = 0; i < lsh_num_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0) 
        {
            return (*builtin_func[i])(args);
        }
    }

    fprintf(stderr, "ArenaOS: No command match with your request.\n");
    return EXIT_FAILURE;


    return Launch(args);
}

