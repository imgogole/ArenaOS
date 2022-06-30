#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities.h"

#define MIDDLE_PATH "├──"
#define END_PATH "└──"

// Add colors 

void ResetColor(int IsBold)
{
    if (IsBold) printf("\033[1;37m");
    else printf("\033[0;37m");
}

void Purple(int IsBold)
{
    if (IsBold) printf("\033[1;35m");
    else printf("\033[0;35m");
}

// Utilities

int StartsWith(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

struct LinkedList_NodeChain
{
    Node* Value;
    LinkedList_NodeChain* Next;
};

struct FileData
{
    int Type;
    char* Data;
};

typedef struct Node
{
    FileData* Data;
    char Name[256];
    LinkedList_NodeChain* Child;
    Node* Parent;
} Node;

int GetListCount(LinkedList_NodeChain* list)
{
    int Count = 1;
    LinkedList_NodeChain* temp = list;
    while (temp != NULL)
    {
        temp = temp -> Next;
        Count++;
    }
    printf("Count of list : %d\n", Count);
    return Count;
}

int GetChildrenCount(Node* node)
{
    if (node == NULL) return 0;
    return GetListCount(node -> Child);
}

Node* GetChild(LinkedList_NodeChain* parent, int index)
{
    if (index >= GetListCount(parent)) return NULL;
    LinkedList_NodeChain* target = parent;
    int location = 0;
    while (location != index)
    {
        target = target -> Next;
        location++;
    }
    return target -> Value;
}

void LookAtChildrenOf(Node* node)
{
    if (node -> Child != NULL)
    {
        LinkedList_NodeChain* Child = node -> Child;
        while (Child != NULL)
        {
            printf("%s -> ", Child -> Value -> Name);
            Child = Child -> Next;
        }
    }

    printf("NULL\n");
}

LinkedList_NodeChain* CreateChild(Node* child)
{
    LinkedList_NodeChain* list = malloc(sizeof(LinkedList_NodeChain));
    if (list != NULL)
    {
        list -> Value = child;
    }
    return list;
}

void InsertNode(Node* parent, Node* node)
{
    LinkedList_NodeChain* temp = malloc(sizeof(LinkedList_NodeChain));
    if (temp != NULL)
    {
        temp -> Value = node;
        temp -> Next = parent -> Child;
        parent -> Child = temp;
    }
}

void ListDirectory(Node* _current)
{
    Node* current = _current;

    LinkedList_NodeChain* item = GetFirstChild(current);
    while (item != NULL)
    {
        Node* node = item -> Value;
        if (node != NULL) if (node -> Data != NULL) if (node -> Data -> Type == 0) Purple(1);
        printf("%s      ", GetNameLLNC(item));
        ResetColor(0);
        item = GetNext(item);
    }

    printf("\n");
}

Node* CreateNode(FileData* data, Node* parent, char name[256])
{
    Node* node = malloc(sizeof(Node));
    if (node != NULL)
    {
        node -> Data = data;
        node -> Parent = parent;
        strcpy(node -> Name, name);
        if (parent != NULL)
        {
            InsertNode(parent, node);
        }
    }

    return node;
}

FileData* CreateFileData(int Type, char* Data)
{
    FileData* fileData = malloc(sizeof(FileData));
    if (fileData != NULL) 
    {
        fileData -> Type = Type;
        fileData -> Data = Data;
    }

    return fileData;
}

int NodeCount(Node* node)
{
    if (node == NULL) return 0;
    LinkedList_NodeChain* linkedList = node -> Child;
    int ChildrenCount = 0;
    for (LinkedList_NodeChain* n = linkedList; n != NULL; n = n -> Next)
    {     
        ChildrenCount += NodeCount(n -> Value) + 1;
    }
    return ChildrenCount;
}

void ShowTree(Node* node, int level)
{
    if (node != NULL)
    {
        if (node -> Data != NULL)
        {
            if (node -> Data -> Type == 0) Purple(1);
        }
        printf("%s\n", node -> Name);
        ResetColor(0);        
        LinkedList_NodeChain* linkedList = node -> Child;
        for (LinkedList_NodeChain* n = linkedList; n != NULL; n = n -> Next)
        {     
            int IsEndPath = n -> Next == NULL;
            for (int i = 0; i < level; i++) printf("|   ");
            printf("%s ", (IsEndPath ? END_PATH : MIDDLE_PATH));
            ShowTree(n -> Value, level + 1);
        }
    }
}

char* GetName(Node* node)
{
    char* name = malloc(256);
    if (name != NULL)
    {
        strcpy(name, node -> Name);
    }
    return name;
}

Node* GetParent(Node* node)
{
    return node -> Parent;
}

Node* GetNodeValue(LinkedList_NodeChain* chain)
{
    if (chain != NULL) return chain -> Value;
    return NULL;
}

LinkedList_NodeChain* GetNext(LinkedList_NodeChain* llnc)
{
    if (llnc == NULL) return NULL;
    return llnc -> Next;
}

LinkedList_NodeChain* GetFirstChild(Node* node)
{
    return node -> Child;
}

char* GetNameLLNC(LinkedList_NodeChain* llnc)
{
    if (llnc == NULL) return NULL;
    if (llnc -> Value == NULL) return NULL;
    return llnc -> Value -> Name;
}

FileData* NewFileData(FileData* copy)
{
    FileData* data = (FileData*)malloc(sizeof(FileData));
    
    if (data != NULL)
    {
        if (copy != NULL)
        {
            data -> Type = copy -> Type;
            data -> Data = copy -> Data;
        }
    }

    return data;
}

void SetFileData(FileData* data, int Type, char* Data)
{
    if (data != NULL)
    {
        data -> Type = Type;
        data -> Data = Data;
    }
}

FileData* GetData(Node* node)
{
    if (node == NULL) return NULL;
    return node -> Data;
}

int GetDataType(FileData* data)
{
    if (data == NULL) return 0;
    return data -> Type;
}

char* GetDataContent(FileData* data)
{
    if (data == NULL) return 0;
    return data -> Data;
}
