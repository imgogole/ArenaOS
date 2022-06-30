#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"
#include <string.h>
#include <sys/ucontext.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "gamemanager.h"

#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

/*-----Node types----*/

#define FOLDER_TYPE 0
#define FILE_TYPE 1

/*-------------------*/

/*-----Check OS------*/

#ifdef _WIN32
#define OS_INDEX 0
#elifdef __linux__
#define OS_INDEX 1
#endif

/*-------------------*/

Node* CurrentDirectory;
char* UserName;
Node* RootDirectory;
char* CurrentDirectoryName;
char* ReadLine(void);
char **SplitLine(char *line);
int ExecuteCommand(char **args);

int main()
{
    Clear(OS_INDEX);

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
int ArenaOS_cat(char **args);

/*
List of builtin commands, followed by their corresponding functions.
*/
char *builtin_str[] =
{
    "help",
    "exit",
    "cd",
    "tree",
    "ls",
    "cat"
};

int (*builtin_func[]) (char **) =
{
    &ArenaOS_help,
    &ArenaOS_exit,
    &ArenaOS_cd,
    &ArenaOS_tree,
    &ArenaOS_ls,
    &ArenaOS_cat
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
    else if (strcmp(args[1], "..") == 0)
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
                Node* node = GetNodeValue(item);
                if (GetDataType(GetData(node)) == FOLDER_TYPE) CurrentDirectory = GetNodeValue(item);
                else fprintf(stderr, "ArenaOS: %s is not a folder.\n", GetNameLLNC(item));
                return 1;
            }
            item = GetNext(item);
        }

        fprintf(stderr, "ArenaOS: No such file of directory.\n");
        return 1;
    }

    return 1;
}

int ArenaOS_cat(char **args)
{
    Node* current = CurrentDirectory;

    if (args[1] == NULL)
    {
        fprintf(stderr, "ArenaOS: Please specify a file name.\n");
    }
    else
    {
        LinkedList_NodeChain* item = GetFirstChild(current);
        while (item != NULL)
        {
            if (strcmp(GetNameLLNC(item), args[1]) == 0)
            {
                Node* node = GetNodeValue(item);
                if (GetDataType(GetData(node)) == FILE_TYPE) fprintf(stderr, "%s\n", GetDataContent(GetData(node)));
                else fprintf(stderr, "ArenaOS: %s is a folder.\n", GetNameLLNC(item));
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
    ListDirectory(CurrentDirectory);
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
    Node* n12;
    Node* n13;
    Node* n14;
    Node* n15;
    Node* n16;
    Node* n17;

    FileData* FolderType = NewFileData(NULL);
    FileData* FileType = NewFileData(NULL);
    SetFileData(FolderType, FOLDER_TYPE, 0);
    SetFileData(FileType, FILE_TYPE, "Hello world !");

    n1 = CreateNode(NewFileData(FolderType), NULL, "root");
    n3 = CreateNode(NewFileData(FolderType), n1, "lib");
    n2 = CreateNode(NewFileData(FolderType), n1, "bin");
    n4 = CreateNode(NewFileData(FolderType), n2, "minecraft");
    n5 = CreateNode(NewFileData(FolderType), n2, "blockbench");
    n6 = CreateNode(NewFileData(FolderType), n2, "tilesII");
    n7 = CreateNode(NewFileData(FolderType), n5, "project");
    n8 = CreateNode(NewFileData(FolderType), n7, "collections");
    n9 = CreateNode(NewFileData(FolderType), n7, "textures");
    n10 = CreateNode(NewFileData(FileType), n6, "Records");
    n11 = CreateNode(NewFileData(FileType), n6, "Logs");
    n12 = CreateNode(NewFileData(FolderType), n6, "Assets");
    n13 = CreateNode(NewFileData(FolderType), n12, "Scripts");
    n14 = CreateNode(NewFileData(FileType), n12, "Missile");
    n15 = CreateNode(NewFileData(FileType), n13, "Main");
    n16 = CreateNode(NewFileData(FileType), n13, "Database");
    n17 = CreateNode(NewFileData(FileType), n13, "Photon");


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

    free(FolderType);
    free(FileType);
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

